SRCS := srcs/main.cpp

OBJS := $(SRCS:.cpp=.o)

NAME := tester_scop

COMPILER ?= c++

RM		:= rm -f

CFLAGS 	:= -Wall -Werror -Wextra -g

LIBRARIES := 

.cpp.o:
			${COMPILER} ${CFLAGS} -c $< -o ${<:.cpp=.o}

all: 		${NAME}

${NAME}:	${OBJS}
			${COMPILER} ${OBJS} -o ${NAME} ${LIBRARIES}

clean:
			${RM} ${OBJS}

fclean: 	clean
			${RM} ${NAME}

re:
			make fclean
			make

.PHONY: 	all clean fclean re
