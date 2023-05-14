/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:21:16 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/13 13:29:28 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_variable_name(char *env)
{
	char	*env_name;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (env[i] && env[i] != '=' && env[i] != '+')
		i++;
	env_name = malloc((i + 2) * sizeof(char));
	if (!env_name)
		return (NULL);
	env_name[0] = '$';
	i = 1;
	while (env[j] && env[j] != '=' && env[j] != '+')
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

char	*envp_name(t_envp *env)
{
	int		i;
	int		j;
	char	*name;

	name = malloc(sizeof(char) * ft_strlen(env->envp_name) + 1);
	if (name == NULL)
		return (NULL);
	i = 1;
	j = 0;
	while (env->envp_name[i] != '\0')
	{
		name[j] = env->envp_name[i];
		i++;
		j++;
	}
	name[j] = '=';
	j++;
	name[j] = '\0';
	return (name);
}

void	fill_the_name(t_envp **envp)
{
	t_envp	*tmp;

	tmp = *envp;
	while (tmp)
	{
		tmp->env_pre = envp_name(tmp);
		tmp->flag = 0;
		tmp = tmp->next;
	}
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
	fill_the_name(envp);
}
