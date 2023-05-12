/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:58 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/11 18:03:00 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../minishell.h"


int ft_pwd(t_cmds *cmd,t_envp **env)
{
    char *pwd;

    pwd = get_env_value(*env,"$PWD");
    if(pwd == NULL)
        pwd = getcwd(pwd,100008);
    print_string(pwd);
    write(1,"\n",1);
    return(EXIT_SUCCESS);
}