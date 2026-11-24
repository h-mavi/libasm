NAME = libasm.a
SOURCES = ft_strlen.s ft_strcmp.s ft_strcpy.s ft_write.s ft_read.s ft_strdup.s

CC = nasm
FLAGS = -f elf64 -g

CCC = gcc
CFLAGS = -g -Wall -Wextra -Werror

OBJ_DIR = obj
OBJECTS = $(patsubst %.s,$(OBJ_DIR)/%.o,$(SOURCES))

all : $(NAME)

$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $< -o $@


$(NAME) : $(OBJECTS)
	@echo "Compiling $(NAME)"
	ar -rcs $(NAME) $(OBJECTS)

test: re
	${CCC} ${CFLAGS} main.c -L. -lasm -o test

clean: 
	rm -f ${OBJECTS}

fclean: clean 
	rm -f ${NAME}
	rm -f test

re: fclean all

.PHONY: all clean fclean re test
.SILENT: