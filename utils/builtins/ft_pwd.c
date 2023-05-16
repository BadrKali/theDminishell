/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:58 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/15 12:15:16 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"

int	ft_pwd(t_envp **env)
{
	char	*pwd;

	pwd = get_env_value(*env, "$PWD");
	if (pwd == NULL)
	{
		pwd = getcwd(pwd, 10008);
		ft_putstr_fd(pwd, 1);
		free(pwd);
		return (EXIT_SUCCESS);
	}
	ft_putstr_fd(pwd, 1);
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
