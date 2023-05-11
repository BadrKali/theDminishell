#include"../../minishell.h"
int ft_strchr(const char *str, int c)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	while (i <= len)
	{
		if (str[i] == (char )c)
			return (i);
		i ++;
	}
	return (0);
}

int export_print(t_envp **env)
{
    char *str;
    t_envp *tmp;

    tmp = *env;
    while(tmp)
    {
        ft_putstr_fd("declare -x ",1);
        ft_putstr_fd(tmp->env_pre,1);
        ft_putstr_fd(tmp->envp_value,1);
        write(1,"\n",1);
        tmp = tmp->next;
    }
    return(EXIT_SUCCESS);
}

int ft_check_type(char *arg,t_envp *env)
{
    int eq;
    int flag;
    char *env_name;

    flag = 0;
    eq = ft_strchr(arg,'=');
    env_name = get_env_value(env,get_variable_name(arg));
    if(arg[eq - 1] == '+')
        return(1000);
    if(env_name != NULL)
    {
        free(env_name);
        return(2000);
    }
    else
        return(3000);
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
			return ((char *)&str[i+1]);
		i ++;
	}
	return (NULL);
}

void create_new_env(t_envp **envp,char *arg)
{
    t_envp *new;

    if(ft_strchr(arg,'=') == 0)
        return;
    new = ft_lstnew_envp(remove_name(arg),get_variable_name(arg));
    new->env_pre = envp_name(new);
    ft_lstadd_back_envp(envp,new);
}


int syntax_check(char *str)
{
    int i;
    int eq;

    i = 0;
    eq = 0;
    if(ft_isalpha(str[0]) == 0 && str[0] != '_')
        return(1);
    eq = ft_strchr(str,'=');
    if(eq == 0)
        eq = ft_strlen(str);
    if(eq != 0)
        if(str[eq - 1] == '+')
            eq = eq - 1;
    while(i < eq)
    {
        if(ft_isalpha(str[i]) == 0 && ft_isdigit(str[i]) == 0 && str[i] != '_')
            return(1);
        i++;
    }
    return(2);
}

t_envp *check_env_exist(char *args,t_envp *env)
{
    char *arg_name;

    arg_name = get_variable_name(args);
    while(env != NULL)
    {
        if(ft_memcmp(arg_name,env->envp_name,ft_strlen(arg_name)) == 0)
            return(env);
            
        env = env->next;
    }
    return(NULL);
    //memcmp i <= n check it 
}

void append_env(char *arg,t_envp **env)
{
    t_envp *tmp;
    char *set_arg;

    tmp = check_env_exist(arg,*env);
    if(set_arg == NULL)
        create_new_env(env,arg);
    else if(set_arg != NULL)
        tmp->envp_value = ft_strjoin(tmp->envp_value,get_str(arg,'='));
}

void update_env_value(t_envp **env,char *arg)
{
    t_envp *tmp;
    char *value;

    tmp = check_env_exist(arg,*env);
    free(tmp->envp_value);
    tmp->envp_value = ft_strdup(get_str(arg,'='));
}

int ft_ex_error(char *str)
{
    ft_putstr_fd("Minishell: export: '",2);
    ft_putstr_fd(str,2);
    ft_putstr_fd("' : not a valid identifier\n",2);
    return(1);
}

int ft_export(t_cmds *cmd,t_envp **env)
{
    int i;
    int type;
    t_envp *tmp;

    i = 1;
    type = 0;
    if(cmd->args[1] == NULL)
        return(export_print(env));
    while(cmd->args[i] != NULL)
    {
        if(syntax_check(cmd->args[i]) > 1)
        {
            type = ft_check_type(cmd->args[i],*env);
            if(type ==  NEW_ENV)
                create_new_env(env,cmd->args[i]);
            else if(type == ENV_APPEND)
                append_env(cmd->args[i],env);
            else if(type == ENV_UPDATE)
               update_env_value(env,cmd->args[i]);
        }
        else
            return(ft_ex_error(cmd->args[i]));
        i++;
    }
    return(EXIT_SUCCESS);
}

//need to parse the after = 