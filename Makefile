# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 13:13:34 by abahsine          #+#    #+#              #
#    Updated: 2023/05/09 14:34:44 by abahsine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFLAGS = -Wall -Wextra -Werror

CFILES = minishell.c \
utils/env_utils.c utils/env_handlers.c \
utils/libft/ft_strdup.c utils/libft/ft_strlen.c utils/libft/ft_lstnew_token.c \
utils/libft/ft_substr.c utils/libft/ft_putstr_fd.c utils/libft/ft_isalpha.c \
utils/libft/ft_strjoin.c utils/libft/ft_itoa.c utils/libft/ft_isdigit.c \
utils/libft/ft_strcmp.c utils/libft/ft_lstnew_cmd.c utils/libft/ft_split.c \
utils/tokenizer/tokenizer.c utils/tokenizer/tokenizer_utils.c \
utils/tokenizer/token_handlers_two.c utils/tokenizer/token_handlers_three.c \
utils/tokenizer/token_handlers.c \
utils/parser/syntax_analyzer.c utils/parser/command_table.c utils/parser/heredoc.c \
utils/parser/heredoc_utils.c utils/parser/redirections_handlers.c \
utils/parser/redirections_utils.c  utils/parser/command_table_utils_two.c \
utils/parser/command_table_utils.c \
utils/expander/expand_variables.c utils/expander/variables_in_quotes_handler.c \
utils/expander/variables_handler.c utils/expander/variables_in_quotes_utils.c \
utils/leaks_removers.c utils/minishell_utils.c

OFILES = ${CFILES:.c=.o}

all: ${NAME}

${NAME}: ${OFILES} minishell.h
	${CC} ${CFLAGS} ${OFILES} -o ${NAME} -L/Users/abahsine/homebrew/opt/readline/lib -I/Users/abahsine/homebrew/opt/readline/include -lreadline

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f ${OFILES}

fclean: clean
	rm -f ${NAME}

re: fclean all