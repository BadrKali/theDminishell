/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:22:52 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/17 16:37:08 by abahsine         ###   ########.fr       */
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
		ft_split_input(input, &token);
		if (!ft_check_syntax(token))
		{
			ft_expand_vars(&token, envp);
			ft_cmd_table(token, &cmd);
			while (cmd)
			{
				printf("cmd: [%s]\n", cmd->cmd);
				int	i = 0;
				while (cmd->args[i])
				{
					printf("args[%d]: [%s]\n", i, cmd->args[i]);
					i++;
				}
				printf("std_in: %d\n", cmd->std_in);
				printf("std_out: %d\n", cmd->std_out);
				cmd = cmd->next;
			}
			// while (token)
			// {
			// 	printf("value: [%s]\n", token->value);
			// 	printf("type: [%d]\n", token->type);
			// 	printf("--------------------------\n");
			// 	token = token->next;
			// }
		}
		token = NULL;
	}
}