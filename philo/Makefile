#* VARIABLES
NAME = philo

#TODO: Folders name must end with '\'
SRC_DIR = src/
OBJ_DIR = build/

INCL_DIR = includes
LIB_DIRS =
LIB_FILES = pthread

CC = cc
CFLAGS := -Wall -Wextra -Werror -MMD -g
RM = rm -f
MD = mkdir -p
AR = ar rcs

C_FILES =	init.c		\
			philo.c		\
			routine.c	\
			utils.c

#* Colors

ESC = \033[
DEF_COLOR = $(ESC)0;39m
GRAY = $(ESC)0;90m
RED = $(ESC)0;91m
GREEN = $(ESC)0;92m
YELLOW = $(ESC)0;93m
BLUE = $(ESC)0;94m
MAGENTA = $(ESC)0;95m
CYAN = $(ESC)0;96m
UNDERLINE = $(ESC)4m

COLOR_PRINT = @printf "$(1)$(2)$(DEF_COLOR)\n"


#* Automatic

INCL_FLAGS = $(addprefix -I, $(INCL_DIR))
LIB_FLAGS = $(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIB_FILES))

SRCS = $(addprefix $(SRC_DIR), $(C_FILES))
OBJS := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)
O_DIRS := $(sort $(dir $(OBJS)))

#? cmd for make final file
ifeq ($(suffix $(NAME)), .a)
	LINK_CMD = $(AR) $(NAME) $(OBJS) $(LIB_FLAGS)
else
	LINK_CMD = $(CC) $(OBJS) -o $(NAME) $(CFLAGS) $(LIB_FLAGS)
endif

#? get some size for compilation (see philo.h)
STACK_SIZE := $(shell echo $$(($(shell ulimit -s) * 1024)))
MUTEX_SIZE := $(shell echo '#include <pthread.h>\n#include <stdio.h>\nint main() { printf("%zu", sizeof(pthread_mutex_t)); return 0; }' | cc -xc - -o /tmp/get_size && /tmp/get_size)
PHILO_SIZE := $(shell echo '#include "includes/philo.h"\n#include <stdio.h>\nint main() { printf("%zu", sizeof(struct s_philo)); return 0; }' | cc -xc - -Iincludes -DALLOC_MODE -o /tmp/get_size && /tmp/get_size)
DATA_SIZE := $(shell echo '#include "includes/philo.h"\n#include <stdio.h>\nint main() { printf("%zu", sizeof(struct s_data)); return 0; }' | cc -xc - -Iincludes -DALLOC_MODE -o /tmp/get_size && /tmp/get_size)

CFLAGS +=  -DSTACK_SIZE=$(STACK_SIZE) \
			-DMUTEX_SIZE=$(MUTEX_SIZE) \
			-DPHILO_SIZE=$(PHILO_SIZE) \
			-DDATA_SIZE=$(DATA_SIZE)

#* Rules

all:	$(NAME)

$(NAME): $(O_DIRS) $(OBJS)
	@printf "$(GRAY)"
	$(LINK_CMD)
	$(call COLOR_PRINT,$(GREEN)$(UNDERLINE),$(NAME) compiled !)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(call COLOR_PRINT,$(YELLOW),Compiling: $<)
	@$(CC) -c $< -o $@ $(CFLAGS) $(INCL_FLAGS)

-include $(DEPS)

$(O_DIRS):
	@$(MD) $@

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	$(call COLOR_PRINT,$(BLUE),$(NAME) object files cleaned!)

fclean:		clean
	@$(RM) $(NAME)
	$(call COLOR_PRINT,$(CYAN),executables files cleaned!)

re:		fclean all
	$(call COLOR_PRINT,$(GREEN),Cleaned and rebuilt everything for $(NAME)!)

run:
	./$(NAME) | cat -e

.PHONY:		all clean fclean re run
