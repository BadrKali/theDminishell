/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:10:14 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/13 12:48:22 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_there_forwardslash(char *cmd)
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

static void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
}

void	handle_argument(t_tokens **token, char **args, int *i, int *is_cmd)
{
	if (((*token)->type == VAR && ft_strlen((*token)->value))
		|| (*token)->type == WORD || (*token)->type == QUOTES
		|| (*token)->type == S_QUOTES)
		get_arguments_two(token, args, i, is_cmd);
	else
		*token = (*token)->next;
}

char	*get_path(t_envp *envp, char *cmd)
{
	char	*path;
	char	**tab;
	int		is_forwardslash;
	int		i;

	i = 0;
	is_forwardslash = is_there_forwardslash(cmd);
	path = get_env_value(envp, "$PATH");
	if (!path && !is_forwardslash)
		return (NULL);
	else if (is_forwardslash)
		return (ft_strdup(cmd));
	tab = ft_split(path, ':');
	while (tab[i] && !ft_strcmp(cmd, "export")
		&& ft_strlen(cmd) && !ft_strcmp("..", cmd)
		&& !ft_strcmp(".", cmd))
	{
		path = ft_strjoin(tab[i], "/");
		path = ft_strjoin(path, cmd);
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
			return (free_tab(&tab[++i]), free(tab), path);
		free(path);
		i++;
	}
	return (free_tab(&tab[i]), free(tab), ft_strdup(cmd));
}

char	*fill_arg(t_tokens **token)
{
	char	*arg;

	arg = ft_strdup((*token)->value);
	*token = (*token)->next;
	return (arg);
}
