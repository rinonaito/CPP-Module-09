NAME    = ex00

CC	= c++
CFLAGS  = -Wall -Wextra -Werror -std=c++98

SRC_DIR =       src/
SRCS    =       $(wildcard *.cpp)

OBJ_DIR = obj/
OBJS    = ${SRCS:%.cpp=${OBJ_DIR}%.o}

vpath %.cpp ${SRC_DIR}
vpath %.o ${OBJ_DIR}

INCLUDE		= -I./include

RM              = rm -rf

all             :       ${NAME}
${NAME} :       ${OBJS}
		${CC} ${CFLAGS} ${OBJS} -o $@

${OBJ_DIR}%.o: %.cpp
	mkdir -p ${OBJ_DIR}
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean   :
	${RM} -rf ${OBJ_DIR}

fclean  : clean
	${RM} -rf ${NAME} ${OBJ_DIR}

re              : fclean all

.PHONY: all clean fclean re
