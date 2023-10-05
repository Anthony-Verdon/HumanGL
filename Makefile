SRCS := srcs/main.cpp \
		srcs/init.cpp \
		srcs/input.cpp \
		srcs/parsing/parseObjFile.cpp \
		srcs/parsing/parseMtlFile.cpp \
		includes/glad/glad.cpp \
		srcs/classes/Camera/Camera.cpp \
		srcs/classes/Shader/Shader.cpp \
		srcs/classes/Texture/Texture.cpp \
		srcs/classes/Time/Time.cpp \
		srcs/classes/Object/Object.cpp \
		srcs/classes/Utils/Utils.cpp \
		srcs/classes/Matrix/Matrix.cpp \
		srcs/classes/Material/Material.cpp

SRCS_BONUS := srcs_bonus/main.cpp \
		srcs_bonus/init.cpp \
		srcs_bonus/input.cpp \
		srcs_bonus/parsing/parseObjFile.cpp \
		srcs_bonus/parsing/parseMtlFile.cpp \
		includes/glad/glad.cpp \
		srcs_bonus/classes/Camera/Camera.cpp \
		srcs_bonus/classes/Shader/Shader.cpp \
		srcs_bonus/classes/Texture/Texture.cpp \
		srcs_bonus/classes/Time/Time.cpp \
		srcs_bonus/classes/Object/Object.cpp \
		srcs_bonus/classes/Utils/Utils.cpp \
		srcs_bonus/classes/Matrix/Matrix.cpp \
		srcs_bonus/classes/Material/Material.cpp


OBJS := $(SRCS:.cpp=.o)

OBJS_BONUS := $(SRCS_BONUS:.cpp=.o)

NAME := scop

NAME_BONUS := scop_bonus

COMPILER ?= c++

RM		:= rm -f

CFLAGS 	:= -Wall -Werror -Wextra -g

LIBRARIES := -Lincludes/glfw3/build/src -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

.cpp.o:
			${COMPILER} ${CFLAGS} -c $< -o ${<:.cpp=.o}

all: 		${NAME}

${NAME}:	${OBJS}
			${COMPILER} ${OBJS} -o ${NAME} ${LIBRARIES}

bonus: 		${NAME_BONUS}

${NAME_BONUS}:	${OBJS_BONUS}
			${COMPILER} ${OBJS_BONUS} -o ${NAME_BONUS} ${LIBRARIES}

tester:
			make -C tester

clean:
			${RM} ${OBJS} ${OBJS_BONUS} 

fclean: 	clean
			${RM} ${NAME} ${NAME_BONUS}

re:
			make fclean
			make

.PHONY: 	all clean fclean re bonus tester
