/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:13:51 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/09 19:46:27 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_tokens	*ft_lstlast_token(t_tokens *lst)
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
		ft_lstlast_token(*lst)->next = new;
		new->prev = *lst;
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
		ft_lstlast_envp(*lst)->next = new;
		new->prev = *lst;
	}
}
