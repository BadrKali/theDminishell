/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 16:32:20 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/08 16:44:34 by abahsine         ###   ########.fr       */
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
