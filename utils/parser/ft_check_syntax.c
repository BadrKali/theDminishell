/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:34:50 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/10 00:09:41 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int    ft_check_syntax(t_tokens *token)
{
	if (token && (token->type == PIPE || (token->next
		&& token->next->type == PIPE && (token->type == HEREDOC_OPERATOR
		|| token->type == APPEND_OPERATOR || token->type == IR_OPERATOR
		|| token->type == OR_OPERATOR))))
		return (ft_putstr_fd(Err_PIPE "\n", 2), 1);
	// else if (token->next && token->next->next && (token->type == HEREDOC_OPERATOR
	// 	|| token->type == APPEND_OPERATOR || token->type == IR_OPERATOR
	// 	|| token->type == OR_OPERATOR) && (token->next->type == SPACE
	// 	&& token->next->next->type == PIPE))
	// 	return (ft_putstr_fd(Err_PIPE "\n", 2), 1);
	while (token)
	{
		if (token->next && token->next->next && (token->type == HEREDOC_OPERATOR
		|| token->type == APPEND_OPERATOR || token->type == IR_OPERATOR
		|| token->type == OR_OPERATOR) && (token->next->type == SPACE
		&& token->next->next->type == PIPE))
			return (ft_putstr_fd(Err_PIPE "\n", 2), 1);
		token = token->next;
	}
	return (0);
}