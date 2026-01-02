NAME = miniRT
SRC = $(addprefix src/, main.c color.c color_op.c vec3.c vec3_op.c ray.c geobjs/sphere.c hittable_list.c hook_manager.c \
						interval.c camera_animation.c camera.c phong.c camera_utils.c debug_info.c rt_utils.c material.c \
						action.c action_extend.c geobjs/quad.c geobjs/plane.c geobjs/cylinder.c geobjs/cylinder_utils.c \
						geobjs/cone.c geobjs/cone_quadratic.c geobjs/root_calc.c geobjs/triangle.c bump.c bump_tbn.c bump_map.c texture.c \
						texture_solid_color.c texture_checker.c diffuse_light.c onb.c pdf.c parser/error.c parser/error_message.c \
						parser/error_utils.c parser/keyword.c parser/parse_color.c parser/parse_fields.c parser/parse_acl_fields.c\
						parser/parse_object_fields.c parser/parse_object_fields2.c parser/parse_scalar.c parser/parse_vector.c\
						parser/parser.c parser/rules.c parser/rules2.c parser/ft_atof.c\
						parser/ft_ftoa.c controls/controls.c controls/drag.c controls/hooks.c\
						controls/render_objects.c controls/render_sliders.c controls/render_text.c\
						controls/bitmaps.c controls/render_utils.c controls/render.c controls/scroll.c\
						controls/sliders_utils.c controls/values.c controls/transform.c controls/matrix.c\
						controls/rotation.c controls/translation.c controls/resize.c controls/reset.c\
						mouseselect.c scene.c worldfree.c surfacechecker.c texture_picture.c info.c)
LIBFT_DIR = ./libft
LIBFT_A = $(LIBFT_DIR)/libft.a

MLX_DIR = minilibx-linux
MLX_LIB = -L$(MLX_DIR) -lmlx
MLX_INC = -I$(MLX_DIR)

CC = cc
CCFLAGS = -Wextra -Wall -Werror -MMD -g

D ?= 0

ifeq ($(D),1)
    CCFLAGS += -DDEBUG=1
else
    CCFLAGS += -DDEBUG=0
endif

OBJ_DIR = objs

OBJ  = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(MLX_DIR)/libmlx.a $(LIBFT_A) $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft $(MLX_LIB) -lXext -lX11 -lm -lz -o $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -g -Iincludes -I$(LIBFT_DIR) $(MLX_INC) -O0 -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR) DEBUG=$(D)

$(MLX_DIR)/libmlx.a:
	@$(MAKE) -C $(MLX_DIR)

MLX_URL = https://cdn.intra.42.fr/document/document/40913/minilibx-linux.tgz
#MLX_DIR = mlx_linux
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

debug: fclean
	$(MAKE) D=1
	@./$(NAME) ./RT/base2.rt

.PHONY: all clean fclean re