NAME := pipex

SRC:= $(addprefix $(SRC_DIR),exec.c utils.c handle_pipe.c child.c pipe.c)
ifdef BONUS
	SRC:= $(SRC) $(addprefix $(SRC_DIR), init_bonus.c)
else	
	SRC:= $(SRC) $(addprefix $(SRC_DIR), init.c)
endif
OBJ_DIR:= .obj/
OBJ:= $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
DEPS:= $(OBJ:%.o=%.d)

CC:= cc
CCFLAGS:= -Wextra -Wall -Werror -g3
CPPFLAGS = -MMD -MP
SRC_DIR:= src/
INCLUDES:= include/

LIBFT_DIR := libft/
LIBFT := $(LIBFT_DIR)libft.a 
LIBFT_FLAG := -L $(LIBFT_DIR) $(LIBFT)

HEADERS:= -I $(INCLUDES) -I $(LIBFT_DIR)

all: welcome $(NAME)

$(NAME): $(LIBFT) $(OBJ) 
	$(CC) $(CCFLAGS) $(OBJ) $(LIBFT_FLAG) -o $(NAME)
	@echo "👨‍🍳 Cooking up binary goodness: $(BLUE)$(CC) $(CCFLAGS) $(OBJ) $(LIBFT_FLAG) -o $(NAME)$(DEF_COLOR)"
	@echo "$(GREEN)🍽️ Le chef a COOK $(NAME) à la perfection ! Bon appétit ! 🍽️$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo "🛠️  $(MAGENTA)Compiling: $< $(DEF_COLOR)"
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(HEADERS) -o $@ -c $<

$(LIBFT): libft

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
-include $(DEPS)
# Colors

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

bonus:
	$(MAKE) BONUS=1 all
	
welcome:
	@echo "🍳 $(CYAN)Préchauffage des fourneaux pour $(NAME)... La compilation mijote ! $(DEF_COLOR)"

clean:
	@echo "🧹 $(YELLOW)Cleaning object files...$(DEF_COLOR)"
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)"
	@echo "        . . . . . . . . . ."
	@echo "      .     *     *     *"
	@echo "   .       *       *       ."
	@echo "         *   BOOM!   *"
	@echo "      .       *       *       ."
	@echo "    *     *     *     *     *"
	@echo "      ' . . . . . . . . '"
	@echo "$(DEF_COLOR)"
	@echo "$(RED)🧨🧨🧨🧨🧨💥 $(NAME) remove 💥🧨🧨🧨🧨🧨 $(DEF_COLOR)"
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)
re: fclean all

info:
	@echo "OBJ": $(OBJ)
	@echo "DEPS": $(DEPS)

.PHONY: all clean fclean re libft bonus