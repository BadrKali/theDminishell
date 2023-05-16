/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:14:08 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/15 13:56:31 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
{
	if (sig == SIGINT && globale.cmd != 1 && globale.heredoc != 1)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
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
	globale.heredoc = -1;
	globale.cmd = -1;
	rl_catch_signals = 0;
	fill_env_pointer(&envp, env);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, signal_handler);
		input = readline("> minishell ");
		if (!input)
		{
			ft_putstr_fd("exit\n",2);
			exit(0);
		}
			
		if (!input_tokenizer(&token, envp, input) && !syntax_checker(token))
		{
			if (input[0])
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
			if (cmd && globale.heredoc != 1)
				exec_cmd(&cmd, &envp);
		}
		mini_cleaner(&token, &cmd, input);
		globale.heredoc = -1;
	}
}

//handle CTRl + D when minishell get run from another minishell
//make a signal handler that checks if you are in a nother minishell

//unset PWD whrn run env | grep OLDPWD dosent showed up 