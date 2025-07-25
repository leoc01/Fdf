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

CC = cc
CFLAGS = -Wall -Werror -Wextra -flto -fstrict-aliasing -ffast-math -O3
RM = rm -rf
SRC = fdf.c hooks.c gradient.c generate_map.c matrix.c color.c line.c render.c

OBJ = $(SRC:.c=.o)

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
	make -C libft clean

fclean: clean
	$(RM) minilibx-linux
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all

