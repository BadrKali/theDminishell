/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_in_quotes_handler.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:51:39 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/13 13:27:24 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**find_variables_in_quotes(char *value)
{
	char	**envs;
	int		i;
	int		j;

	i = 0;
	j = 0;
	envs = malloc((get_envs_in_quotes_len(value) + 1) * sizeof(char *));
	if (!envs)
		return (NULL);
	while (value[i])
	{
		if (value[i] == '$')
			get_variables_in_quotes(envs, value, &i, &j);
		else
			i++;
	}
	envs[j] = 0;
	return (envs);
}

static char	*remove_false_variables(char *value, char *var)
{
	char	*first_half;
	char	*second_half;
	int		pos;
	int		len;

	len = ft_strlen(var);
	if (len == 1 || (var[1] && (var[1] == '|' || var[1] == '<'
				|| var[1] == '>' || var[1] == '\'' || var[1] == '\"')))
		return (value);
	first_half = ft_strdup("");
	pos = find_variable_position(value, var);
	if (pos != 0)
	{
		free(first_half);
		first_half = ft_substr(value, 0, pos);
	}
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	first_half = ft_strjoin(first_half, second_half);
	return (free(value), free(second_half), first_half);
}

static char	*rewrite_string(char *value, char *var, char *val)
{
	char	*first_half;
	char	*second_half;
	char	*expanded;
	int		pos;
	int		len;

	len = ft_strlen(var);
	if (len == 1 || (var[1] && (var[1] == '|' || var[1] == '<'
				|| var[1] == '>' || var[1] == '\'' || var[1] == '\"')))
		return (value);
	expanded = ft_strdup(val);
	free(val);
	pos = find_variable_position(value, var);
	first_half = ft_strdup("");
	if (pos != 0)
	{
		free(first_half);
		first_half = ft_substr(value, 0, pos);
	}
	first_half = ft_strjoin(first_half, expanded);
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	first_half = ft_strjoin(first_half, second_half);
	return (free(value), free(expanded), free(second_half), first_half);
}

void	handle_variables_in_quotes(t_tokens **token, t_envp *envp)
{
	t_envp	*tmp;
	char	**var;
	int		i;

	i = 0;
	var = find_variables_in_quotes((*token)->value);
	while (var[i])
	{
		tmp = get_variable(envp, var[i]);
		if (tmp)
			(*token)->value = rewrite_string((*token)->value, var[i],
					ft_strdup(tmp->envp_value));
		else if (ft_strcmp(var[i], "$?"))
			(*token)->value = rewrite_string((*token)->value, var[i],
					ft_itoa(globale.exit_code));
		else
			(*token)->value = remove_false_variables((*token)->value, var[i]);
		free(var[i]);
		i++;
	}
	free(var);
}
