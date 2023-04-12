/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analyzer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 20:12:09 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/10 01:19:39 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_count_tokens(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			ft_get_quotes_len(input, &i, &count, input[i]);
		else if (input[i] == ' ')
			ft_get_spaces_len(input, &i, &count);
		else if (input[i + 1] && ((input[i] == '<' && input[i + 1] == '<')
				|| (input[i] == '>' && input[i + 1] == '>')))
			ft_get_redirections_len(&i, &count, 1);
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
			ft_get_redirections_len(&i, &count, 2);
		else if (input[i] == '$')
			ft_get_var_len(input, &i, &count);
		else
			ft_get_string_len(input, &i, &count);
	}
	return (count);
}

int	ft_check_token_pos(char **token, int i)
{
	if (i == 0)
		return (0);
	if (token[i - 1] && token[i - 1][0] == '|')
		return (0);
	else if (i < 2)
		return (0);
	if (token[i - 1][0] == ' ' && token[i - 2][0] == '|')
		return (0);
	return (1);
}

void    ft_tokenizer(char **tokens, t_tokens **token)
{
	int i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] == '\'')		
			ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(tokens[i], 1, ft_strlen(tokens[i]) - 2), S_QUOTES));
		else if (tokens[i][0] == '\"')
			ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(tokens[i], 1, ft_strlen(tokens[i]) - 2), QUOTES));
		else if (tokens[i][0] == ' ')
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], SPACE));
		else if (tokens[i][0] && tokens[i][1]
			&& (tokens[i][0] == '<' || tokens[i][1] == '<'))
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], HEREDOC_OPERATOR));
		else if (tokens[i][0] && tokens[i][1]
			&& (tokens[i][0] == '>' || tokens[i][1] == '>'))
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], APPEND_OPERATOR));
		else if (tokens[i][0] == '>')
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], OR_OPERATOR));
		else if (tokens[i][0] == '<')
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], IR_OPERATOR));
		else if (tokens[i][0] == '|')
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], PIPE));
		else if (tokens[i][0] == '$')
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], VAR));
		else if (ft_check_token_pos(tokens, i))
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], ARG));
		else
			ft_lstadd_back_token(token, ft_lstnew_token(tokens[i], WORD));
		i++;
	}
}

int	ft_check_is_end(char *input, int i)
{
	while (input[i] && input[i] == ' ')
		i++;
	if (!input[i])
		return (1);
	return (0);
}

void	ft_split_input(char *input, t_tokens **token)
{
	char	**tokens;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tokens = malloc((ft_count_tokens(input) + 1) * sizeof(char *));
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			tokens[j++] = ft_handle_quotes(input, &i, input[i]);
		else if (input[i] == ' ')
		{
			if (i == 0 || ft_check_is_end(input, i))
				while (input[i] && input[i] == ' ')
					i++;
			else
				tokens[j++] = ft_handle_space(input, &i);
		}
		else if (input[i + 1] && ((input[i] == '<' && input[i + 1] == '<')
				|| (input[i] == '>' && input[i + 1] == '>')))
			tokens[j++] = ft_hanlde_redirections(input, &i, 1);
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
			tokens[j++] = ft_hanlde_redirections(input, &i, 2);
		else if (input[i] == '$')
			tokens[j++] = ft_handle_variable(input, &i);
		else
			tokens[j++] = ft_handle_string(input, &i);
	}
	tokens[j] = 0;
	ft_tokenizer(tokens, token);
}