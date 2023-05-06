/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:07:26 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/06 19:51:31 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	skip_redirections(t_tokens **token)
{
	while (*token && ((*token)->type == OR_OPERATOR
		|| (*token)->type == IR_OPERATOR || (*token)->type == HEREDOC_OPERATOR
		|| (*token)->type == APPEND_OPERATOR))
	{
		*token = (*token)->next;
		if (*token && (*token)->type == T_SPACE)
			*token = (*token)->next;
		if (*token)
			*token = (*token)->next;
		if (*token && (*token)->type == T_SPACE)
			*token = (*token)->next;
	}
}

char	*get_command(t_tokens *token)
{
	t_tokens	*tmp;

	tmp = token;
	skip_redirections(&tmp);
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == WORD || tmp->type == QUOTES
			|| tmp->type == S_QUOTES || tmp->type == VAR)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	get_arguments_len(t_tokens *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		skip_redirections(&token);
		if (token && (token->type == WORD || token->type == QUOTES
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

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
}

char	*get_path(char *cmd)
{
	char	*PATH;
	char	**tab;
	int		is_forwardslash;
	int		i;

	i = 0;
	PATH = getenv("PATH");
	tab = ft_split(PATH, ':');
	is_forwardslash = is_there_forwardslash(cmd);
	while (tab[i] && !is_forwardslash && !ft_strcmp(cmd, "export") && ft_strlen(cmd))
	{
		PATH = ft_strjoin(tab[i], "/");
		PATH = ft_strjoin(PATH, cmd);
		if (access(PATH, F_OK) == 0 && access(PATH, X_OK) == 0)
			return (free_tab(&tab[++i]), free(tab), PATH);
		free(PATH);
		i++;
	}
	return (free_tab(&tab[i]), free(tab), ft_strdup(cmd));
}

char	*merge_args(t_tokens **token)
{
	char	*arg;

	arg = ft_strdup((*token)->value);
	*token = (*token)->next;
	while (*token && (*token)->type != OR_OPERATOR && (*token)->type != IR_OPERATOR
		&& (*token)->type != HEREDOC_OPERATOR && (*token)->type != APPEND_OPERATOR
		&& (*token)->type != PIPE && (*token)->type != T_SPACE)
	{
		arg = ft_strjoin(arg, (*token)->value);
		*token = (*token)->next;
	}
	return (arg);
}

char	*fill_arg(t_tokens **token)
{
	char	*arg;

	arg = ft_strdup((*token)->value);
	*token = (*token)->next;
	return (arg);
}

char	**get_arguments(t_tokens *token, char *cmd)
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
		args[i++] = get_path(cmd);
	while (token && token->type != PIPE)
	{
		skip_redirections(&token);
		if (token && (token->type == WORD || token->type == QUOTES
			|| token->type == S_QUOTES || token->type == VAR))
		{
			if (is_cmd)
			{
				is_cmd = 0;
				token = token->next;
			}
			else if (token->next && token->next->type != OR_OPERATOR && token->next->type != IR_OPERATOR
			&& token->next->type != HEREDOC_OPERATOR && token->next->type != APPEND_OPERATOR
			&& token->next->type != PIPE && token->next->type != T_SPACE)
				args[i++] = merge_args(&token);
			else
				args[i++] = fill_arg(&token);
		}
		else if ((token && token->type != OR_OPERATOR && token->type != IR_OPERATOR
			&& token->type != HEREDOC_OPERATOR && token->type != APPEND_OPERATOR
			&& token->type != PIPE))
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
}

void	find_heredocs(t_tokens **token, t_envp *envp, int *file_index)
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

int	check_variable_syntax(char *val, int type)
{
	if (type == VAR)
		if (is_there_space(val))
			return (1);
	return (0);
}

int	is_there_same_type_next(t_tokens *token, int type)
{
	while (token && token->type != PIPE)
	{
		if ((type == 1 && (token->type == OR_OPERATOR
			|| token->type == APPEND_OPERATOR))
			|| (type == 2 && token->type == IR_OPERATOR))
			return (1);
		token = token->next;
	}
	return (0);
}

int	handle_input_redirection(t_tokens **token)
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

int	handle_output_redirection(t_tokens **token)
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

int	handle_append_operator(t_tokens **token)
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

int	*allocate_for_stds(int std_in, int std_out)
{
	int	*stds;

	stds = malloc(2 * sizeof(int));
	stds[0] = std_in;
	stds[1] = std_out;
	return (stds);
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

void	command_table(t_tokens *token, t_cmds **cmd, t_envp *envp)
{
	char	*command;
	int		*stds;
	char	**args;

	while (token)
	{
		command = get_command(token);
		args = get_arguments(token, command);
		// if ((args[0] && !ft_strcmp(args[0], "export")) && access(args[0], F_OK) != 0
		// 	&& access(args[0], X_OK) != 0)
		// 	command = NULL;
		stds = handle_redirections_for_cmd(&token, envp);
		ft_lstadd_back_cmd(cmd, ft_lstnew_cmd(command, args, stds));
		get_to_end(&token);
	}
}