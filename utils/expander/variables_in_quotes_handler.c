/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_in_quotes_handler.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:51:39 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/05 11:48:43 by abahsine         ###   ########.fr       */
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
	if (len == 1 || (var[1] && (var[1] == '|' || var[ 1] == '<'
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

static char	*rewrite_string(char *value, char *var, t_envp *tmp)
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
	expanded = ft_strdup(tmp->envp_value);
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

static int	is_quotes_only_space(char *value)
{
	int	i;

	i = 0;
	while (value[i] && (value[i] == ' ' || value[i] == '\t'
		|| value[i] == '\n' || value[i] == '\r' || value[i] == '\f'
		|| value[i] == '\v'))
		i++;
	if (value[i] == '\0')
		return (1);
	return (0);
}

void    handle_variables_in_quotes(t_tokens **token, t_envp *envp, t_tokens **head)
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
			(*token)->value = rewrite_string((*token)->value, var[i], tmp);
		else
			(*token)->value = remove_false_variables((*token)->value, var[i]);
		free(var[i]);
		i++;
	}
	free(var);
	if (ft_strlen((*token)->value) && is_quotes_only_space((*token)->value))
		ft_deleteNode(head, *token);
}
