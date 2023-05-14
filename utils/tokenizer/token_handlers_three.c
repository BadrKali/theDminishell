/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers_three.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:37:04 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/13 12:16:38 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_previous(t_tokens *token)
{
	t_tokens	*last;

	last = ft_lstlast_token(token);
	if (last && last->type == T_SPACE)
		last = last->prev;
	if (last && last->type == HEREDOC_OPERATOR)
		return (1);
	return (0);
}

void	handle_quotes_two(t_tokens **token, t_envp *envp, char *tok)
{
	t_tokens	*last_token;
	char		*res;

	res = ft_substr(tok, 1, ft_strlen(tok) - 2);
	ft_lstadd_back_token(token, ft_lstnew_token(res, QUOTES));
	last_token = ft_lstlast_token(*token);
	handle_variables_in_quotes(&last_token, envp);
}

int	find_space(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

void	handle_exit_variable(t_tokens **token, int *end)
{
	ft_lstadd_back_token(token, ft_lstnew_token(ft_itoa(globale.exit_code),
			VAR));
	(*end)++;
}
