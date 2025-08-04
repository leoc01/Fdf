# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/23 14:15:42 by lbuscaro          #+#    #+#              #
#    Updated: 2025/07/23 14:15:49 by lbuscaro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf
BONUS_NAME = fdf_bonus

CC = cc
CFLAGS = -Wall -Werror -Wextra -O3
RM = rm -rf

SRC = fdf.c hooks.c gradient.c generate_map.c matrix.c color.c line.c render.c
BONUS_SRC = fdf_bonus.c hooks_bonus.c render_bonus.c loop_bonus.c matrix_bonus.c color_bonus.c line_bonus.c gradient_bonus.c generate_map_bonus.c

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

all: mlx libft/libft.a $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -Llibft -lft -Lminilibx-linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -Iminilibx-linux -Ilibft -c $< -o $@

mlx:
	if [ ! -d "minilibx-linux" ]; then git clone https://github.com/42paris/minilibx-linux.git; fi
	make -C minilibx-linux

libft/libft.a:
	make -C libft all

clean:
	$(RM) $(OBJ)
	$(RM) $(BONUS_OBJ)
	make -C libft clean

fclean: clean
	$(RM) minilibx-linux
	$(RM) $(NAME)
	$(RM) $(BONUS_NAME)
	make -C libft fclean

re: fclean all

bonus: mlx libft/libft.a $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ)
	$(CC) $(CFLAGS) $(BONUS_OBJ) -Llibft -lft -Lminilibx-linux -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -lz -o $(NAME)
