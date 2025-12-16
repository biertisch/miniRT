NAME = miniRT
SRC = $(addprefix src/, main.c color.c vec3.c ray.c sphere.c hittable_list.c interval.c \
						camera.c rt_utils.c material.c action.c quad.c plane.c cylinder.c \
						texture.c texture_solid_color.c texture_checker.c diffuse_light.c \
						onb.c pdf.c cone.c parser/error.c parser/error_utils.c parser/keyword.c\
						parser/parse_color.c parser/parse_fields.c parser/parse_acl_fields.c\
						parser/parse_object_fields.c parser/parse_scalar.c parser/parse_vector.c\
						parser/parser.c parser/rules.c parser/rules2.c parser/ft_atof.c\
						parser/ft_ftoa.c)
LIBFT_DIR = ./libft
LIBFT_A = $(LIBFT_DIR)/libft.a

CC = cc
#CCFLAGS = -Wextra -Wall -Werror -MMD --std=gnu11
CCFLAGS = -Werror -MMD --std=gnu11

D ?= 0

ifeq ($(D),1)
    CCFLAGS += -DDEBUG=1
else
    CCFLAGS += -DDEBUG=0
endif

OBJ_DIR = objs

OBJ  = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ) $(MLX) $(LIBFT_A)
	$(CC) $(CCFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -g -I/usr/include -Iincludes -Ilibft -Imlx_linux -O0 -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) DEBUG=$(D)

$(MLX):
	@$(MAKE) -C $(MLX_PATH)

MLX_URL = https://cdn.intra.42.fr/document/document/40913/minilibx-linux.tgz
MLX_DIR = mlx_linux
MLX_TGZ = $(MLX_DIR).tgz

dmlx:
	@if [ ! -d $(MLX_DIR) ]; then \
		echo "Downloading MiniLibX..."; \
		curl -sSL $(MLX_URL) -o $(MLX_TGZ); \
		mkdir -p $(MLX_DIR); \
		tar -xzf $(MLX_TGZ) -C $(MLX_DIR); \
		mv $(MLX_DIR)/minilibx-linux/* $(MLX_DIR); \
		rm -f $(MLX_TGZ); \
		echo "Building MiniLibX..."; \
		$(MAKE) -C $(MLX_DIR); \
	else \
		echo "MiniLibX already present."; \
	fi

clean:
	-rm -f $(OBJ)
	-rm -fr $(OBJ_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	-rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

run: re
	./$(NAME)

-include $(OBJ:.o=.d)

re : fclean all
