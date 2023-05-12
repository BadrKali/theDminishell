/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_table_utils_two.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:18:08 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 14:19:08 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*merge_args(t_tokens **token)
{
	char	*arg;

	arg = ft_strdup((*token)->value);
	*token = (*token)->next;
	while (*token && (*token)->type != OR_OPERATOR
		&& (*token)->type != IR_OPERATOR && (*token)->type != HEREDOC_OPERATOR
		&& (*token)->type != APPEND_OPERATOR && (*token)->type != PIPE
		&& (*token)->type != T_SPACE)
	{
		arg = ft_strjoin(arg, (*token)->value);
		*token = (*token)->next;
	}
	return (arg);
}

int	get_arguments_len(t_tokens *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		skip_redirections(&token);
		if (token && (token->type == WORD || token->type == QUOTES
				|| token->type == S_QUOTES || token->type == VAR))
		{
			token = token->next;
			count++;
		}
		else if (token && token->type != PIPE)
			token = token->next;
	}
	return (count);
}
