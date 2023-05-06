/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:05:34 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/05 12:02:59 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_cmds	*ft_lstlast_cmd(t_cmds *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_cmd(t_cmds **lst, t_cmds *new)
{
	if (*lst == NULL)
		*lst = new;
	else
	{
		new->prev = ft_lstlast_cmd(*lst);
		ft_lstlast_cmd(*lst)->next = new;
	}
}

t_cmds	*ft_lstnew_cmd(char *command, char **args, int *stds)
{
	t_cmds	*cmd;

	cmd = (t_cmds *)malloc(sizeof(t_cmds));
	if (!cmd)
		return (NULL);
	cmd->cmd = command;
	cmd->args = args;
	cmd->std_in = stds[0];
	cmd->std_out = stds[1];
	cmd->next = NULL;
	cmd->prev = NULL;
	return (free(stds), cmd);
}