/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:34:50 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/12 22:34:06 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// int    ft_check_syntax(t_tokens *token)
// {
// 	if (token && (token->type == PIPE || (token->next
// 		&& token->next->type == PIPE && (token->type == HEREDOC_OPERATOR
// 		|| token->type == APPEND_OPERATOR || token->type == IR_OPERATOR
// 		|| token->type == OR_OPERATOR))))
// 		return (ft_putstr_fd(Err_PIPE "\n", 2), 1);
// 	while (token)
// 	{
// 		if (token->next && token->next->next && (token->type == HEREDOC_OPERATOR
// 		|| token->type == APPEND_OPERATOR || token->type == IR_OPERATOR
// 		|| token->type == OR_OPERATOR) && (token->next->type == SPACE
// 		&& token->next->next->type == PIPE))
// 			return (ft_putstr_fd(Err_PIPE "\n", 2), 1);
// 		token = token->next;
// 	}
// 	return (0);
// }

int	ft_check_before_pipe(t_tokens *token)
{
	if (token->next && token->next->type == SPACE)
	{
		if (token->next->next && token->next->next->type == PIPE)
			return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	}
	else if (token->next && token->next->type == PIPE)
		return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	return (0);
}

int	ft_check_after_pipe(t_tokens *token)
{
	if (!token->next || (token->next->type == SPACE && !token->next->next))
		return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	if (token->next && token->next->type == SPACE)
	{
		if (token->next->next && (token->next->next->type == OR_OPERATOR
			|| token->next->next->type == IR_OPERATOR
			|| token->next->next->type == HEREDOC_OPERATOR
			|| token->next->next->type == APPEND_OPERATOR))
		return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	}
	else if (token->next && token->next->next && (token->next->next->type == OR_OPERATOR
		|| token->next->next->type == IR_OPERATOR
		|| token->next->next->type == HEREDOC_OPERATOR
		|| token->next->next->type == APPEND_OPERATOR))
		return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	return (0);
}

int	ft_check_end(t_tokens *token)
{
	int	tok;

	tok = ft_lstlast_token(token)->type;
	if (tok == OR_OPERATOR || tok == IR_OPERATOR || tok == HEREDOC_OPERATOR
		|| tok == APPEND_OPERATOR)
		return (ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2), 1);
	return (0);
}

int    ft_check_syntax(t_tokens *token)
{
	if (ft_check_end(token))
		return (1);
	while (token)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
		|| token->type == HEREDOC_OPERATOR || token->type == APPEND_OPERATOR)
			if (ft_check_before_pipe(token))
				return (1);
		if (token->type == PIPE)
			if (ft_check_after_pipe(token))
				return (1);
		token = token->next;
	}
	return (0);
}