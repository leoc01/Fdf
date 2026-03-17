NAME = fdf

CC = cc
CFLAGS = -Wall -Werror -Wextra -O3
RM = rm -rf
SRC = fdf.c color.c generate_map.c gradient.c hooks.c line_sdl.c loop.c matrix.c render_sdl.c

OBJ = $(SRC:.c=.o)

all: libft/libft.a $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -Llibft -lft -lSDL2 -lm -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -Ilibft -c $< -o $@

libft/libft.a:
	make -C libft all

clean:
	$(RM) $(OBJ)
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all

