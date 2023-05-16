/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:19:08 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/15 13:07:06 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

void	close_fds(int index, int cmd_num, int **ends)
{
	int	i;

	i = 0;
	while (i < cmd_num - 1)
	{
		if (i != index)
			close(ends[i][1]);
		if (i != index - 1)
			close(ends[i][0]);
		i ++;
	}
}

void	multi_command(t_cmds **cmd, t_envp **envp)
{
	int		cmd_num;
	int		i;
	int		status;
	t_cmds	*tmp;
	int		signum;

	tmp = *cmd;
	i = 0;
	cmd_num = len_liste(*cmd);
	multi_command_exec(*cmd, envp, cmd_num);
	while (i < cmd_num)
	{
		waitpid(tmp->pid, &status, 0);
		i++;
		tmp = tmp->next;
	}
	if (WIFEXITED(status))
		globale.exit_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		signum = WTERMSIG(status);
		globale.exit_code = 130;
		signal_handler_exec(signum);
	}
}

void	exec_cmd(t_cmds **cmd, t_envp **envp)
{
	t_cmds	*tmp;
	int		i;
	t_envp	*tmp_env;

	tmp = *cmd;
	i = 0;
	globale.cmd = 1;
	if (tmp->cmd && ft_memcmp("./minishell", tmp->cmd,
			ft_strlen("./minishell")) == 0)
	{
		tmp_env = check_env_exist("SHLVL",*envp);
		tmp_env->envp_value = ft_itoa(ft_atoi(tmp_env->envp_value) + 1);
	}
	if (len_liste(*cmd) == 1)
	{
		if (builtins_check(tmp->cmd) == 0)
			globale.exit_code = builtins_handler(tmp, envp);
		else
			simple_exec_handler(tmp, envp);
	}
	else
		multi_command(cmd, envp);
	globale.cmd = -1;
}
