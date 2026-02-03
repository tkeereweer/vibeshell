# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#              #
#    Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I includes -I libft

# Lexer sources
LEXER_DIR	= src/lexer
LEXER_FILES	= tokenize.c \
			  lexer_process.c \
			  state_handlers.c \
			  quote_handlers.c \
			  operator_handler.c \
			  char_classify.c \
			  operator_recognize.c \
			  token_utils.c \
			  buffer_utils.c

# Parser sources
PARSER_DIR	= src/parser
PARSER_FILES = parse.c \
			   parse_precedence.c \
			   parse_primary.c \
			   parse_command.c \
			   parse_redirect.c \
			   node_create.c \
			   node_free.c \
			   parse_helpers.c \
			   parse_token_check.c \
			   parse_utils.c \
			   build_argv.c \
			   validate_syntax.c \
			   parse_error.c

# Executor sources
EXECUTOR_DIR	= src/executor
EXECUTOR_FILES	= execute.c \
				  exec_command.c \
				  exec_redirect.c \
				  exec_pipe.c \
				  exec_logical.c \
				  exec_subshell.c \
				  exec_utils.c \
				  path_resolve.c

# Environment sources
ENV_DIR		= src/env
ENV_FILES	= env_init.c \
			  env_get.c \
			  env_set.c \
			  env_unset.c \
			  env_to_array.c \
			  env_utils.c

# Built-in sources
BUILTINS_DIR	= src/builtins
BUILTINS_FILES	= builtin_dispatcher.c \
				  builtin_echo.c \
				  builtin_cd.c \
				  builtin_pwd.c \
				  builtin_env.c \
				  builtin_export.c \
				  builtin_unset.c \
				  builtin_exit.c

# Util sources
UTILS_DIR	= src/utils
UTILS_FILES	= error.c

# Main source
MAIN_SRC	= src/main.c

# Combine all sources
LEXER_SRCS		= $(addprefix $(LEXER_DIR)/, $(LEXER_FILES))
PARSER_SRCS		= $(addprefix $(PARSER_DIR)/, $(PARSER_FILES))
EXECUTOR_SRCS	= $(addprefix $(EXECUTOR_DIR)/, $(EXECUTOR_FILES))
ENV_SRCS		= $(addprefix $(ENV_DIR)/, $(ENV_FILES))
BUILTINS_SRCS	= $(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES))
UTILS_SRCS		= $(addprefix $(UTILS_DIR)/, $(UTILS_FILES))

SRCS		= $(MAIN_SRC) $(LEXER_SRCS) $(PARSER_SRCS) $(EXECUTOR_SRCS) $(ENV_SRCS) $(BUILTINS_SRCS) $(UTILS_SRCS)
OBJS		= $(SRCS:.c=.o)

# Libft
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

$(LIBFT):
	@if [ -d "$(LIBFT_DIR)" ]; then \
		make -C $(LIBFT_DIR); \
	else \
		echo "Warning: libft directory not found. Please add your libft."; \
	fi

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@if [ -d "$(LIBFT_DIR)" ]; then make -C $(LIBFT_DIR) clean; fi

fclean: clean
	rm -f $(NAME)
	@if [ -d "$(LIBFT_DIR)" ]; then make -C $(LIBFT_DIR) fclean; fi

re: fclean all

.PHONY: all clean fclean re
