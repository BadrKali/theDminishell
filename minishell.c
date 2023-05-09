/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:14:08 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 16:48:45 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *env[])
{
	t_tokens	*token;
	t_envp		*envp;
	t_cmds		*cmd;
	char		*input;

	(void)argv;
	if (argc != 1)
		return (1);
	token = NULL;
	envp = NULL;
	cmd = NULL;
	fill_env_pointer(&envp, env);
	input = readline("> minishell ");
	while (input != NULL)
	{
		add_history(input);
		if (!input_tokenizer(&token, envp, input) && !syntax_checker(token))
		{
			delete_two_exec_spaces(&token);
			command_table(token, &cmd, envp);
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
		}
		mini_cleaner(&token, &cmd, input);
		input = readline("> minishell ");
	}
}
