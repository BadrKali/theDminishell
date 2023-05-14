/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:53 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/14 13:42:30 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"../../minishell.h"
void create_new_env(t_envp **envp,char *arg)
{
    t_envp *new;

    if(ft_strchr(arg,'=') == 0)
    {
        new = ft_lstnew_envp(NULL,get_variable_name(arg));
        new->env_pre = ft_strdup(arg);
        new->envp_value = NULL;
        new->flag = 1;
        ft_lstadd_back_envp(envp,new);
        return;
    }
    new = ft_lstnew_envp(remove_name(arg),get_variable_name(arg));
    new->env_pre = envp_name(new);
    new->flag = 0;
    ft_lstadd_back_envp(envp,new);
}

void append_env(char *arg,t_envp **env)
{
    t_envp *tmp;
    char *set_arg;
    
    tmp = check_env_exist(arg,*env);
    if(tmp == NULL)
        create_new_env(env,arg);
    if(ft_strchr(tmp->env_pre,'=') == 0)
    {
        tmp->env_pre = ft_strjoin(tmp->env_pre,"=");
        tmp->envp_value = ft_strjoin(tmp->envp_value,get_str(arg,'='));
        if(tmp->envp_value == NULL)
            tmp->envp_value = ft_strdup(get_str(arg,'='));
        return;
    }
    else if(tmp != NULL)
        tmp->envp_value = ft_strjoin(tmp->envp_value,get_str(arg,'='));
}

void update_env_value(t_envp **env,char *arg)
{
    t_envp *tmp;
    char *value;

    if(ft_strchr(arg,'=') == 0)
        return;
    tmp = check_env_exist(arg,*env);
    tmp->flag = 0;
    if(ft_strchr(tmp->env_pre,'=') == 0)
    {
        tmp->env_pre = ft_strjoin(tmp->env_pre,"=");
        tmp->envp_value = ft_strdup(get_str(arg,'='));
        return;
    }
    free(tmp->envp_value);
    tmp->envp_value = ft_strdup(get_str(arg,'='));
}


void export_handler(char *arg,t_envp **env)
{
    int type;

    type = ft_check_type(arg,*env);
    if(type == NEW_ENV)
        create_new_env(env,arg);
    if(type == ENV_UPDATE)
        update_env_value(env,arg);
    if(type == ENV_APPEND)
        append_env(arg,env);
}

int ft_export(t_cmds *cmd,t_envp **env)
{
    int i;
    int exit;

    i = 1;
    exit = 0;
    if(cmd->args[1] == NULL)
        return(export_print(env));
    while(cmd->args[i] != NULL)
    {
        if(syntax_check(cmd->args[i]) > 1)
            export_handler(cmd->args[i],env);
        else
        {
            ft_putstr_fd("Minishell: export: '",2);
            ft_putstr_fd(cmd->args[i],2);
            ft_putstr_fd("' : not a valid identifier\n",2);
            exit = 1;
        }
        i++;
    }
    return(exit);
}




// export BADR => export BADR+=hello