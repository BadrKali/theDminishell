/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:18:13 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/29 18:53:28 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*rewrite_string_heredoc(char *value, char *var, t_env *tmp)
{
	char	*first_half;
	char	*second_half;
	char	*expanded;
	int		pos;
	int		len;

	len = ft_strlen(var);
	if (len == 1 || (var[1] && (var[1] == '|' || var[ 1] == '<'
		|| var[1] == '>' || var[1] == '\'' || var[1] == '\"')))
		return (value);
	expanded = ft_remove_name(tmp->env_name);
	pos = ft_find_var_position(value, var);
	first_half = ft_strdup("");
	if (pos != 0)
		first_half = ft_substr(value, 0, pos);
	first_half = ft_strjoin(first_half, expanded);
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	return (ft_strjoin(first_half, second_half));
}

char	*remove_false_vars_heredoc(char *value, char *var)
{
	char	*first_half;
	char	*second_half;
	int		pos;
	int		len;
	
	len = ft_strlen(var);
	if (len == 1 || (var[1] && (var[1] == '|' || var[ 1] == '<'
		|| var[1] == '>' || var[1] == '\'' || var[1] == '\"')))
		return (value);
	first_half = ft_strdup("");
	pos = ft_find_var_position(value, var);
	if (pos != 0)
		first_half = ft_substr(value, 0, pos);
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	return (ft_strjoin(first_half, second_half));
}

char	*handle_heredoc_vars(char *input, t_env *envp)
{
	t_env	*tmp;
	char	**var;
	int		i;

	i = 0;
	var = ft_check_for_var(input);
	while (var[i])
	{
		tmp = ft_get_var(envp, var[i]);
		if (tmp)
			input = rewrite_string_heredoc(input, var[i], tmp);
		else
			input = remove_false_vars_heredoc(input, var[i]);
		i++;
	}
	return (input);
}

void	handle_heredoc(t_tokens **tokens, t_env *envp, int file_index)
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
		input = handle_heredoc_vars(input, envp);
		ft_putstr_fd(input, fd);
		ft_putstr_fd("\n", fd);
	}
	close(fd);
	ft_change_token_value(tokens, file_name);
}