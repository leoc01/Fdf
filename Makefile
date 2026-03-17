BUILD_DIR = build
OBJ_DIR_NATIVE = $(BUILD_DIR)/obj
OBJ_DIR_WEB = $(BUILD_DIR)/obj_web
WEB ?= 0

CC_NATIVE = cc
CC_WEB = emcc
CFLAGS = -Wall -Werror -Wextra -O3
WEB_CFLAGS = -sUSE_SDL=2 -D__EMSCRIPTEN__
RM = rm -rf
SRC_NATIVE = src/fdf.c src/color.c src/generate_map.c src/gradient.c src/hooks.c src/line_sdl.c src/loop.c src/matrix.c src/render_sdl.c
SRC_WEB = src/emscripten_main.c src/color.c src/generate_map.c src/gradient.c src/hooks.c src/line_sdl.c src/loop.c src/matrix.c src/render_sdl.c

OBJ_NATIVE = $(SRC_NATIVE:src/%.c=$(OBJ_DIR_NATIVE)/%.o)
OBJ_WEB = $(SRC_WEB:src/%.c=$(OBJ_DIR_WEB)/%.o)

ifeq ($(WEB),1)
NAME = $(BUILD_DIR)/fdf.html
OBJ_DIR = $(OBJ_DIR_WEB)
OBJ = $(OBJ_WEB)
CC = $(CC_WEB)
EXTRA_CFLAGS = $(WEB_CFLAGS)
SYS_INC =
LINK_LIBS = -Llibft -lft -lm
LINK_EXTRA = --preload-file maps -s ALLOW_MEMORY_GROWTH=1 -s INITIAL_MEMORY=67108864 -s WASM=1 -s USE_SDL=2 --shell-file working_template.html
LIBFT_MAKE = emmake make -C libft all
else
NAME = $(BUILD_DIR)/fdf
OBJ_DIR = $(OBJ_DIR_NATIVE)
OBJ = $(OBJ_NATIVE)
CC = $(CC_NATIVE)
EXTRA_CFLAGS =
SYS_INC = -I/usr/include
LINK_LIBS = -Llibft -lft -lSDL2 -lm
LINK_EXTRA =
LIBFT_MAKE = make -C libft all
endif

all: libft/libft.a $(NAME)

$(NAME): $(OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(OBJ) $(LINK_LIBS) -o $(NAME) $(LINK_EXTRA)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(SYS_INC) -Iinclude -Ilibft -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

libft/libft.a:
	$(LIBFT_MAKE)

clean:
	$(RM) $(OBJ_DIR_NATIVE) $(OBJ_DIR_WEB)
	make -C libft clean

fclean: clean
	$(RM) $(BUILD_DIR)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re web

web:
	$(MAKE) WEB=1 all

.SUFFIXES:
MAKEFLAGS += -r
