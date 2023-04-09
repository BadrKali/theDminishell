/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:22:52 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/09 21:53:43 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_get_var(t_env *envp, char *value)
{
	while (envp)
	{
		printf("envp: %s -- token: %s\n", envp->name, value);
		if (envp->name == value)
			return (envp);
		envp = envp->next;
	}
	return (NULL);
}

void	ft_expand_vars(t_tokens **token, t_env *envp)
{
	t_env		*tmp;
	t_tokens	*tmp2;

	tmp2 = *token;
	while (*token)
	{
		if ((*token)->type == VAR)
		{
			tmp = ft_get_var(envp, (*token)->value);
			(*token)->value = tmp->env_name;
		}
		*token = (*token)->next;
	}
	*token = tmp2;
}

char	*ft_get_var_name(char *env)
{
	char	*env_name;
	int		i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	if (env[i] && env[i] == '=')
		i++;
	env_name = malloc((i + 1) * sizeof(char));
	if (!env_name)
		return (NULL);
	env_name[0] = '$';
	i = 1;
	while (env[i] && env[i] != '=')
	{
		env_name[i] = env[i];
		i++;
	}
	if (env[i] && env[i] == '=')
		env_name[i] = env[i];
	env_name[++i] = '\0';
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
	char		*input;

	(void)argc, (void)argv;
	token = NULL;
	envp = NULL;
	ft_fill_envp(&envp, env);
	while (envp)
	{
		printf("%s\n", envp->env_name);
		envp = envp->next;
	}
	while ((input = readline("\e[0;32m$> minishell \e[0m")) != NULL)
	{
		add_history(input);
		ft_split_input(input, &token);
		if (!ft_check_syntax(token))
		{
			// ft_expand_vars(&token, envp);
			while (token)
			{
				printf("value: [%s]\n", token->value);
				printf("type: [%d]\n", token->type);
				printf("--------------------------\n");
				token = token->next;
			}
		}
		token = NULL;
	}
}