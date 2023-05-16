/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 19:20:18 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/15 12:29:00 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

void	dup_fds(t_cmds *cmd, int **ends, int index, int cmd_num)
{
	close_fds(index, cmd_num, ends);
	if (cmd->prev == NULL)
		dup2 (ends[index][1], STDOUT_FILENO);
	else if (cmd->next != NULL)
	{
		dup2(ends[index][1], STDOUT_FILENO);
		close(ends[index][1]);
		dup2(ends[index - 1][0], STDIN_FILENO);
		close(ends[index - 1][0]);
	}
	else
	{
		dup2(ends[index - 1][0], STDIN_FILENO);
		close(ends[index - 1][0]);
	}
}

void	dup_check_slash(t_cmds *cmd, int **ends, int i, int cmd_num)
{
	dup_fds(cmd, ends, i, cmd_num);
	if (cmd->cmd[0] == '.' && cmd->cmd[1] == '/')
	{
		if (check_handler(cmd) == 1)
			exit(126);
	}
}

int	pipe_create(int **ends, int cmd_num)
{
	int	i;

	i = 0;
	while (i < cmd_num - 1)
	{
		if (pipe(ends[i]) == -1)
		{
			ft_putstr_fd("pipe failed\n", 2);
			return (1);
		}
		i ++;
	}
	return (0);
}

int	**pipe_alocate(int cmd_num)
{
	int	**end;
	int	i;

	i = 0;
	end = malloc((cmd_num - 1) * sizeof(int *));
	if (!end)
		return (NULL);
	end[cmd_num - 1] = NULL;
	while (i < cmd_num - 1)
	{
		end[i] = malloc(2 * sizeof(int));
		if (end[i] == NULL)
			return (NULL);
		i++;
	}
	return (end);
}

void	multi_command_exec(t_cmds *cmd, t_envp **env, int cmd_num)
{
	int	**ends;
	int	i;

	ends = pipe_alocate(cmd_num);
	if (ends == NULL || pipe_create(ends, cmd_num) == 1)
		return ;
	i = 0;
	while (cmd != NULL)
	{
		cmd->pid = fork();
		if (cmd->pid < 0)
			return (ft_putstr_fd("FORK FAILED\n", 2));
		if (cmd->pid == 0)
		{
			dup_check_slash(cmd, ends, i, cmd_num);
			single_cmd_handler(cmd, env);
		}
		i++;
		cmd = cmd->next;
	}
	close(ends[0][1]);
	close_fds(0, cmd_num, ends);
	free_2d_arrays_int(ends);
}
