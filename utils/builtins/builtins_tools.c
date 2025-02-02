/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:21 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/15 12:20:18 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

int	export_print(t_envp **env)
{
	t_envp	*tmp;

	tmp = *env;
	while (tmp)
	{
		if ((tmp->flag == 1 || tmp->flag == 0))
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(tmp->env_pre, 1);
			if (tmp->envp_value != NULL)
				ft_putstr_fd("\"", 1);
			ft_putstr_fd(tmp->envp_value, 1);
			if (tmp->envp_value != NULL)
				ft_putstr_fd("\"", 1);
			write(1, "\n", 1);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

int	ft_check_type(char *arg, t_envp *env)
{
	int		eq;
	int		flag;
	t_envp	*env_name;

	flag = 0;
	eq = ft_strchr(arg, '=');
	env_name = check_env_exist(arg, env);
	if (arg[eq - 1] == '+')
		return (1000);
	if (env_name != NULL)
		return (2000);
	else
		return (3000);
}

char	*get_str(const char *str, int c)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	while (i <= len)
	{
		if (str[i] == (char )c)
			return ((char *)&str[i + 1]);
		i ++;
	}
	return (NULL);
}

int	syntax_check(char *str)
{
	int	i;
	int	eq;

	i = 0;
	eq = 0;
	if (ft_isalpha(str[0]) == 0 && str[0] != '_')
		return (1);
	eq = ft_strchr(str, '=');
	if (eq == 0)
		eq = ft_strlen(str);
	if (eq != 0)
		if (str[eq - 1] == '+')
			eq = eq - 1;
	while (i < eq)
	{
		if (ft_isalpha(str[i]) == 0 && ft_isdigit(str[i]) == 0 && str[i] != '_')
			return (1);
		i++;
	}
	return (2);
}

t_envp	*check_env_exist(char *args, t_envp *env)
{
	char	*arg_name;

	arg_name = get_variable_name(args);
	while (env != NULL)
	{
		if (ft_memcmp(arg_name, env->envp_name, ft_strlen(arg_name)) == 0)
		{
			free(arg_name);
			return (env);
		}
		env = env->next;
	}
	free(arg_name);
	return (NULL);
}
