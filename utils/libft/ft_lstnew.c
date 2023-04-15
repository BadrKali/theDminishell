/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 18:02:16 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/15 18:08:09 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_deleteNode(t_tokens **head_ref, t_tokens *del)
{
    if (*head_ref == NULL || del == NULL)
        return;
    if (*head_ref == del)
        *head_ref = del->next;
    if (del->next != NULL)
        del->next->prev = del->prev;
    if (del->prev != NULL)
        del->prev->next = del->next;
    free(del);
    return;
}

t_tokens	*ft_lstnew_token(char *value, int type)
{
	t_tokens	*token;

	token = (t_tokens *)malloc(sizeof(t_tokens));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

t_env	*ft_lstnew_envp(char *value, char *name)
{
	t_env	*envp;

	envp = (t_env *)malloc(sizeof(t_env));
	if (!envp)
		return (NULL);
	envp->env_name = value;
	envp->name = name;
	envp->next = NULL;
	envp->prev = NULL;
	return (envp);
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
	return (cmd);
}