/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 19:17:11 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/15 10:18:46 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

int	len_liste(t_cmds *cmd)
{
	int	len;

	len = 0;
	while (cmd != NULL)
	{
		cmd = cmd->next;
		len ++;
	}
	return (len);
}

int	env_len(t_envp *env)
{
	int	len;

	len = 0;
	while (env != NULL)
	{
		env = env->next;
		len ++;
	}
	return (len);
}

char	**env_joiner(t_envp *env)
{
	char	**str;
	int		len;
	int		i;

	i = 0;
	len = env_len(env);
	str = malloc(sizeof(char *) * (len + 1));
	if (str == NULL)
		return (NULL);
	str[len] = NULL;
	while (env != NULL)
	{
		str[i] = ft_strjoin(env->env_pre, env->envp_value);
		env = env->next;
		i++;
	}
	return (str);
}

void	signal_exec(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler_exec(int num)
{
	if (num == SIGINT)
		printf("\n");
}
