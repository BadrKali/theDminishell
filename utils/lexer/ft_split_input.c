/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 20:14:31 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/12 02:23:46 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//! Handle unclosed quotes better

char	*ft_handle_quotes(char	*input, int *i, char quote)
{
	int		start;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] && input[*i] == quote)
		return (ft_substr(input, start, ++(*i) - start));
	return (ft_putstr_fd("You must close quotes...\n", 2), NULL);
}

char	*ft_handle_space(char *input, int *i)
{
	int		start;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] == ' ')
		(*i)++;
	return (ft_substr(input, start, (*i) - start));
}

char	*ft_hanlde_redirections(char *input, int *i, int flag)
{
	int		start;

	start = *i;
	if (flag == 1)
	{
		(*i) += 2;
		return (ft_substr(input, start, (*i) - start));
	}
	else if (flag == 2)
	{
		(*i)++;
		return (ft_substr(input, start, (*i) - start));
	}
	return (NULL);
}

//! came back and check this

char	*ft_handle_variable(char *input, int *i)
{
	int		start;

	start = *i;
	(*i)++;
	if (input[*i] == ' ')
		return (ft_substr(input, start, (*i) - start));
	if (input[*i] && (input[*i] != '_' && !ft_isalpha(input[*i])))
		return (ft_substr(input, start, ++(*i) - start));
	while (input[*i] && ft_isalpha(input[*i]))
		(*i)++;
	return (ft_substr(input, start, (*i) - start));
}

char	*ft_handle_string(char *input, int *i)
{
	int		start;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != ' ' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '$' && input[*i] != '\''
		&& input[*i] != '\"' && input[*i] != '|')
		(*i)++;
	return (ft_substr(input, start, (*i) - start));
}