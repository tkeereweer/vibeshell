NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = ft_strdup.c ft_strlen.c ft_putstr_fd.c ft_strcmp.c \
	   ft_strjoin.c ft_strchr.c ft_split.c ft_free_split.c \
	   ft_isalpha.c ft_isalnum.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
