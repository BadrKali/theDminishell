/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:15:19 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/09 15:45:00 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# include <readline/history.h>
# include <readline/readline.h>

#define ERR "minishell: syntax error near unexpected token `|'\n"

typedef struct s_tokens {
	int				type;
	char			*value;
	int				is_joined;
	struct s_tokens	*next;
	struct s_tokens	*prev;
}	t_tokens;

typedef struct s_envp {
	char			*envp_name;
	char			*envp_value;
	struct s_envp	*next;
	struct s_envp	*prev;
}	t_envp;

typedef struct s_cmds {
	char			*cmd;
	char			**args;
	int				std_in;
	int				std_out;
	struct s_cmds	*next;
	struct s_cmds	*prev;
}	t_cmds;

enum e_tokens {
	WORD,
	T_SPACE,
	OR_OPERATOR,
	IR_OPERATOR,
	HEREDOC_OPERATOR,
	APPEND_OPERATOR,
	PIPE,
	QUOTES,
	S_QUOTES,
	VAR
};

/**************
LIBFT FUNCTIONS
***************/

char		*ft_strdup(char *s1);
char		*ft_substr(char *s, unsigned int start, size_t len);
size_t		ft_strlen(char *s);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_strcmp(char *s1, char *s2);
char		*ft_itoa(int n);
void		ft_putstr_fd(char *s, int fd);
char		*ft_strjoin(char *s1, char *s2);
void		ft_deletenode(t_tokens **head_ref, t_tokens *del);
t_tokens	*ft_lstnew_token(char *value, int type);
t_tokens	*ft_lstlast_token(t_tokens *lst);
void		ft_lstadd_back_token(t_tokens **lst, t_tokens *new);
void		ft_lstadd_back_cmd(t_cmds **lst, t_cmds *new);
t_cmds		*ft_lstnew_cmd(char *command, char **args, int *stds);
char		**ft_split(char *s, char c);
void		*free_2d_arrays(char **res);
void		ft_lstadd_back_envp(t_envp **lst, t_envp *new);
t_envp		*ft_lstnew_envp(char *value, char *name);
void		fill_env_pointer(t_envp **envp, char *env[]);

/******************
TOKENIZER FUNCTIONS
******************/

void		handle_string(t_tokens **token, char *input, int *end);
void		handle_variable_two(t_tokens **token, t_envp *envp, char *input,
				int *end);
void		handle_variable(t_tokens **token, t_envp *envp, char *input,
				int *end);
void		handle_redirections(t_tokens **token, char *input, int *i,
				int flag);
void		handle_redirections_flag_one(t_tokens **token, char *input,
				int start, int *end);
void		handle_redirections_flag_two(t_tokens **token, char *input,
				int start, int *end);
void		handle_quotes_two(t_tokens **token, t_envp *envp, char *tok);
void		handle_space(t_tokens **token, char *input, int *i);
int			input_tokenizer(t_tokens **token, t_envp *envp, char *input);
int			check_is_end(char *input, int i);
int			handle_quotes(t_tokens **token, t_envp *envp, char *input, int *i);
int			check_unclosed_quotes(char *val, int type);
int			find_quotes(char *val, int c);
int			find_space(char *value);
int			check_previous(t_tokens *token);
int			check_variable_syntax(char *val, int type);
int			is_there_same_type_next(t_tokens *token, int type);
int			*allocate_for_stds(int std_in, int std_out);

/****************
PARSER FUNCTIONS
****************/

int			syntax_checker(t_tokens *token);
void		command_table(t_tokens *token, t_cmds **cmd, t_envp *envp);
int			handle_heredoc(t_tokens **tokens, t_envp *envp, int file_index);
char		*get_delimiter(t_tokens *token);
void		change_red_value(t_tokens **token);
void		change_delimiter_value(t_tokens **token, char *file_name);
void		change_token_value(t_tokens **token, char *file_name);
int			check_delimiter_type(t_tokens *token);
int			check_is_joined(t_tokens *token);
char		*get_path(t_envp *envp, char *cmd);
char		*fill_arg(t_tokens **token);
char		*merge_args(t_tokens **token);
int			get_arguments_len(t_tokens *token);
int			*handle_redirections_for_cmd(t_tokens **token, t_envp *envp);
void		skip_redirections(t_tokens **token);

/*****************
EXPANDER FUNCTIONS
*****************/

void		handle_variables_in_quotes(t_tokens **token, t_envp *envp);
void		get_variables_in_quotes(char **envs, char *value, int *i, int *j);
t_envp		*get_variable(t_envp *envp, char *value);
int			get_envs_in_quotes_len(char *value);
int			find_variable_position(char *value, char *var);
char		**find_variables_in_quotes(char *value);
void		handle_variables(t_tokens **token, t_envp *envp, t_tokens **head);
t_envp		*get_variable(t_envp *envp, char *value);

/**************
LEAKS FUNCTIONS
**************/

void		free_linked_list(t_tokens *token);
void		free_linked_list_cmd(t_cmds *cmd);
void		*free_2d_arrays(char **res);
void		delete_two_exec_spaces(t_tokens **token);

/**************
OTHER FUNCTIONS
**************/

void		close_open_fds(t_cmds *cmd);
void		delete_tmp_files(t_tokens *token);
void		mini_cleaner(t_tokens **token, t_cmds **cmd, char *input);

#endif