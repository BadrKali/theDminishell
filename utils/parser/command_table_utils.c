/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:10:14 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 18:36:41 by abahsine         ###   ########.fr       */
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

//! Duplicate

// int	ft_memcmp(const void *str1, const void *str2, size_t n)
// {
//         size_t			i;
//         unsigned char	*s1;
//         unsigned char	*s2;

//         s1 = (unsigned char *)str1;
//         s2 = (unsigned char *)str2;
//         i = 0;
//         while (i <= n)
//         {
//                 if (s1[i] != s2[i])
//                     return(1);
//                 i ++;
//         }
//         return (0);
// }

// char	*get_env_value(t_envp *env,char *name)
// {
//     t_envp *tmp;

//     tmp = env;
//     while(tmp != NULL)
//     {
//         if(ft_memcmp(name,tmp->envp_name,ft_strlen(name)) == 0)
//             return(tmp->envp_value);
//         tmp = tmp->next;
//     }
//     return(NULL);
// }

//! ^ to remove later

char	*get_path(t_envp *envp, char *cmd)
{
	char	*path;
	char	**tab;
	int		is_forwardslash;
	int		i;

	i = 0;
	path = get_env_value(envp, "$PATH");
	if (!path)
		return (NULL);
	tab = ft_split(path, ':');
	is_forwardslash = is_there_forwardslash(cmd);
	while (tab[i] && !is_forwardslash && !ft_strcmp(cmd, "export")
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
