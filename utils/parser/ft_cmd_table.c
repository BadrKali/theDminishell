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
		if (token->type == WORD || token->type == QUOTES
			|| token->type == S_QUOTES || token->type == VAR)
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
	if (*token)
		*token = (*token)->next;
}

void	skip_command(t_tokens **token, char *cmd)
{
	while (*token && !ft_strcmp((*token)->value, cmd))
		*token = (*token)->next;
	if (*token && ft_strcmp((*token)->value, cmd))
		*token = (*token)->next;
}

int	ft_get_args_len(t_tokens *token, char *cmd)
{
	int	count;

	count = 0;
	if (cmd)
		count++;
	while (token && token->type != PIPE)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
			|| token->type == HEREDOC_OPERATOR || token->type == APPEND_OPERATOR)
			ft_skip_redirection(&token);
		if (token && token->type == T_SPACE)
			token = token->next;
		if (token && (token->type == ARG || token->type == QUOTES
			|| token->type == S_QUOTES || token->type == VAR))
		{
			token = token->next;
			count++;
		}
		else if (token && token->type != PIPE)
			token = token->next;
	}
	return (count);
}

int	is_there_forwardslash(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

char	*get_path(char *cmd)
{
	char	*PATH;
	char	**tab;
	int		i;
	int		is_forwardslash;

	i = 0;
	PATH = getenv("PATH");
	tab = ft_split(PATH, ':');
	is_forwardslash = is_there_forwardslash(cmd);
	while (tab[i] && !is_forwardslash)
	{
		PATH = ft_strjoin(tab[i], "/");
		PATH = ft_strjoin(PATH, cmd);
		if (access(PATH, F_OK) == 0 && access(PATH, X_OK) == 0)
			return (PATH);
		i++;
	}
	return (cmd);
}

char	**ft_get_args(t_tokens *token, char *cmd)
{
	char	**args;
	int		i;

	i = 0;
	if (cmd)
		skip_command(&token, cmd);
	args = malloc((ft_get_args_len(token, cmd) + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	if (cmd)
		args[i++] = get_path(cmd);
	while (token && token->type != PIPE)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
			|| token->type == HEREDOC_OPERATOR || token->type == APPEND_OPERATOR)
			ft_skip_redirection(&token);
		if (token && token->type == T_SPACE)
			token = token->next;
		if (token && (token->type == ARG || token->type == QUOTES
			|| token->type == S_QUOTES))
		{
			if (token->next && token->next->type != OR_OPERATOR && token->next->type != IR_OPERATOR
			&& token->next->type != HEREDOC_OPERATOR && token->next->type != APPEND_OPERATOR
			&& token->next->type != PIPE && token->next->type != T_SPACE)
			{
				args[i] = token->value;
				token = token->next;
				while (token && token->type != OR_OPERATOR && token->type != IR_OPERATOR
					&& token->type != HEREDOC_OPERATOR && token->type != APPEND_OPERATOR
					&& token->type != PIPE && token->type != T_SPACE)
				{
					args[i] = ft_strjoin(args[i], token->value);
					token = token->next;
				}
				i++;
			}
			else
			{
				args[i++] = token->value;
				token = token->next;
			}
		}
		else if (token && token->type == VAR)
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

int	is_there_same_type_next(t_tokens *token, int type)
{
	int	is_found;

	is_found = 0;
	while (token && token->type != PIPE)
	{
		if ((type == 1 && (token->type == OR_OPERATOR
			|| token->type == APPEND_OPERATOR))
			|| (type == 2 && token->type == IR_OPERATOR))
			is_found = 1;
		token = token->next;
	}
	return (is_found);
}

int	is_there_space(char *val)
{
	int	i;

	i = 0;
	while (val[i])
	{
		if (val[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int	check_for_var(char *val, int type)
{
	if (type == VAR)
		if (is_there_space(val))
			return (1);
	return (0);
}

int	ft_handle_input_red(t_tokens **token)
{
	int	fd;

	*token = (*token)->next;
	if (*token && (*token)->type == T_SPACE)
		(*token) = (*token)->next;
	if (!(*token))
		return (0);
	if (check_for_var((*token)->value, (*token)->type))
		return (ft_putstr_fd("minishell: ambiguous redirect\n", 2), 0);
	fd = open((*token)->value, O_RDONLY, 0777);
	if (fd == -1)
		return (-1);
	if (is_there_same_type_next(*token, 2))
		close(fd);
	return (fd);	
}

int	handle_output_redirection(t_tokens **token)
{
	int	fd;

	*token = (*token)->next;
	if (*token && (*token)->type == T_SPACE)
		(*token) = (*token)->next;
	if (!(*token))
		return (1);
	if (check_for_var((*token)->value, (*token)->type))
		return (ft_putstr_fd("minishell: ambiguous redirect\n", 2), 0);
	fd = open((*token)->value, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fd == -1)
		return (1);
	if (is_there_same_type_next(*token, 1))
		close(fd);
	return (fd);
}

int	handle_append_operator(t_tokens **token)
{
	int	fd;

	*token = (*token)->next;
	if (*token && (*token)->type == T_SPACE)
		(*token) = (*token)->next;
	if (!(*token))
		return (1);
	if (check_for_var((*token)->value, (*token)->type))
		return (ft_putstr_fd("minishell: ambiguous redirect\n", 2), 0);
	fd = open((*token)->value, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fd == -1)
		return (1);
	if (is_there_same_type_next(*token, 1))
		close(fd);
	return (fd);
}

int	*ft_handle_redirections(t_tokens **token, t_env *envp)
{
	t_tokens	*tmp;
	int			*stds;
	static int	file_index;
	int			is_error;

	tmp = *token;
	is_error = 0;
	stds = malloc(2 * sizeof(int *));
	if (!stds)
		return (NULL);
	stds[0] = 0;
	stds[1] = 1;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == HEREDOC_OPERATOR)
			handle_heredoc(&tmp, envp, file_index);
		if (tmp)
			tmp = tmp->next;
	}
	while (*token && (*token)->type != PIPE)
	{
		if ((*token)->type == IR_OPERATOR && !is_error)
			stds[0] = ft_handle_input_red(token);
		else if ((*token)->type == OR_OPERATOR && !is_error)
			stds[1] = handle_output_redirection(token);
		else if ((*token)->type == APPEND_OPERATOR && !is_error)
			stds[1] = handle_append_operator(token);
		if (stds[0] == -1)
			is_error = 1;
		if (*token)
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

void	ft_cmd_table(t_tokens *token, t_cmds **cmds, t_env *envp)
{
	char	*cmd;
	int		*stds;
	char	**args;

	while (token)
	{
		cmd = ft_get_cmd(token);
		args = ft_get_args(token, cmd);
		if ((args[0] && !ft_strcmp(args[0], "export")) || (access(args[0], F_OK) != 0 && access(args[0], X_OK) != 0))
			cmd = NULL;
		stds = ft_handle_redirections(&token, envp);
		ft_lstadd_back_cmd(cmds, ft_lstnew_cmd(cmd, args, stds));
		ft_get_to_end(&token);
	}
}