/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 14:12:31 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 14:19:11 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	skip_redirections(t_tokens **token)
{
	while (*token && ((*token)->type == OR_OPERATOR
			|| (*token)->type == IR_OPERATOR
			|| (*token)->type == HEREDOC_OPERATOR
			|| (*token)->type == APPEND_OPERATOR))
	{
		*token = (*token)->next;
		if (*token && (*token)->type == T_SPACE)
			*token = (*token)->next;
		if (*token)
			*token = (*token)->next;
		if (*token && (*token)->type == T_SPACE)
			*token = (*token)->next;
	}
}

static int	is_there_space(char *val)
{
	int	i;

	i = 0;
	while (val[i])
	{
		if (val[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int	check_variable_syntax(char *val, int type)
{
	if (type == VAR)
		if (is_there_space(val))
			return (1);
	return (0);
}

int	is_there_same_type_next(t_tokens *token, int type)
{
	while (token && token->type != PIPE)
	{
		if ((type == 1 && (token->type == OR_OPERATOR
					|| token->type == APPEND_OPERATOR))
			|| (type == 2 && token->type == IR_OPERATOR))
			return (1);
		token = token->next;
	}
	return (0);
}

int	*allocate_for_stds(int std_in, int std_out)
{
	int	*stds;

	stds = malloc(2 * sizeof(int));
	stds[0] = std_in;
	stds[1] = std_out;
	return (stds);
}
