/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:13:51 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/12 22:32:04 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_tokens	*ft_lstlast_token(t_tokens *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_token(t_tokens **lst, t_tokens *new)
{
	if (*lst == NULL)
		*lst = new;
	else
	{
		new->prev = ft_lstlast_token(*lst);
		ft_lstlast_token(*lst)->next = new;
	}
}

static t_env	*ft_lstlast_envp(t_env *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_envp(t_env **lst, t_env *new)
{
	if (*lst == NULL)
		*lst = new;
	else
	{
		new->prev = ft_lstlast_envp(*lst);
		ft_lstlast_envp(*lst)->next = new;
	}
}
