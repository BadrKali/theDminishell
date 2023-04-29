/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 21:25:42 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/29 14:40:04 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*ft_get_var(t_env *envp, char *value)
{
	while (envp)
	{
		if (ft_strcmp(envp->name, value))
			return (envp);
		envp = envp->next;
	}
	return (NULL);
}

char	*ft_remove_name(char *env_name)
{
	int	i;

	i = 0;
	while (env_name && env_name[i] != '=')
		i++;
	if (env_name && env_name[i] == '=')
		i++;
	return (ft_substr(env_name, i, ft_strlen(env_name) - i));
}

int	ft_get_envs(char *value)
{
	int	i;
	int	count;

	i = 0;
	count = 0;

	while (value && value[i])
	{
		if (value[i] == '$')
		{
			i++;
			if (value[i] && ft_isdigit(value[i]))
			{
				count++;
				i++;
			}
			else if (value[i] && !ft_isalpha(value[i]))
				i++;
			else
			{
				while (value[i] && ft_isalpha(value[i]))
					i++;
				count++;
			}
		}
		if (value[i] != '$')
			i++;
	}
	return (count);
}

char	**ft_check_for_var(char *value)
{
	char	**envs;
	int		start;
	int		i;
	int		j;

	i = 0;
	j = 0;
	envs = malloc((ft_get_envs(value) + 1) * sizeof(char *));
	if (!envs)
		return (NULL);
	while (value[i])
	{
		if (value[i] == '$')
		{
			start = i++;
			if (value[i] && ft_isdigit(value[i]))
				envs[j++] = ft_substr(value, start, ++i - start);
			else if (value[i] && !ft_isalpha(value[i]))
				i++;
			else
			{
				while (value[i] && ft_isalpha(value[i]))
					i++;
				envs[j++] = (ft_substr(value, start, i - start));
			}
		}
		if (value[i] && value[i] != '$')
			i++;
	}
	envs[j] = 0;
	return (envs);
}

int	ft_find_var_position(char *value, char *var)
{
	int	pos;
	int	i;
	int	j;

	pos = 0;
	j = 0;
	i = 0;
	while (value[i])
	{
		pos += j;
		j = 0;
		while (value[i] && value[i] == var[j])
		{
			i++;
			j++;
		}
		if (var[j] == '\0')
			return (pos);
		i++;
		pos++;
	}
	return (-1);
}