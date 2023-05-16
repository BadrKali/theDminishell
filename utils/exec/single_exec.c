/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 10:24:08 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/15 12:33:12 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

int	dup_redirections(t_cmds *cmd)
{
	if (cmd->std_in < 0)
	{
		ft_putstr_fd("Minishell: input file not found\n", 2);
		return (EXIT_FAILURE);
	}
	if (cmd->std_out < 0)
	{
		ft_putstr_fd("Minishell: output file not found\n", 2);
		return (EXIT_FAILURE);
	}
	if (cmd->std_in != 0 || cmd->std_out != 1)
	{
		if (dup2(cmd->std_in, 0) < 0)
		{
			ft_putstr_fd("Minishell: dup2 failed\n", 2);
			return (EXIT_FAILURE);
		}
		if (dup2(cmd->std_out, 1) < 0)
		{
			ft_putstr_fd("Minishell: dup2 failed\n", 2);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

void	single_cmd_handler(t_cmds *cmd, t_envp **env)
{
	char	**tmp_env;

	tmp_env = env_joiner(*env);
	if (tmp_env == NULL)
		exit(1);
	if (dup_redirections(cmd) == 1)
	{
		free_2d_arrays(tmp_env);
		exit(EXIT_FAILURE);
	}
	if (builtins_check(cmd->cmd) == 0)
		exit(builtins_handler(cmd, env));
	signal_exec ();
	if (execve(cmd->args[0], cmd->args, tmp_env) < 0)
	{
		ft_putstr_fd("Minishell: ", 2),
		ft_putstr_fd(cmd->cmd, 2),
		ft_putstr_fd(": command not found\n", 2),
		exit(127);
	}
}

int	check_handler(t_cmds *cmd)
{
	DIR	*dir;

	dir = opendir(cmd->cmd);
	if (dir)
	{
		closedir(dir);
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		globale.exit_code = 126;
		return (1);
	}
	else
	{
		if (access(cmd->cmd, X_OK) != 0)
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(cmd->cmd, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			globale.exit_code = 126;
			return (1);
		}
	}
	return (0);
}

void	single_exec_wait(t_cmds *cmd)
{
	int	status;
	int	signum;

	waitpid(cmd->pid, &status, 0);
	if (WIFEXITED(status))
		globale.exit_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		signum = WTERMSIG(status);
		globale.exit_code = 130;
		signal_handler_exec(signum);
	}
}

void	simple_exec_handler(t_cmds *cmd, t_envp **env)
{
	if (cmd->cmd == NULL)
		return ;
	if (cmd->cmd[0] == '.' && cmd->cmd[1] == '/')
	{
		if (check_handler(cmd) == 1)
			return ;
	}
	cmd->pid = fork();
	if (cmd->pid < 0)
	{
		ft_putstr_fd("FORK FAILED\n", 2);
		return ;
	}
	if (cmd->pid == 0)
		single_cmd_handler(cmd, env);
	if (cmd->pid > 0)
		single_exec_wait(cmd);
}
