/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:39 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/14 18:10:29 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

int	ft_env(t_envp *env)
{
	while (env != NULL)
	{
		if (env->flag == 0)
		{
			ft_putstr_fd(env->env_pre, 1);
			ft_putstr_fd(env->envp_value, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
	return (EXIT_SUCCESS);
}
