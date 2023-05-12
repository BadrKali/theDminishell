/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:53:57 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 15:33:27 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	check_is_after_heredoc(t_tokens **token)
{
	if ((*token)->prev && (*token)->prev->type == T_SPACE)
	{
		if ((*token)->prev->prev
			&& (*token)->prev->prev->type == HEREDOC_OPERATOR)
			return (1);
	}
	else if ((*token)->prev && (*token)->prev->type == HEREDOC_OPERATOR)
		return (1);
	return (0);
}

static int	check_variable_next(t_tokens *token)
{
	token = token->next;
	if (token && token->type == T_SPACE)
		token = token->next;
	if (token && (token->type == QUOTES || token->type == S_QUOTES))
		return (1);
	return (0);
}

static void	handle_dollar_and_two_dollars(t_tokens **token)
{
	if ((*token)->value[1] == '$')
		(*token)->value = ft_itoa(getpid());
	if ((*token)->next != NULL)
		*token = (*token)->next;
	return ;
}

static void	handle_true_variable(t_tokens **token, t_envp *tmp)
{
	free((*token)->value);
	(*token)->value = ft_strdup(tmp->envp_value);
}

void	handle_variables(t_tokens **token, t_envp *envp, t_tokens **head)
{
	t_envp	*tmp;

	if (check_is_after_heredoc(token))
		return ;
	if ((ft_strlen((*token)->value) == 1 && !check_variable_next(*token))
		|| (*token)->value[1] == '$')
		return (handle_dollar_and_two_dollars(token));
	tmp = get_variable(envp, (*token)->value);
	if (tmp)
		handle_true_variable(token, tmp);
	else
		ft_deletenode(head, *token);
}
