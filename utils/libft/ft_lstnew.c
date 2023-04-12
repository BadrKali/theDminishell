/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 18:02:16 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/12 21:16:13 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_remove_node(t_tokens **token)
{
	t_tokens	*tmp2;

	tmp2 = NULL;
	if ((*token)->next)
		tmp2 = (*token)->next;
	else if ((*token)->prev)
		tmp2 = (*token)->prev;
	if ((*token)->prev != NULL)
		(*token)->prev->next = (*token)->next;
	if ((*token)->next != NULL)
		(*token)->next->prev = (*token)->prev;
	free((*token)->value);
	free(*token);
	*token = tmp2;
}

t_tokens	*ft_lstnew_token(char *value, int type)
{
	t_tokens	*token;

	token = (t_tokens *)malloc(sizeof(t_tokens));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

t_env	*ft_lstnew_envp(char *value, char *name)
{
	t_env	*envp;

	envp = (t_env *)malloc(sizeof(t_env));
	if (!envp)
		return (NULL);
	envp->env_name = value;
	envp->name = name;
	envp->next = NULL;
	envp->prev = NULL;
	return (envp);
}