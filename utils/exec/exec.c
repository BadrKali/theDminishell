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

void exec_cmd(t_cmds **cmd,t_envp **envp)
{
    t_cmds *tmp;
    int cmd_num;
    int i;
    int status;
    
    tmp = *cmd;
    i = 0;
    if(len_liste(*cmd) == 1)
    {
        if(builtins_check(tmp->cmd) == 0)
            globale.exit_code = builtins_handler(tmp,envp);
        else
            simple_exec_handler(tmp,envp);
    }
    else
    {
        cmd_num = len_liste(*cmd);
        multi_command_exec(*cmd,envp,cmd_num);
        while(i < cmd_num)
        {
            waitpid(tmp->pid,&status,0);
            i++;
            tmp = tmp->next;
        }
        if(WIFEXITED(status))
            globale.exit_code = WEXITSTATUS(status);
    }
}





// []->[]->[]->[]->[]