/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:14:08 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/06 19:24:43 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_2d_arrays(char **res)
{
	int	j;

	j = 0;
	while (res[j] != NULL)
		j++;
	while (j--)
		free(res[j]);
	free(res);
	return (NULL);
}

int	check_start(char *val)
{
	char	*str;
	int		i;

	i = 0;
	str = ft_strdup("/tmp/");
	while (i < 5)
	{
		if (str[i] != val[i])
			return (1);
		i++;
	}
	return (0);
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

int	main(int argc, char *argv[], char *env[])
{
	t_tokens	*token;
	t_envp		*envp;
	t_cmds		*cmd;
	char		*input;

	if (argc != 1)
		return (1);
	(void)argv;
	token = NULL;
	envp = NULL;
	cmd = NULL;
	fill_env_pointer(&envp, env);
	while ((input = readline("> minishell ")) != NULL)
	{
		add_history(input);
		if (!input_tokenizer(&token, envp, input) && !syntax_checker(token))
		{
			expand_variables(&token, envp);
			command_table(token, &cmd, envp);
			// -------------------
			// t_tokens *tmp = token;
			// while (tmp)
			// {
			// 	printf("value: [%s]\n", tmp->value);
			// 	printf("type: [%d]\n", tmp->type);
			// 	printf("is_joined: [%d]\n", tmp->is_joined);
			// 	printf("--------------------------\n");
			// 	tmp = tmp->next;
			// }
			// -------------------
			t_cmds *tmp = cmd;
			while (tmp)
			{
				printf("cmd: [%s]\n", tmp->cmd);
				int	i = 0;
				while (tmp->args[i])
				{
					printf("args[%d]: [%s]\n", i, tmp->args[i]);
					i++;
				}
				printf("std_in: %d\n", tmp->std_in);
				printf("std_out: %d\n", tmp->std_out);
				tmp = tmp->next;
			}
			// -------------------
		}
		delete_tmp_files(token);
		close_open_fds(cmd);
		token = NULL;
		cmd = NULL;
	}
}