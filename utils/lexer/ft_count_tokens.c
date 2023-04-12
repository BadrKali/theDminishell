/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 20:13:34 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/12 02:35:21 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_get_quotes_len(char *input, int *i, int *count, char quote)
{
	(*count)++;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] && input[*i] == quote)
		(*i)++;
}

void	ft_get_spaces_len(char *input, int *i, int *count)
{
	(*count)++;
	(*i)++;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
}

void	ft_get_redirections_len(int *i, int *count, int flag)
{
	if (flag == 1)
	{
		(*count)++;
		(*i) += 2;
	}
	else if (flag == 2)
	{
		(*count)++;
		(*i)++;
	}
}

void	ft_get_var_len(char *input, int *i, int *count)
{
	(*count)++;
	(*i)++;
	if (input[*i] == ' ')
		return ;
	if (input[*i] && (input[*i] != '_' || !ft_isalpha(input[*i])))
		(*i)++;
	while (input[*i] && input[*i] != ' ' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '$' && input[*i] != '\''
		&& input[*i] != '\"' && input[*i] != '|')
		(*i)++;
}

void	ft_get_string_len(char *input, int *i, int *count)
{
	(*count)++;
	(*i)++;
	while (input[*i] && input[*i] != ' ' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '$' && input[*i] != '\''
		&& input[*i] != '\"' && input[*i] != '|')
		(*i)++;
}