#include"../../minishell.h"


void print_envp(t_envp *env)
{
	while(env != NULL)
	{
		printf("%s%s\n",env->env_pre,env->envp_value);
		env = env->next;
	}
}

int ft_env(t_envp *env)
{
    print_envp(env);
    return(EXIT_SUCCESS);
}