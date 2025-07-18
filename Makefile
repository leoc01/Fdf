NAME = fdf

CC = cc
CFLAGS = -Wall -Werror -Wextra -flto -fstrict-aliasing -ffast-math
RM = rm -rf
SRC = fdf.c hooks.c graphic.c init.c matrix.c color.c utils.c line.c

OBJ = $(SRC:.c=.o)

all: libft/libft.a $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) get_next_line.c get_next_line_utils.c -Llibft -lft -Lmlx_linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz -O3 -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -Imlx_linux -Ilibft -O3 -c $< -o $@

libft/libft.a:
	make -C libft all

clean:
	$(RM) $(OBJ)
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all

