/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_in_quotes_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:57:21 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/13 13:03:28 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	get_variables_in_quotes(char **envs, char *value, int *i, int *j)
{
	int	start;

	start = (*i)++;
	if (value[*i] && (ft_isdigit(value[*i]) || value[*i] == '?'))
		envs[(*j)++] = ft_substr(value, start, ++(*i) - start);
	else if (value[*i] && !ft_isalpha(value[*i]))
		(*i)++;
	else
	{
		while (value[*i] && (ft_isalpha(value[*i]) || ft_isdigit(value[*i])
				|| value[*i] == '_'))
			(*i)++;
		envs[(*j)++] = ft_substr(value, start, (*i) - start);
	}
}

static void	count_and_skip_variable(char *value, int *i, int *count)
{
	while (value[*i] && (ft_isalpha(value[*i] || ft_isdigit(value[*i])
				|| value[*i] == '_')))
		(*i)++;
	(*count)++;
}

int	get_envs_in_quotes_len(char *value)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (value && value[i])
	{
		if (value[i] == '$')
		{
			i++;
			if (value[i] && (ft_isdigit(value[i]) || value[i] == '?'))
			{
				count++;
				i++;
			}
			else if (value[i] && !ft_isalpha(value[i]))
				i++;
			else
				count_and_skip_variable(value, &i, &count);
		}
		else
			i++;
	}
	return (count);
}

int	find_variable_position(char *value, char *var)
{
	int	pos;
	int	i;
	int	j;

	pos = 0;
	j = 0;
	i = 0;
	while (value[i])
	{
		pos += j;
		j = 0;
		while (value[i + j] && value[i + j] == var[j])
			j++;
		if (var[j] == '\0')
			return (pos);
		i += j;
		i++;
		pos++;
	}
	return (-1);
}
