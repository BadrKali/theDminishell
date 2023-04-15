# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/08 01:08:31 by abahsine          #+#    #+#              #
#    Updated: 2023/04/15 17:19:40 by abahsine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFLAGS = -Wall -Wextra -Werror

CFILES = minishell.c utils/libft/ft_substr.c utils/libft/ft_putstr_fd.c \
utils/libft/ft_strlen.c utils/libft/ft_isalpha.c utils/libft/ft_strdup.c \
utils/libft/ft_lstadd_back.c utils/libft/ft_lstnew.c utils/libft/ft_strcmp.c \
utils/libft/ft_isdigit.c utils/libft/ft_strjoin.c utils/libft/ft_itoa.c \
utils/lexer/ft_count_tokens.c utils/lexer/ft_split_input.c \
utils/lexer/lexical_analyzer.c \
utils/parser/ft_check_syntax.c utils/parser/ft_cmd_table.c \
utils/expander/ft_expander.c utils/expander/ft_expander_utils.c \
utils/expander/ft_expander_utils_2.c

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