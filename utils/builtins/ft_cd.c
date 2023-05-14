/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:27 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/14 12:16:36 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"../../minishell.h"

char *get_env_value(t_envp *env,char *name)
{
    t_envp *tmp;

    tmp = env;
    while(tmp != NULL)
    {
        if(ft_memcmp(name,tmp->envp_name,ft_strlen(name)) == 0)
            return(tmp->envp_value);
        tmp = tmp->next;
    }
    return(NULL);
}

int update_env(char *name,char *value,t_envp *env)
{
    if(value == NULL)
        return(EXIT_FAILURE);
    while(env != NULL)
    {
        if(ft_memcmp(name,env->envp_name,ft_strlen(name)) == 0)
        {
            free(env->envp_value);
            env->envp_value = ft_strdup(value);
            return(EXIT_SUCCESS);
        }
        env = env->next;
    }
    return(EXIT_FAILURE);
}

int ft_cd_home(t_envp **env,t_cmds *cmd)
{
    char *home;
    t_envp *tmp;
    
    if(cmd->args[1] == NULL)
        home = get_env_value(*env,"$HOME");
    else if(cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
        home = ft_strdup(get_env_value(*env,"$OLDPWD"));
    if(chdir(home) == -1)
    {
        ft_putstr_fd("Minishell: cd: HOME not set\n",2);
        return(EXIT_FAILURE);
    }
    update_env("$OLDPWD",get_env_value(*env,"$PWD"),*env);
    update_env("$PWD",home,*env);
    return(EXIT_SUCCESS);
}


int ft_cd(t_cmds *cmd,t_envp **env)
{
    char *buf;
    int t;

    buf = NULL;
    if(cmd->args[1] == NULL)
        return(ft_cd_home(env,cmd));
    if(cmd->args[1][0] == '-' && cmd->args[1][1] == '\0')
        return(ft_cd_home(env,cmd));
    else if(cmd->args[1] != NULL)
    {
        if(chdir(cmd->args[1]) == -1)
        {
            ft_putstr_fd("Minishell: cd: ",2);
            ft_putstr_fd(cmd->args[1],2);
            ft_putstr_fd(": No such file or directory\n",2);
            return(EXIT_FAILURE);
        }
        buf = getcwd(buf,10004);
        update_env("$OLDPWD",get_env_value(*env,"$PWD"),*env);
        update_env("$PWD",buf,*env);
        free(buf);
    }
    return(EXIT_SUCCESS);
}

// i added (cmd->args[1][0] == '-' && cmd->args[1][1] == '\0') to ft_cd
// teste env | grep PWD after using cd -
//it segfault when i run cd only
//recheck the updated OLDPWD AND PWD IN ALL CASES


//pwd + cd utils => double free