/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:23:15 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/12 22:32:13 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

#define Err_PIPE "minishell: syntax error near unexpected token `|'"

/* LINKED LISTS */

typedef struct s_tokens {
    int             type;
    char            *value;
    struct s_tokens *next;
	struct s_tokens *prev;
} t_tokens;

typedef struct s_utils {
    int				i;
    int				j;
    int				k;
    int				start;
} t_utils;

typedef struct s_env {
    char            *env_name;
    char            *name;
    struct s_env *next;
	struct s_env *prev;
} t_env;

typedef struct s_cmds {
	char			*cmd;
	char			**args;
	int				std_in;
	int				std_out;
} t_cmds;

enum tokens {
	WORD,
	SPACE,
	ARG,
	OR_OPERATOR,
	IR_OPERATOR,
	HEREDOC_OPERATOR,
	APPEND_OPERATOR,
	PIPE,
	QUOTES,
	S_QUOTES,
	VAR
};

/* LIBFT FUNCTIONS */

char	    *ft_substr(char *s, unsigned int start, size_t len);
void	    ft_putstr_fd(char *s, int fd);
size_t	    ft_strlen(char *s);
char		*ft_strdup(char *s1);
char		*ft_strjoin(char *s1, char *s2);
int         ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_strcmp(char *s1, char *s2);
t_tokens	*ft_lstnew_token(char *value, int type);
void		ft_remove_node(t_tokens **token);
t_env		*ft_lstnew_envp(char *value, char *name);
void	    ft_lstadd_back_token(t_tokens **lst, t_tokens *new);
t_tokens	*ft_lstlast_token(t_tokens *lst);
void		ft_lstadd_back_envp(t_env **lst, t_env *new);

/* COUNT TOKENS FUNCTIONS */

void	ft_get_quotes_len(char *input, int *i, int *count, char quote);
void	ft_get_spaces_len(char *input, int *i, int *count);
void	ft_get_redirections_len(int *i, int *count, int flag);
void	ft_get_var_len(char *input, int *i, int *count);
void	ft_get_string_len(char *input, int *i, int *count);

/* SPLIT INPUT FUNCTIONS */

char	*ft_handle_quotes(char	*input, int *i, char quote);
char	*ft_handle_space(char *input, int *i);
char	*ft_hanlde_redirections(char *input, int *i, int flag);
char	*ft_handle_variable(char *input, int *i);
char	*ft_handle_string(char *input, int *i);
void	ft_split_input(char *input, t_tokens **token);

/* PARSING FUNCTIONS */

int    ft_check_syntax(t_tokens *token);


/* EXPANDER FUNCTIONS */

void	ft_expand_vars(t_tokens **token, t_env *envp);
t_env	*ft_get_var(t_env *envp, char *value);
char	*ft_remove_name(char *env_name);
char	**ft_check_for_var(t_tokens *token);
int		ft_is_only_space(char *value);
void	ft_initialize_vars(t_utils *utils);
int		ft_find_var_position(char *value, char *var);

#endif