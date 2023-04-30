/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:34:50 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/30 15:48:52 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_syntax_before_pipe(t_tokens *token)
{
	if (token->next && token->next->type == T_SPACE)
	{
		if (token->next->next && token->next->next->type == PIPE)
			return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	}
	else if (token->next && token->next->type == PIPE)
		return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	return (0);
}

int	check_syntax_after_pipe(t_tokens *token)
{
	if (!token->next || (token->next->type == T_SPACE && !token->next->next))
		return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	return (0);
}

int	check_redirections_syntax(t_tokens *token)
{
	while (token)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
			|| token->type == HEREDOC_OPERATOR || token->type == APPEND_OPERATOR)
		{
			if (!token->next)
				return (ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2), 1);
			else if (token->next && token->next->type == T_SPACE)
			{
				token = token->next;
				if (!token)
					return (ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2), 1);
			}			
		}
		token = token->next;
	}
	return (0);
}

int	find_quotes(char *val, int c)
{
	int	i;

	i = 0;
	if (val[i] && val[i] == c)
		i++;
	while (val[i] && val[i] != c)
		i++;
	if ((val[i] && val[i] != c) || !val[i])
	{
		
		return (ft_putstr_fd("minishell: Quotes are not being properly closed!\n", 2), 1);
	}
	return (0);
}

int	check_unclosed_quotes(char *val, int type)
{
	if (type == S_QUOTES)
		return (find_quotes(val, '\''));
	else if (type == QUOTES)
		return (find_quotes(val, '\"'));
	return (0);
}

int    ft_check_syntax(t_tokens *token)
{
	if (check_redirections_syntax(token))
		return (1);
	if (token && token->type == PIPE && !token->prev)
		return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2), 1);
	while (token)
	{
		if (token->type == OR_OPERATOR || token->type == IR_OPERATOR
		|| token->type == HEREDOC_OPERATOR || token->type == APPEND_OPERATOR)
			if (check_syntax_before_pipe(token))
				return (1);
		if (token->type == PIPE)
			if (check_syntax_after_pipe(token))
				return (1);
		token = token->next;
	}
	return (0);
}