/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:21:35 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/14 15:08:49 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*rewrite_string_heredoc(char *value, char *var, t_envp *tmp)
{
	char	*first_half;
	char	*second_half;
	char	*expanded;
	int		pos;
	int		len;

	len = ft_strlen(var);
	if (len == 1 || (var[1] && (var[1] == '|' || var[1] == '<'
				|| var[1] == '>' || var[1] == '\'' || var[1] == '\"')))
		return (value);
	expanded = ft_strdup(tmp->envp_value);
	pos = find_variable_position(value, var);
	first_half = ft_strdup("");
	if (pos != 0)
	{
		free(first_half);
		first_half = ft_substr(value, 0, pos);
	}
	first_half = ft_strjoin(first_half, expanded);
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	first_half = ft_strjoin(first_half, second_half);
	return (free(value), free(expanded), free(second_half), first_half);
}

static char	*remove_false_vars_heredoc(char *value, char *var)
{
	char	*first_half;
	char	*second_half;
	int		pos;
	int		len;

	len = ft_strlen(var);
	if (len == 1 || (var[1] && (var[1] == '|' || var[1] == '<'
				|| var[1] == '>' || var[1] == '\'' || var[1] == '\"')))
		return (value);
	first_half = ft_strdup("");
	pos = find_variable_position(value, var);
	if (pos != 0)
	{
		free(first_half);
		first_half = ft_substr(value, 0, pos);
	}
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	first_half = ft_strjoin(first_half, second_half);
	return (free(value), free(second_half), first_half);
}

char	*handle_heredoc_variables(char *input, t_envp *envp)
{
	t_envp	*tmp;
	char	**var;
	int		i;

	i = 0;
	var = find_variables_in_quotes(input);
	while (var[i])
	{
		tmp = get_variable(envp, var[i]);
		if (tmp)
			input = rewrite_string_heredoc(input, var[i], tmp);
		else
			input = remove_false_vars_heredoc(input, var[i]);
		free(var[i]);
		i++;
	}
	free(var);
	return (input);
}

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1);
		globale.heredoc = 1;
		globale.heredoc_tmp = dup(0);
		close(0);
	}
}

int	handle_heredoc(t_tokens **token, t_envp *envp, int file_index)
{
	char	*file_name;
	char	*index;
	int		fd;

	index = ft_itoa(file_index);
	file_name = ft_strjoin(ft_strdup("/tmp/tmp"), index);
	file_name = ft_strjoin(file_name, ".txt");
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		return (free(index), fd);
	read_heredoc_input(token, envp, fd);
	change_token_value(token, file_name);
	return (free(index), 0);
}
