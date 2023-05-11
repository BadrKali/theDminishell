/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:14:08 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/11 15:53:44 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//! Put back flags later

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		//! exit status should be 130
	}
}

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
	rl_catch_signals = 0;
	fill_env_pointer(&envp, env);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("> minishell ");
		if (!input)
			exit(0);
		if (!input_tokenizer(&token, envp, input) && !syntax_checker(token))
		{
			add_history(input);
			delete_two_exec_spaces(&token);
			command_table(token, &cmd, envp);
			// t_cmds *tmp = cmd;
			// while (tmp)
			// {
			// 	printf("cmd: [%s]\n", tmp->cmd);
			// 	int	i = 0;
			// 	while (tmp->args[i])
			// 	{
			// 		printf("args[%d]: [%s]\n", i, tmp->args[i]);
			// 		i++;
			// 	}
			// 	printf("std_in: %d\n", tmp->std_in);
			// 	printf("std_out: %d\n", tmp->std_out);
			// 	tmp = tmp->next;
			// }
			if (cmd)
				exec_cmd(&cmd, &envp);
			mini_cleaner(&token, &cmd, input);
		}
	}
}
