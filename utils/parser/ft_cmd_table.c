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
			|| token->type == S_QUOTES)
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
			|| token->type == S_QUOTES))
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
		tab[i] = ft_strjoin(tab[i], "/");
		tab[i] = ft_strjoin(tab[i], cmd);
		if (access(tab[i], F_OK) == 0 && access(tab[i], X_OK) == 0)
			return (tab[i]);
		i++;
	}
	// if (is_forwardslash && access(cmd, F_OK) == 0
	// 	&& access(cmd, X_OK) == 0)
	// 		return (cmd);
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
	(*token)->type = IR_OPERATOR;
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
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
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

int	ft_handle_input_red(t_tokens **token)
{
	int	fd;

	*token = (*token)->next;
	if (*token && (*token)->type == T_SPACE)
		(*token) = (*token)->next;
	if (!(*token))
		return (0);
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
	fd = open((*token)->value, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fd == -1)
		return (1);
	if (is_there_same_type_next(*token, 1))
		close(fd);
	return (fd);
}

int	*ft_handle_redirections(t_tokens **token)
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
			ft_handle_heredoc(&tmp, file_index);
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

void	ft_cmd_table(t_tokens *token, t_cmds **cmds)
{
	char	*cmd;
	int		*stds;
	char	**args;

	while (token)
	{
		cmd = ft_get_cmd(token);
		args = ft_get_args(token, cmd);
		stds = ft_handle_redirections(&token);
		ft_lstadd_back_cmd(cmds, ft_lstnew_cmd(cmd, args, stds));
		ft_get_to_end(&token);
	}
}