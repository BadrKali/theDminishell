/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:47:07 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/04 15:49:57 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
	if (type == '\'')
		return (find_quotes(val, '\''));
	else if (type == '\"')
		return (find_quotes(val, '\"'));
	return (0);
}

int	check_is_end(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\n' || input[i] == '\r'
		|| input[i] == '\f' || input[i] == '\v'))
		i++;
	if (!input[i])
		return (1);
	return (0);
}

void	handle_redirections_flag_one(t_tokens **token, char *input, int start, int *end)
{
	(*end) += 2;
	if (input[start] == '<')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*end) - start), HEREDOC_OPERATOR));
	else if (input[start] == '>')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*end) - start), APPEND_OPERATOR));
}

void	handle_redirections_flag_two(t_tokens **token, char *input, int start, int *end)
{
	(*end)++;
	if (input[start] == '<')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*end) - start), IR_OPERATOR));
	else if (input[start] == '>')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*end) - start), OR_OPERATOR));
	else if (input[start] == '|')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*end) - start), PIPE));
}