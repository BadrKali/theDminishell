#include"../../minishell.h"

int    ft_memcmp(const void *str1, const void *str2, size_t n)
{
        size_t                  i;
        unsigned char   *s1;
        unsigned char   *s2;

        s1 = (unsigned char *)str1;
        s2 = (unsigned char *)str2;
        i = 0;
        while (i <= n)
        {
                if (s1[i] != s2[i])
                    return(1);
                i ++;
        }
        return (0);
}


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

int ft_cd_home(t_envp **env)
{
    char *home;
    t_envp *tmp;

    home = get_env_value(*env,"$HOME");
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
        return(ft_cd_home(env));
    if(cmd->args[2] != NULL)
    {
        ft_putstr_fd("Minishell: cd: ",2);
        ft_putstr_fd(": too many arguments\n",2);
        return(EXIT_FAILURE);
    }
    else if(cmd->args[1] != NULL)
    {
        if(chdir(cmd->args[1]) == -1)
        {
            ft_putstr_fd("Minishell: cd: ",2);
            ft_putstr_fd(cmd->args[1],2);
            ft_putstr_fd(": No such file or directory\n",2);
            return(EXIT_FAILURE);
        }
        update_env("$OLDPWD",get_env_value(*env,"$PWD"),*env);
        update_env("$PWD",getcwd(buf,10004),*env);
    }
    return(EXIT_SUCCESS);
}