/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:22:52 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/30 18:51:00 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_var_name(char *env)
{
	char	*env_name;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (env[i] && env[i] != '=')
		i++;
	env_name = malloc((i + 2) * sizeof(char));
	if (!env_name)
		return (NULL);
	env_name[0] = '$';
	i = 1;
	while (env[j] && env[j] != '=')
	{
		env_name[i] = env[j++];
		i++;
	}
	env_name[i] = '\0';
	return (env_name);
}

void	ft_fill_envp(t_env **envp, char *env[])
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_lstadd_back_envp(envp, ft_lstnew_envp(ft_strdup(env[i]), ft_get_var_name(env[i])));
		i++;
	}
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
			if (!check_start(token->value))
				unlink(token->value);
		}
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

void	free_linked_list(t_tokens **head, t_tokens *token)
{
	while (token)
	{
		free(token->value);
		ft_deleteNode(head, token);
		token = token->next;
	}
}

int main(int argc, char *argv[], char *env[])
{
	t_tokens	*token;
	t_env		*envp;
	t_cmds		*cmd;
	char		*input;

	(void)argc, (void)argv;
	token = NULL;
	envp = NULL;
	cmd = NULL;
	ft_fill_envp(&envp, env);
	while ((input = readline("\e[0;32m$> minishell \e[0m")) != NULL)
	{
		add_history(input);
		if (!ft_split_input(input, &token) && !ft_check_syntax(token))
		{
			ft_expand_vars(&token, envp);
			ft_cmd_table(token, &cmd, envp);
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
			// while (token)
			// {
			// 	printf("value: [%s]\n", token->value);
			// 	printf("type: [%d]\n", token->type);
			// 	printf("--------------------------\n");
			// 	token = token->next;
			// }
		}
		// system("leaks minishell");
		delete_tmp_files(token);
		close_open_fds(cmd);
		// free_linked_list(&token, token);
		token = NULL;
		cmd = NULL;
	}
}