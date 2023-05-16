/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks_removers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:09:08 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 15:33:23 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_2d_arrays_int(int **res)
{
	int	i;

	i = 0;
	while (res[i])
		free(res[i++]);
	free(res);
}

void	*free_2d_arrays(char **res)
{
	int	i;

	i = 0;
	while (res[i])
		free(res[i++]);
	free(res);
	return (NULL);
}

void	free_linked_list(t_tokens *token)
{
	t_tokens	*tmp;

	while (token)
	{
		free(token->value);
		tmp = token->next;
		free(token);
		token = tmp;
	}
	free(token);
}

void	free_linked_list_cmd(t_cmds *cmd)
{
	t_cmds	*tmp;

	while (cmd)
	{
		free_2d_arrays(cmd->args);
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
	free(cmd);
}

void	delete_two_exec_spaces(t_tokens **token)
{
	t_tokens	*tmp;

	tmp = *token;
	while (tmp)
	{
		if (tmp && tmp->type == T_SPACE && (tmp->prev == NULL
				|| (tmp->next && tmp->next->type == T_SPACE)
				|| tmp->next == NULL))
			ft_deletenode(token, tmp);
		tmp = tmp->next;
	}
}
