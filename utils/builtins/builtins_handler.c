/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:15 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/15 13:11:11 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

int	builtins_check(char *name)
{
	char	**builtins;
	int		i;

	if (name == NULL)
		return (1);
	builtins = ft_split("echo cd pwd export unset env exit", ' ');
	i = 0;
	while (builtins[i] != NULL)
	{
		if (ft_memcmp(name, builtins[i], ft_strlen(name)) == 0)
		{
			free_2d_arrays(builtins);
			return (0);
		}
		i++;
	}
	free_2d_arrays(builtins);
	return (1);
}

int	builtins_redirections(t_cmds *cmd)
{
	int	save;

	save = -1;
	if (cmd->std_in < 0)
	{
		ft_putstr_fd("Minishell: input file: No such file or directory\n", 2);
		return (save);
	}
	if (cmd->std_out != 1)
	{
		save = dup(STDOUT_FILENO);
		dup2(cmd->std_out, STDOUT_FILENO);
		close(cmd->std_out);
	}
	return (save);
}

int	builtins_function(t_cmds *cmd, t_envp **envp)
{
	int	result;

	result = 1;
	if (ft_memcmp(cmd->cmd, "echo", ft_strlen("echo")) == 0)
		result = ft_echo(cmd);
	else if (ft_memcmp(cmd->cmd, "cd", ft_strlen("cd")) == 0)
		result = ft_cd(cmd, envp);
	else if (ft_memcmp(cmd->cmd, "pwd", ft_strlen("pwd")) == 0)
		result = ft_pwd(envp);
	else if (ft_memcmp(cmd->cmd, "export", ft_strlen("export")) == 0)
		result = ft_export(cmd, envp);
	else if (ft_memcmp(cmd->cmd, "unset", ft_strlen("unset")) == 0)
		result = ft_unset(envp, cmd);
	else if (ft_memcmp(cmd->cmd, "env", ft_strlen("env")) == 0)
		result = ft_env(*envp);
	else if (ft_memcmp(cmd->cmd, "exit", ft_strlen("exit")) == 0)
		result = ft_exit(cmd);
	return (result);
}

int	builtins_handler(t_cmds *cmd, t_envp **envp)
{
	int	result;
	int	save;

	save = builtins_redirections(cmd);
	if (save == 2000)
		return (1);
	result = builtins_function(cmd, envp);
	if (cmd->std_out != 1)
	{
		dup2(save, STDOUT_FILENO);
		close(save);
	}
	return (result);
}
