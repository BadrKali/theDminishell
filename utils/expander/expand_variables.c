/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:32:20 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/06 16:47:30 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_envp	*get_variable(t_envp *envp, char *value)
{
	while (envp)
	{
		if (ft_strcmp(envp->envp_name, value))
			return (envp);
		envp = envp->next;
	}
	return (NULL);
}

void	skip_heredoc(t_tokens **token)
{
	*token = (*token)->next;
	if (*token && (*token)->type == T_SPACE)
		*token = (*token)->next;
	if (*token)
		*token = (*token)->next;
}

void	delete_two_exec_spaces(t_tokens **token)
{
	t_tokens	*tmp;

	tmp = *token;
	while (tmp)
	{
		if (tmp && tmp->type == T_SPACE && (tmp->prev == NULL
		|| (tmp->next && tmp->next->type == T_SPACE) || tmp->next == NULL))
			ft_deleteNode(token, tmp);
		tmp = tmp->next;
	}
}

//! Change this one!

void	expand_variables(t_tokens **token, t_envp *envp)
{
	(void)envp;
	delete_two_exec_spaces(token);
}