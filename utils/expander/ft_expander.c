/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:47:52 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/29 16:44:44 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_rewrite_string(char *value, char *var, t_env *tmp)
{
	char	*first_half;
	char	*second_half;
	char	*expanded;
	int		pos;
	int		len;

	len = ft_strlen(var);
	if (len == 1 || (var[1] && (var[1] == '|' || var[ 1] == '<'
		|| var[1] == '>' || var[1] == '\'' || var[1] == '\"')))
		return (value);
	expanded = ft_remove_name(tmp->env_name);
	pos = ft_find_var_position(value, var);
	first_half = ft_strdup("");
	if (pos != 0)
		first_half = ft_substr(value, 0, pos);
	first_half = ft_strjoin(first_half, expanded);
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	return (ft_strjoin(first_half, second_half));
}

char	*ft_remove_false_vars(char *value, char *var)
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
	pos = ft_find_var_position(value, var);
	if (pos != 0)
		first_half = ft_substr(value, 0, pos);
	second_half = ft_substr(value, pos + len, ft_strlen(&value[len]));
	return (ft_strjoin(first_half, second_half));
}

void    ft_handle_vars_quotes(t_tokens **token, t_env *envp, t_tokens **head)
{
	t_env	*tmp;
	char	**var;
	int		i;

	i = 0;
	var = ft_check_for_var((*token)->value);
	while (var[i])
	{
		tmp = ft_get_var(envp, var[i]);
		if (tmp)
			(*token)->value = ft_rewrite_string((*token)->value, var[i], tmp);
		else
			(*token)->value = ft_remove_false_vars((*token)->value, var[i]);
		i++;
	}
	if (ft_is_only_space((*token)->value))
		ft_deleteNode(head, *token);
} 


void	ft_handle_vars(t_tokens **token, t_env *envp, t_tokens **head)
{
	t_env	*tmp;

	if (ft_strlen((*token)->value) == 1 || (*token)->value[1] == '$')
	{
		if ((*token)->value[1] == '$')
			(*token)->value = ft_itoa(getpid());
		if ((*token)->next != NULL)
			*token = (*token)->next;
		return ;
	}
	tmp = ft_get_var(envp, (*token)->value);
	if (tmp)
		(*token)->value = ft_remove_name(tmp->env_name);
	else
		ft_deleteNode(head, *token);
}

void	ft_delete_two_exec_spaces(t_tokens **head, t_tokens *token)
{
	if (token && token->type == T_SPACE
	&& (token->prev == NULL
	|| (token->next && token->next->type == T_SPACE)
	|| (token->prev && token->prev->type == T_SPACE)
	|| token->next == NULL))
	ft_deleteNode(head, token);
	token = token->next;
}

void	ft_expand_vars(t_tokens **token, t_env *envp)
{
	t_tokens	*tmp;

	tmp = *token;
	while (tmp)
	{
		if (tmp->type == VAR)
			ft_handle_vars(&tmp, envp, token);
		else if (tmp->type == QUOTES)
			ft_handle_vars_quotes(&tmp, envp, token);
		tmp = tmp->next;
	}
	tmp = *token;
	while (tmp)
	{
		ft_delete_two_exec_spaces(token, tmp);
		tmp = tmp->next;
	}
}