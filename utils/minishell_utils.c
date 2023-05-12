/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:10:12 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 13:58:10 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_start(char *val)
{
	char	*str;
	int		i;

	i = 0;
	str = ft_strdup("/tmp/");
	while (i < 5)
	{
		if (str[i] != val[i])
			return (free(str), 1);
		i++;
	}
	return (free(str), 0);
}

void	delete_tmp_files(t_tokens *token)
{
	while (token)
	{
		if (token->type == IR_OPERATOR)
		{
			token = token->next;
			if (token && token->type == T_SPACE)
				token = token->next;
			if (token && !check_start(token->value))
				unlink(token->value);
		}
		if (token)
			token = token->next;
	}
}

void	close_open_fds(t_cmds *cmd)
{
	while (cmd)
	{
		if (cmd->std_in != 0 && cmd->std_in != -1)
			close(cmd->std_in);
		if (cmd->std_out != 1 && cmd->std_out != -1)
			close(cmd->std_out);
		cmd = cmd->next;
	}
}

void	mini_cleaner(t_tokens **token, t_cmds **cmd, char *input)
{
	delete_tmp_files(*token);
	close_open_fds(*cmd);
	free_linked_list_cmd(*cmd);
	free_linked_list(*token);
	*cmd = NULL;
	*token = NULL;
	free(input);
}

int	check_is_joined(t_tokens *token)
{
	token = token->next;
	if (token->type == T_SPACE)
		token = token->next;
	if (token->is_joined != 1)
		return (1);
	return (0);
}
