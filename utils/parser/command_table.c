/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:07:26 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/12 17:38:38 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_command(t_tokens *token)
{
	t_tokens	*tmp;

	tmp = token;
	skip_redirections(&tmp);
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == WORD || tmp->type == QUOTES
			|| tmp->type == S_QUOTES || tmp->type == VAR)
			if ((tmp->type == VAR && ft_strlen(tmp->value))
				|| tmp->type == WORD || tmp->type == QUOTES
				|| tmp->type == S_QUOTES)
				return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	get_arguments_two(t_tokens **token, char **args, int *i, int *is_cmd)
{
	if (*is_cmd)
	{
		*is_cmd = 0;
		*token = (*token)->next;
	}
	else if ((*token)->next && (*token)->next->type != OR_OPERATOR
		&& (*token)->next->type != IR_OPERATOR
		&& (*token)->next->type != HEREDOC_OPERATOR
		&& (*token)->next->type != APPEND_OPERATOR
		&& (*token)->next->type != PIPE
		&& (*token)->next->type != T_SPACE)
		args[(*i)++] = merge_args(token);
	else
		args[(*i)++] = fill_arg(token);
}

char	**get_arguments(t_tokens *token, t_envp *envp, char *cmd)
{
	char	**args;
	int		i;
	int		is_cmd;

	i = 0;
	is_cmd = 1;
	args = malloc((get_arguments_len(token) + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	if (cmd)
		args[i++] = get_path(envp, cmd);
	while (token && token->type != PIPE)
	{
		skip_redirections(&token);
		if (token && (token->type == WORD || token->type == QUOTES
				|| token->type == S_QUOTES || token->type == VAR))
		{
			if ((token->type == VAR && ft_strlen(token->value)) || token->type == WORD || token->type == QUOTES
				|| token->type == S_QUOTES)
				get_arguments_two(&token, args, &i, &is_cmd);
			else
				token = token->next;
		}
		else if ((token && token->type != OR_OPERATOR
				&& token->type != IR_OPERATOR
				&& token->type != HEREDOC_OPERATOR
				&& token->type != APPEND_OPERATOR && token->type != PIPE))
			token = token->next;
	}
	args[i] = 0;
	return (args);
}

void	get_to_end(t_tokens **token)
{
	while (*token && (*token)->type != PIPE)
		*token = (*token)->next;
	if (*token && (*token)->type == PIPE)
		*token = (*token)->next;
	if (*token && (*token)->type == T_SPACE)
		*token = (*token)->next;
}

void	command_table(t_tokens *token, t_cmds **cmd, t_envp *envp)
{
	char	*command;
	int		*stds;
	char	**args;

	while (token)
	{
		command = get_command(token);
		args = get_arguments(token, envp, command);
		stds = handle_redirections_for_cmd(&token, envp);
		ft_lstadd_back_cmd(cmd, ft_lstnew_cmd(command, args, stds));
		get_to_end(&token);
	}
}
