/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:21:16 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/04 17:36:13 by abahsine         ###   ########.fr       */
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

static t_envp	*ft_lstnew_envp(char *value, char *name)
{
	t_envp	*envp;

	envp = (t_envp *)malloc(sizeof(t_envp));
	if (!envp)
		return (NULL);
	envp->envp_value = value;
	envp->envp_name = name;
	envp->next = NULL;
	envp->prev = NULL;
	return (envp);
}

static t_envp	*ft_lstlast_envp(t_envp *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

static void	ft_lstadd_back_envp(t_envp **lst, t_envp *new)
{
	if (*lst == NULL)
		*lst = new;
	else
	{
		new->prev = ft_lstlast_envp(*lst);
		ft_lstlast_envp(*lst)->next = new;
	}
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
		ft_lstadd_back_envp(envp, ft_lstnew_envp(remove_name(env[i]), get_variable_name(env[i])));
		i++;
	}
}