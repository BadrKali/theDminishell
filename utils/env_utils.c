/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:32:31 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 14:34:00 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_envp	*ft_lstnew_envp(char *value, char *name)
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

void	ft_lstadd_back_envp(t_envp **lst, t_envp *new)
{
	if (*lst == NULL)
		*lst = new;
	else
	{
		new->prev = ft_lstlast_envp(*lst);
		ft_lstlast_envp(*lst)->next = new;
	}
}
