/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 15:45:10 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/06 19:05:10 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_previous(t_tokens *token)
{
	t_tokens *last;

	last = ft_lstlast_token(token);
	if (last && last->prev && last->prev->type == T_SPACE)
		last = last->prev;
	if (last && last->prev && last->prev->type == HEREDOC_OPERATOR)
		return (1);
	return (0);
}

int	handle_quotes(t_tokens **token, t_envp *envp, char *input, int type, int *i)
{
	char	*tok;
	int		start;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != type)
		(*i)++;
	tok = ft_substr(input, start, ++(*i) - start);
	if (check_unclosed_quotes(tok, type))
		return (1);
	if (type == '\"' && check_previous(*token))
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(tok, 1, ft_strlen(tok) - 2), QUOTES));
	else if (type == '\"')
	{
		t_tokens *test;
		char *res = ft_substr(tok, 1, ft_strlen(tok) - 2);
		ft_lstadd_back_token(token, ft_lstnew_token(res, QUOTES));
		test = ft_lstlast_token(*token);
		handle_variables_in_quotes(&test, envp, token);
	}
	if (type == '\'')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(tok, 1, ft_strlen(tok) - 2), S_QUOTES));
	return (0);
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
		while (input[*i] && (input[*i] == ' ' || input[*i] == '\n' || input[*i] == '\r'
		|| input[*i] == '\f' || input[*i] == '\v' || input[*i] == '\t'))
			(*i)++;
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*i) - start), T_SPACE));
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

char	**split_variable(char *value)
{
	char		**args;
	int			i;

	i = 0;
	args = ft_split(value, ' ');
	return (args);	
}

int	find_space(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

void	handle_variable(t_tokens **token, t_envp *envp, char *input, int *end)
{
	t_envp	*var;
	t_tokens	*tmp;
	int		start;
	char	*res;

	start = *end;
	(*end)++;
	if (input[*end] == ' ')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*end) - start), VAR));
	else if (input[*end] && (input[*end] != '_' && !ft_isalpha(input[*end]) && input[*end] != '\"' && input[*end] != '\''))
		ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, ++(*end) - start), VAR));
	else
	{
		while (input[*end] && (ft_isalpha(input[*end]) || ft_isdigit(input[*end]) || input[*end] == '_'))
			(*end)++;
		res = ft_substr(input, start, (*end) - start);
		if (check_previous(*token))
			ft_lstadd_back_token(token, ft_lstnew_token(res, VAR));
		else
		{	
			var = get_variable(envp, res);
			if (var && find_space(var->envp_value))
			{
				char	**args = split_variable(var->envp_value);
				int		i = 0;
				int		end = 0;
				while (args[end])
					end++;
				end--;
				tmp = ft_lstlast_token(*token);
				if ((var->envp_value[0] == ' ' || var->envp_value[0] == '\n'
				|| var->envp_value[0] == '\r' || var->envp_value[0] == '\f'
				|| var->envp_value[0] == '\v' || var->envp_value[0] == '\t') && tmp && tmp->type != QUOTES)
					ft_lstadd_back_token(token, ft_lstnew_token(ft_strdup(" "), T_SPACE));
				while (args[i])
				{
					if (i != end)
					{
						ft_lstadd_back_token(token, ft_lstnew_token(args[i++], VAR));
						ft_lstadd_back_token(token, ft_lstnew_token(ft_strdup(" "), T_SPACE));
					}
					else
					{
						ft_lstadd_back_token(token, ft_lstnew_token(args[i++], VAR));
						if (var->envp_value[ft_strlen(var->envp_value) - 2] == ' ')
							ft_lstadd_back_token(token, ft_lstnew_token(ft_strdup(" "), T_SPACE));
					}
				}
			}
			else
			{
				t_tokens *test;
				ft_lstadd_back_token(token, ft_lstnew_token(res, VAR));
				test = ft_lstlast_token(*token);
				handle_variables(&test, envp, token);
			}
		}
	}
}

void	handle_string(t_tokens **token, char *input, int *end)
{
	int		start;

	start = *end;
	(*end)++;
	while (input[*end] && input[*end] != ' ' && input[*end] != '\n' && input[*end] != '\r'
		&& input[*end] != '\f' && input[*end] != '\v' && input[*end] != '\t' && input[*end] != '<'
		&& input[*end] != '>' && input[*end] != '$' && input[*end] != '\''
		&& input[*end] != '\"' && input[*end] != '|')
		(*end)++;
	ft_lstadd_back_token(token, ft_lstnew_token(ft_substr(input, start, (*end) - start), WORD));
}