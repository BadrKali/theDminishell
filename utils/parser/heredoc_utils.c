/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:36:03 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 13:59:50 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_delimiter(t_tokens *token)
{
	token = token->next;
	if (token->type == T_SPACE)
		token = token->next;
	return (token->value);
}

void	change_red_value(t_tokens **token)
{
	free((*token)->value);
	(*token)->value = ft_strdup("<");
	(*token)->type = IR_OPERATOR;
	*token = (*token)->next;
}

void	change_delimiter_value(t_tokens **token, char *file_name)
{
	free((*token)->value);
	(*token)->value = file_name;
	*token = (*token)->next;
}

void	change_token_value(t_tokens **token, char *file_name)
{
	change_red_value(token);
	if (*token && (*token)->type == T_SPACE)
		*token = (*token)->next;
	if (*token && ((*token)->type == WORD || (*token)->type == VAR
			|| (*token)->type == QUOTES || (*token)->type == S_QUOTES))
		change_delimiter_value(token, file_name);
}

int	check_delimiter_type(t_tokens *token)
{
	token = token->next;
	if (token && token->type == T_SPACE)
		token = token->next;
	if (token && (token->type == QUOTES || token->type == S_QUOTES))
		return (1);
	return (0);
}
