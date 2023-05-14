/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:45 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/14 13:25:58 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"../../minishell.h"

int check_numeric(char *str)
{
    int i;

    i = 0;
    while(str[i] != '\0')
    {
        if(ft_isdigit(str[i]) == 0)
            return(1);
        i++;
    }
    return(0);
}

int ft_exit(t_cmds *cmd,t_envp **env)
{
    int i;

    if(cmd->args[1] == NULL)
    {
        ft_putstr_fd("exit\n",2);
        globale.exit_code = 1;
        exit(globale.exit_code);
    }
    else if(cmd->args[2] != NULL)
    {
        ft_putstr_fd("Minishell: exit: too many arguments\n",2);
        return(1);
    }
    else if(check_numeric(cmd->args[1]) != 0)
    {
        ft_putstr_fd("Minishell: exit: asdasd: numeric argument required\n",2);
        ft_putstr_fd("exit",2);
        //free_resources();
        exit(255);
    }
    else
        return(ft_atoi(cmd->args[1]));
        
}