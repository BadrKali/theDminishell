/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:43:02 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/06 16:16:17 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void	insertAfter(t_tokens *prevNode, char *arg, int type) {
//     if (prevNode == NULL)
// 	{
// 		printf("Error: previous node cannot be NULL\n");
//         return ;
// 	}
// 	t_tokens *newNode = ft_lstnew_token(arg, type);
//     newNode->prev = prevNode;
//     newNode->next = prevNode->next;
//     if (prevNode->next != NULL)
//         prevNode->next->prev = newNode;
//     prevNode->next = newNode;
// }

static void	merge_tokens(t_tokens **token)
{
	t_tokens	*tmp;
	t_tokens	*tmp2;

	tmp = *token;
	tmp2 = *token;
	while (tmp)
	{
		if (tmp->type == WORD || tmp->type == QUOTES || tmp->type == S_QUOTES
			|| tmp->type == VAR)
		{
			tmp2 = tmp;
			tmp = tmp->next;
			while (tmp && tmp->type != T_SPACE && (tmp->type == QUOTES
				|| tmp->type == S_QUOTES || tmp->type == VAR
				|| tmp->type == WORD))
			{
				tmp2->value = ft_strjoin(tmp2->value, tmp->value);
				if (tmp && (tmp->type == QUOTES || tmp->type == S_QUOTES))
					tmp2->is_joined = 1;
				ft_deleteNode(token, tmp);
				tmp = tmp->next;
			}
		}
		else
			tmp = tmp->next;
	}
}

int	input_tokenizer(t_tokens **token, t_envp *envp, char *input)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			error = handle_quotes(token, envp, input, input[i], &i);
		else if (input[i] == ' ' || input[i] == '\n' || input[i] == '\r'
			|| input[i] == '\f' || input[i] == '\v' || input[i] == '\t')
			handle_space(token, input, &i);
		else if (input[i + 1] && ((input[i] == '<' && input[i + 1] == '<')
			|| (input[i] == '>' && input[i + 1] == '>')))
			handle_redirections(token, input, &i, 1);
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
			handle_redirections(token, input, &i, 2);
		else if (input[i] == '$')
			handle_variable(token, envp, input, &i);
		else
			handle_string(token, input, &i);
		if (error)
			return (1);
	}
	merge_tokens(token);
	return (0);
}