/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:11:08 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 14:23:04 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	find_heredocs(t_tokens **token, t_envp *envp, int *file_index)
{
	t_tokens	*tmp;

	tmp = *token;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == HEREDOC_OPERATOR)
			handle_heredoc(&tmp, envp, *file_index);
		if (tmp)
			tmp = tmp->next;
		(*file_index)++;
	}
}

static int	handle_input_redirection(t_tokens **token)
{
	t_tokens	*tmp;
	int			fd;

	tmp = *token;
	tmp = tmp->next;
	if (tmp && tmp->type == T_SPACE)
		tmp = tmp->next;
	if (check_variable_syntax(tmp->value, tmp->type))
		return (ft_putstr_fd("minishell: ambiguous redirect\n", 2), 0);
	fd = open(tmp->value, O_RDONLY, 0777);
	if (fd == -1)
		return (fd);
	if (is_there_same_type_next(tmp, 2))
		close(fd);
	return (fd);
}

static int	handle_output_redirection(t_tokens **token)
{
	t_tokens	*tmp;
	int			fd;

	tmp = *token;
	tmp = tmp->next;
	if (tmp && tmp->type == T_SPACE)
		tmp = tmp->next;
	if (check_variable_syntax(tmp->value, tmp->type))
		return (ft_putstr_fd("minishell: ambiguous redirect\n", 2), 0);
	fd = open(tmp->value, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fd == -1)
		return (fd);
	if (is_there_same_type_next(tmp, 1))
		close(fd);
	return (fd);
}

static int	handle_append_operator(t_tokens **token)
{
	t_tokens	*tmp;
	int			fd;

	tmp = *token;
	tmp = tmp->next;
	if (tmp && tmp->type == T_SPACE)
		tmp = tmp->next;
	if (check_variable_syntax(tmp->value, tmp->type))
		return (ft_putstr_fd("minishell: ambiguous redirect\n", 2), 0);
	fd = open(tmp->value, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fd == -1)
		return (fd);
	if (is_there_same_type_next(tmp, 1))
		close(fd);
	return (fd);
}

int	*handle_redirections_for_cmd(t_tokens **token, t_envp *envp)
{
	int	std_in;
	int	std_out;
	int	file_index;

	std_in = 0;
	std_out = 1;
	file_index = 0;
	find_heredocs(token, envp, &file_index);
	while (*token && (*token)->type != PIPE)
	{
		if ((*token)->type == IR_OPERATOR)
			std_in = handle_input_redirection(token);
		else if ((*token)->type == OR_OPERATOR)
			std_out = handle_output_redirection(token);
		else if ((*token)->type == APPEND_OPERATOR)
			std_out = handle_append_operator(token);
		if (std_in == -1 || std_out == -1)
			return (allocate_for_stds(std_in, std_out));
		*token = (*token)->next;
	}
	return (allocate_for_stds(std_in, std_out));
}
