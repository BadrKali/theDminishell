/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:45:10 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/13 12:16:31 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_quotes(t_tokens **token, t_envp *envp, char *input, int *i)
{
	char	*tok;
	int		start;
	int		type;

	type = input[*i];
	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != type)
		(*i)++;
	tok = ft_substr(input, start, ++(*i) - start);
	if (check_unclosed_quotes(tok, type))
		return (free(tok), 1);
	if (type == '\"')
	{
		if (check_previous(*token))
			ft_lstadd_back_token(token,
				ft_lstnew_token(ft_substr(tok, 1, ft_strlen(tok) - 2), QUOTES));
		else
			handle_quotes_two(token, envp, tok);
	}
	if (type == '\'')
		ft_lstadd_back_token(token,
			ft_lstnew_token(ft_substr(tok, 1, ft_strlen(tok) - 2), S_QUOTES));
	return (free(tok), 0);
}

void	handle_space(t_tokens **token, char *input, int *i)
{
	int		start;

	start = *i;
	if (*i == 0 || check_is_end(input, *i))
		while (input[*i] && (input[*i] == ' ' || input[*i] == '\n'
				|| input[*i] == '\r' || input[*i] == '\f' || input[*i] == '\v'
				|| input[*i] == '\t'))
			(*i)++;
	else
	{
		(*i)++;
		while (input[*i] && (input[*i] == ' ' || input[*i] == '\n'
				|| input[*i] == '\r' || input[*i] == '\f' || input[*i] == '\v'
				|| input[*i] == '\t'))
			(*i)++;
		ft_lstadd_back_token(token,
			ft_lstnew_token(ft_substr(input, start, (*i) - start), T_SPACE));
	}
}

void	handle_redirections(t_tokens **token, char *input, int *i, int flag)
{
	int		start;

	start = *i;
	if (flag == 1)
		handle_redirections_flag_one(token, input, start, i);
	else if (flag == 2)
		handle_redirections_flag_two(token, input, start, i);
}

void	handle_variable(t_tokens **token, t_envp *envp, char *input, int *end)
{
	int			start;

	start = *end;
	(*end)++;
	if (input[*end] && input[*end] == '?')
		handle_exit_variable(token, end);
	else if (input[*end] && (input[*end] == '\"' || input[*end] == '\''))
		return ;
	else if ((input[*end] && (input[*end] == ' ' || input[*end] == '\n'
				|| input[*end] == '\r' || input[*end] == '\f'
				|| input[*end] == '\v' || input[*end] == '\t'))
		|| !input[*end])
		ft_lstadd_back_token(token,
			ft_lstnew_token(ft_substr(input, start, (*end) - start),
				VAR));
	else if (input[*end] && (input[*end] != '_' && !ft_isalpha(input[*end])))
		(*end)++;
	else
		handle_variable_two(token, envp, input, end);
}

void	handle_string(t_tokens **token, char *input, int *end)
{
	int		start;

	start = *end;
	(*end)++;
	while (input[*end] && input[*end] != ' ' && input[*end] != '\n'
		&& input[*end] != '\r' && input[*end] != '\f' && input[*end] != '\v'
		&& input[*end] != '\t' && input[*end] != '<' && input[*end] != '>'
		&& input[*end] != '$' && input[*end] != '\'' && input[*end] != '\"'
		&& input[*end] != '|')
		(*end)++;
	ft_lstadd_back_token(token,
		ft_lstnew_token(ft_substr(input, start, (*end) - start),
			WORD));
}
