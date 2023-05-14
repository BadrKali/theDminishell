#include"../../minishell.h"

int len_liste(t_cmds *cmd)
{
    int len;

    len = 0;
    while(cmd != NULL)
    {
        cmd = cmd->next;
        len++; 
    }
    return(len);
}

int env_len(t_envp *env)
{
    int len;

    len = 0;
    while(env != NULL)
    {
        env = env->next;
        len ++;
    }
    return(len);
}

char **env_joiner(t_envp *env)
{
    char **str;
    int len;
    int i;

    i = 0;
    len = env_len(env);
    str = malloc(sizeof(char *) * (len + 1));
    if(str == NULL)
        return(NULL);
    str[len] = 0;
    while(env != NULL)
    {
        str[i] = ft_strjoin(env->env_pre,env->envp_value);
		env = env->next;
        i++;
    }
   //str[i] = NULL;
    return(str);
}

void signal_exec()
{
    signal(SIGINT,SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
}

void signal_handler_exec(int num)
{
    if(num == SIGINT)
        printf("\n");
	//rl_on_new_line();
    //globale.cmd = -1;
}