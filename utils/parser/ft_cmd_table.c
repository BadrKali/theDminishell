/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmd_table.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 17:00:46 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/15 17:00:46 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_get_cmd(t_tokens *token)
{
	while (token && token->type != PIPE)
	{
		if (token->type == WORD)
			return (token->value);
		token = token->next;
	}
	return (NULL);
}

void	ft_skip_redirection(t_tokens **token)
{
	*token = (*token)->next;
	if (*token && (*token)->type == T_SPACE)
		*token = (*token)->next;
	if (*token && (*token)->type == ARG)
		*token = (*token)->next;
}

int	ft_get_args_len(t_tokens *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
			|| token->type == HEREDOC_OPERATOR || token->type == APPEND_OPERATOR)
			ft_skip_redirection(&token);
		if (token && token->type == T_SPACE)
			token = token->next;
		if (token && token->type == ARG)
		{
			token = token->next;
			count++;
		}
		else if (token)
			token = token->next;
	}
	return (count);
}

char	**ft_get_args(t_tokens *token)
{
	char	**args;
	int		i;

	i = 0;
	args = malloc((ft_get_args_len(token) + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	while (token && token->type != PIPE)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
			|| token->type == HEREDOC_OPERATOR || token->type == APPEND_OPERATOR)
			ft_skip_redirection(&token);
		if (token && token->type == T_SPACE)
			token = token->next;
		if (token && token->type == ARG)
		{
			args[i++] = token->value;
			token = token->next;
		}
		else if (token && token->type != OR_OPERATOR && token->type != IR_OPERATOR
			&& token->type != HEREDOC_OPERATOR && token->type != APPEND_OPERATOR
			&& token->type != PIPE)
			token = token->next;
	}
	args[i] = 0;
	return (args);
}

char	*ft_get_delimiter(t_tokens *token)
{
	token = token->next;
	if (token->type == T_SPACE)
		token = token->next;
	return (token->value);
}

void	ft_change_red_value(t_tokens **token)
{
	free((*token)->value);
	(*token)->value = ft_strdup("<");
	*token = (*token)->next;
}

void	ft_change_delimiter_value(t_tokens **token, char *file_name)
{
	free((*token)->value);
	(*token)->value = file_name;
	*token = (*token)->next;
}

void	ft_change_token_value(t_tokens **token, char *file_name)
{
	ft_change_red_value(token);
	if (*token && (*token)->type == T_SPACE)
		*token = (*token)->next;
	if (*token && (*token)->type == ARG)
		ft_change_delimiter_value(token, file_name);
}

void	ft_handle_heredoc(t_tokens **tokens, int file_index)
{
	char	*input;
	char	*delimiter;
	char	*file_name;
	int		fd;

	file_name = ft_strjoin(ft_strdup("/tmp/tmp"), ft_itoa(file_index));
	file_name = ft_strjoin(file_name, ft_strdup(".txt"));
	fd = open(file_name, O_RDWR | O_CREAT, 0777);
	if (fd == -1)
		return ;
	delimiter = ft_get_delimiter(*tokens);
	while ((input = readline("> ")) != NULL)
	{
		if (ft_strcmp(input, delimiter))
			break ;
		ft_putstr_fd(input, fd);
		ft_putstr_fd("\n", fd);
	}
	close(fd);
	ft_change_token_value(tokens, file_name);
}

// int	ft_handle_input_red(t_tokens **token, int std_in)
// {
// 	int	fd;

// 	*token = (*token)->next;
// 	if (*token && (*token)->type == T_SPACE)
// 		(*token) = (*token)->next;
// 	if (!(*token))
// 		return ;
// 	fd = open((*token)->value, O_RDONLY, 0777);
// 	if (fd == -1)
// 		return ;
// 	return (fd);	
// }

int	*ft_handle_redirections(t_tokens **token)
{
	t_tokens	*tmp;
	int			*stds;
	static int	file_index;

	tmp = *token;
	stds = malloc(2 * sizeof(int *));
	if (!stds)
		return (NULL);
	stds[1] = 1;
	stds[0] = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == HEREDOC_OPERATOR)
			ft_handle_heredoc(&tmp, file_index);
		if (tmp)
			tmp = tmp->next;
	}
	while (*token && (*token)->type != PIPE)
	{
		// if ((*token)->type == IR_OPERATOR)
		// 	stds[0] = ft_handle_input_red(token);
		// else if ((*token)->type == IR_OPERATOR)
		// 	ft_handle_output_red();
		// else if ((*token)->type == APPEND_OPERATOR)
		// 	ft_handle_append_op();
		*token = (*token)->next;
	}
	return (stds);
}

void	ft_get_to_end(t_tokens **token)
{
	while (*token && (*token)->type != PIPE)
		*token = (*token)->next;
	if (*token && (*token)->type == PIPE)
		*token = (*token)->next;
}

void	ft_cmd_table(t_tokens *token, t_cmds **cmds)
{
	char	*cmd;
	int		*stds;
	char	**args;

	while (token)
	{
		cmd = ft_get_cmd(token);
		args = ft_get_args(token);
		stds = ft_handle_redirections(&token);
		ft_lstadd_back_cmd(cmds, ft_lstnew_cmd(cmd, args, stds));
		ft_get_to_end(&token);
	}
}