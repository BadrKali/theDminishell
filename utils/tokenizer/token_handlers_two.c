/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:33:44 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 16:24:54 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	get_args_len(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

static void	space_at_variable_beginning(t_tokens **token, t_envp *var)
{
	t_tokens	*last_token;

	last_token = ft_lstlast_token(*token);
	if ((var->envp_value[0] == ' ' || var->envp_value[0] == '\n'
			|| var->envp_value[0] == '\r' || var->envp_value[0] == '\f'
			|| var->envp_value[0] == '\v' || var->envp_value[0] == '\t')
		&& last_token && last_token->type != QUOTES
		&& last_token->type != S_QUOTES)
		ft_lstadd_back_token(token, ft_lstnew_token(ft_strdup(" "), T_SPACE));
}

static void	handle_variable_with_space(t_tokens **token, t_envp *var)
{
	char		**args;
	int			end;
	int			i;

	args = ft_split(var->envp_value, ' ');
	end = get_args_len(args);
	end--;
	i = 0;
	space_at_variable_beginning(token, var);
	while (i != end)
	{
		ft_lstadd_back_token(token, ft_lstnew_token(args[i++], VAR));
		ft_lstadd_back_token(token, ft_lstnew_token(ft_strdup(" "), T_SPACE));
	}
	ft_lstadd_back_token(token, ft_lstnew_token(args[i++], VAR));
	if (var->envp_value[ft_strlen(var->envp_value) - 2] == ' ')
		ft_lstadd_back_token(token, ft_lstnew_token(ft_strdup(" "), T_SPACE));
	free(args);
}

static void	handle_variable_without_space(t_tokens **token, t_envp *envp,
	char *res)
{
	t_tokens	*last_token;

	ft_lstadd_back_token(token, ft_lstnew_token(res, VAR));
	last_token = ft_lstlast_token(*token);
	handle_variables(&last_token, envp, token);
}

void	handle_variable_two(t_tokens **token, t_envp *envp, char *input,
		int *end)
{
	t_envp		*var;
	char		*res;
	int			start;

	start = *end;
	start--;
	while (input[*end] && (ft_isalpha(input[*end]) || ft_isdigit(input[*end])
			|| input[*end] == '_'))
		(*end)++;
	res = ft_substr(input, start, (*end) - start);
	if (check_previous(*token))
		ft_lstadd_back_token(token, ft_lstnew_token(res, VAR));
	else
	{
		var = get_variable(envp, res);
		if (var && find_space(var->envp_value))
			handle_variable_with_space(token, var);
		else
			handle_variable_without_space(token, envp, res);
	}
}
