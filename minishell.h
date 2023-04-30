/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:23:15 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/30 15:50:27 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

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
    struct s_cmds	*next;
	struct s_cmds	*prev;
} t_cmds;

enum tokens {
	WORD,
	T_SPACE,
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
t_cmds		*ft_lstnew_cmd(char *command, char **args, int *stds);
void 		ft_deleteNode(t_tokens **head_ref, t_tokens *del);
t_env		*ft_lstnew_envp(char *value, char *name);
void	    ft_lstadd_back_token(t_tokens **lst, t_tokens *new);
t_tokens	*ft_lstlast_token(t_tokens *lst);
void		ft_lstadd_back_envp(t_env **lst, t_env *new);
void		ft_lstadd_back_cmd(t_cmds **lst, t_cmds *new);
char		*ft_itoa(int n);
char		**ft_split(char *s, char c);
void		*free_memory(char **res);

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
int		ft_split_input(char *input, t_tokens **token);

/* PARSING FUNCTIONS */

int		ft_check_syntax(t_tokens *token);
int		check_unclosed_quotes(char *val, int type);
void	ft_cmd_table(t_tokens *token, t_cmds **cmds, t_env *envp);
void	handle_heredoc(t_tokens **tokens, t_env *envp, int file_index);
char	*ft_get_delimiter(t_tokens *token);
void	ft_change_token_value(t_tokens **token, char *file_name);

/* EXPANDER FUNCTIONS */

void	ft_expand_vars(t_tokens **token, t_env *envp);
t_env	*ft_get_var(t_env *envp, char *value);
char	*ft_remove_name(char *env_name);
char	**ft_check_for_var(char *value);
int		ft_is_only_space(char *value);
void	ft_initialize_vars(t_utils *utils);
int		ft_find_var_position(char *value, char *var);

#endif