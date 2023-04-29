/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:24:43 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/29 16:37:32 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_get_delimiter(t_tokens *token)
{
	token = token->next;
	if (token->type == T_SPACE)
		token = token->next;
	return (token->value);
}

void	ft_change_red_value(t_tokens **token)
{
	free((*token)->value);
	(*token)->value = ft_strdup("<");
	(*token)->type = IR_OPERATOR;
	*token = (*token)->next;
}

void	ft_change_delimiter_value(t_tokens **token, char *file_name)
{
	free((*token)->value);
	(*token)->value = file_name;
	*token = (*token)->next;
}

void	ft_change_token_value(t_tokens **token, char *file_name)
{
	ft_change_red_value(token);
	if (*token && (*token)->type == T_SPACE)
		*token = (*token)->next;
	if (*token && (*token)->type == ARG)
		ft_change_delimiter_value(token, file_name);
}