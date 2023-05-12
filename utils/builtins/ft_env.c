/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:39 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/11 18:02:40 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"../../minishell.h"


void print_envp(t_envp *env)
{
	while(env != NULL)
	{
		if(env->flag == 0)
			printf("%s%s\n",env->env_pre,env->envp_value);
		env = env->next;
	}
}

int ft_env(t_envp *env)
{
    print_envp(env);
    return(EXIT_SUCCESS);
}