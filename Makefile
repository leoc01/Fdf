NAME = fdf

CC = cc
CFLAGS = -Wall -Werror -Wextra -flto -fstrict-aliasing -ffast-math
RM = rm -rf
SRC = fdf.c hooks.c graphic.c file.c init.c matrix.c color.c utils.c line.c

OBJ = $(SRC:.c=.o)

all: mlx libft/libft.a $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) get_next_line.c get_next_line_utils.c -Llibft -lft -Lminilibx-linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz -O3 -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -Iminilibx-linux -Ilibft -O3 -c $< -o $@

mlx:
	if [ ! -d "minilibx-linux" ]; then git clone https://github.com/42paris/minilibx-linux.git; fi
	make -C minilibx-linux

libft/libft.a:
	make -C libft all

clean:
	$(RM) $(OBJ)
	make -C libft clean

fclean: clean
	$(RM) minilibx-linux
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all

