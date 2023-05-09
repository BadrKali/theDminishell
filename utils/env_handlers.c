/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:21:16 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 14:34:21 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_variable_name(char *env)
{
	char	*env_name;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (env[i] && env[i] != '=')
		i++;
	env_name = malloc((i + 2) * sizeof(char));
	if (!env_name)
		return (NULL);
	env_name[0] = '$';
	i = 1;
	while (env[j] && env[j] != '=')
	{
		env_name[i] = env[j++];
		i++;
	}
	env_name[i] = '\0';
	return (env_name);
}

char	*remove_name(char *envp_value)
{
	int	i;

	i = 0;
	while (envp_value && envp_value[i] != '=')
		i++;
	if (envp_value && envp_value[i] == '=')
		i++;
	return (ft_substr(envp_value, i, ft_strlen(envp_value) - i));
}

void	fill_env_pointer(t_envp **envp, char *env[])
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_lstadd_back_envp(envp, ft_lstnew_envp(remove_name(env[i]),
				get_variable_name(env[i])));
		i++;
	}
}
