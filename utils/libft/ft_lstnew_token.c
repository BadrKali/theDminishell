/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:52:41 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 15:35:11 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_deletenode(t_tokens **head_ref, t_tokens *del)
{
	if (*head_ref == NULL || del == NULL)
		return ;
	if (*head_ref == del)
		*head_ref = del->next;
	if (del->next != NULL)
		del->next->prev = del->prev;
	if (del->prev != NULL)
		del->prev->next = del->next;
	free(del->value);
	free(del);
	return ;
}

t_tokens	*ft_lstnew_token(char *value, int type)
{
	t_tokens	*token;

	token = (t_tokens *)malloc(sizeof(t_tokens));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->is_joined = -1;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

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
