#include"../../minishell.h"

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
    str = malloc(sizeof(char *) * len + 1);
    if(str == NULL)
        return(NULL);
    while(env != NULL)
    {
        str[i] = ft_strjoin(env->env_pre,env->envp_value);
		env = env->next;
        i++;
    }
    str[i] = NULL;
    return(str);
}

int dup_redirections(t_cmds *cmd) // need to close the open fd's and staff in case of error 
{
    if(cmd->std_in < 0)
    {
        ft_putstr_fd("Minishell: input file not found\n",2);
        return(EXIT_FAILURE);
    }
    if(cmd->std_out < 0)
    {
        ft_putstr_fd("Minishell: output file not found\n",2);
        return(EXIT_FAILURE);
    }
    if(cmd->std_in != 0 || cmd->std_out != 1)
    {
        if(dup2(cmd->std_in,0) < 0)
        {
            ft_putstr_fd("Minishell: dup2 failed\n",2);
            return(EXIT_FAILURE);
        }
        if(dup2(cmd->std_out,1) < 0)
        {
            ft_putstr_fd("Minishell: dup2 failed\n",2);
            return(EXIT_FAILURE);
        }
    }
    return(EXIT_SUCCESS);
}

void signal_exec()
{
    signal(SIGINT,SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
}

void single_cmd_handler(t_cmds *cmd ,t_envp **env)
{
    char **tmp_env;

    tmp_env = env_joiner(*env); // protection here
    if(dup_redirections(cmd) == 1)
    {
        free_2d_arrays(tmp_env);
        exit(EXIT_FAILURE);
    }
    if(builtins_check(cmd->cmd) == 0)
        exit(builtins_handler(cmd,env));
    signal_exec();
    if(execve(cmd->args[0],cmd->args,tmp_env) < 0)
    {
        ft_putstr_fd("Minishell: ",2),
        ft_putstr_fd(cmd->cmd,2),
        ft_putstr_fd(": command not found\n",2),
        exit(127);
    }
}

int check_handler(t_cmds *cmd)
{
    DIR* dir;

    dir = opendir(cmd->cmd);
    if(dir)
    {
        closedir(dir);
        ft_putstr_fd("Minishell: ",2);
        ft_putstr_fd(cmd->cmd,2);
        ft_putstr_fd(": Is a directory\n",2);
        globale.exit_code = 126;
        return(1);
    }
    else
    {
        if(access(cmd->cmd,X_OK) != 0)
        {
            ft_putstr_fd("Minishell: ",2);
            ft_putstr_fd(cmd->cmd,2);
            ft_putstr_fd(": Permission denied\n",2);
            globale.exit_code = 126;
            return(1);
        }
    }
    return(0);
}


void signal_handler_exec(int num)
{
    if(num == SIGINT)
        printf("\n");
	//rl_on_new_line();
    //globale.cmd = -1;
}

void simple_exec_handler(t_cmds *cmd, t_envp **env)
{
    int status;

    if(cmd->cmd == NULL)
        return;
    if(cmd->cmd[0] == '.' && cmd->cmd[1] == '/')
    {
        if(check_handler(cmd) == 1)
            return;
    }
    cmd->pid = fork();
    if(cmd->pid < 0)
    {
        ft_putstr_fd("FORK FAILED\n",2);
        //return(EXIT_FAILURE);
    }
    //globale.cmd = 1;
    if(cmd->pid == 0)
        single_cmd_handler(cmd,env);
    if(cmd->pid > 0)
    {
        waitpid(cmd->pid,&status,0);
        if(WIFEXITED(status))
            globale.exit_code = WEXITSTATUS(status);
        if(WIFSIGNALED(status))
        {
            int signum = WTERMSIG(status);
            globale.exit_code = 130;
            signal_handler_exec(signum);
        }
    }
    //globale.cmd = -1;
}