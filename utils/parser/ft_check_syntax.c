/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:34:50 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/09 17:40:42 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int    ft_check_syntax(t_tokens *token)
{
	if (token->type == PIPE || (token->next
		&& token->next->type == PIPE && (token->type == HEREDOC_OPERATOR
		|| token->type == APPEND_OPERATOR || token->type == IR_OPERATOR
		|| token->type == OR_OPERATOR)))
		return (ft_putstr_fd(Err_PIPE "\n", 2), 1);
	return (0);
}