/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyzer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:54:09 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/13 13:26:11 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_syntax_before_pipe(t_tokens *token)
{
	if (token->next && token->next->type == T_SPACE)
	{
		if (token->next->next && token->next->next->type == PIPE)
			return (ft_putstr_fd(ERR, 2), 1);
	}
	else if (token->next && token->next->type == PIPE)
		return (ft_putstr_fd(ERR, 2), 1);
	return (0);
}

int	check_syntax_after_pipe(t_tokens *token)
{
	if ((!token->next && token->is_deleted != 1)
		|| (token->next && token->next->type == T_SPACE && !token->next->next
			&& token->is_deleted != 1)
		|| (token->next && token->next->type == PIPE)
		|| (token->next && token->next->type == T_SPACE
			&& token->next->next && token->next->next->type == PIPE))
		return (ft_putstr_fd(ERR, 2), 1);
	return (0);
}

int	check_redirections_syntax(t_tokens *token)
{
	while (token)
	{
		if (token->type == OR_OPERATOR || token->type == HEREDOC_OPERATOR
			|| token->type == IR_OPERATOR || token->type == APPEND_OPERATOR)
		{
			if (!token->next || token->next->type == OR_OPERATOR
				|| token->next->type == IR_OPERATOR
				|| token->next->type == HEREDOC_OPERATOR
				|| token->next->type == APPEND_OPERATOR)
				return (ft_putstr_fd(ERR_RED, 2), 1);
			else if (token->next && token->next->type == T_SPACE)
			{
				token = token->next;
				if (!token->next || token->next->type == OR_OPERATOR
					|| token->next->type == IR_OPERATOR
					|| token->next->type == HEREDOC_OPERATOR
					|| token->next->type == APPEND_OPERATOR)
					return (ft_putstr_fd(ERR_RED, 2), 1);
			}
		}
		token = token->next;
	}
	return (0);
}

int	syntax_checker(t_tokens *token)
{
	if (check_redirections_syntax(token))
		return (1);
	if (token && token->type == PIPE && !token->prev)
		return (ft_putstr_fd(ERR, 2), globale.exit_code = 258, 1);
	while (token)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
			|| token->type == HEREDOC_OPERATOR
			|| token->type == APPEND_OPERATOR)
			if (check_syntax_before_pipe(token))
				return (globale.exit_code = 258, 1);
		if (token->type == PIPE)
			if (check_syntax_after_pipe(token))
				return (globale.exit_code = 258, 1);
		token = token->next;
	}
	return (0);
}
