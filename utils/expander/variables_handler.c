/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:53:57 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/15 12:13:22 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_is_after_heredoc(t_tokens **token)
{
	if ((*token)->prev && (*token)->prev->type == T_SPACE)
	{
		if ((*token)->prev->prev
			&& (*token)->prev->prev->type == HEREDOC_OPERATOR)
			return (1);
	}
	else if ((*token)->prev && (*token)->prev->type == HEREDOC_OPERATOR)
		return (1);
	return (0);
}

static void	handle_true_variable(t_tokens **token, t_envp *tmp)
{
	free((*token)->value);
	(*token)->value = ft_strdup(tmp->envp_value);
}

void	handle_variables(t_tokens **token, t_envp *envp)
{
	t_envp	*tmp;

	if (check_is_after_heredoc(token))
		return ;
	tmp = get_variable(envp, (*token)->value);
	if (tmp)
		handle_true_variable(token, tmp);
	else
	{
		free((*token)->value);
		(*token)->value = ft_strdup("");
	}
}
