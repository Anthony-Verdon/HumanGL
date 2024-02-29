SRCS := srcs/main.cpp \
		libs/glad/glad.cpp \
		srcs/classes/Camera/Camera.cpp \
		srcs/classes/Shader/Shader.cpp \
		srcs/classes/Texture/Texture.cpp \
		srcs/classes/Time/Time.cpp \
		srcs/classes/ObjectClasses/Object/Object.cpp \
		srcs/classes/ObjectClasses/ObjectData/ObjectData.cpp \
		srcs/classes/ObjectClasses/ObjectParser/ObjectParser.cpp \
		srcs/classes/MaterialClasses/Material/Material.cpp \
		srcs/classes/MaterialClasses/MaterialData/MaterialData.cpp \
		srcs/classes/MaterialClasses/MaterialParser/MaterialParser.cpp \
		srcs/classes/WindowManagement/WindowManagement.cpp \
		srcs/classes/Utils/Utils.cpp \
		srcs/classes/Matrix/Matrix.cpp \

SRCS_TEST := srcs/tester/tester.cpp \
		libs/glad/glad.cpp \
		srcs/classes/Utils/Utils.cpp \
		srcs/classes/ObjectClasses/Object/Object.cpp \
		srcs/classes/ObjectClasses/ObjectData/ObjectData.cpp \
		srcs/classes/ObjectClasses/ObjectParser/ObjectParser.cpp \
		srcs/classes/MaterialClasses/Material/Material.cpp \
		srcs/classes/MaterialClasses/MaterialData/MaterialData.cpp \
		srcs/classes/MaterialClasses/MaterialParser/MaterialParser.cpp \

SRCS_BONUS := srcs_bonus/main.cpp \
		libs/glad/glad.cpp \
		srcs_bonus/classes/Camera/Camera.cpp \
		srcs_bonus/classes/Shader/Shader.cpp \
		srcs_bonus/classes/Texture/Texture.cpp \
		srcs_bonus/classes/Time/Time.cpp \
		srcs_bonus/classes/ObjectClasses/Object/Object.cpp \
		srcs_bonus/classes/ObjectClasses/ObjectData/ObjectData.cpp \
		srcs_bonus/classes/ObjectClasses/ObjectParser/ObjectParser.cpp \
		srcs_bonus/classes/MaterialClasses/Material/Material.cpp \
		srcs_bonus/classes/MaterialClasses/MaterialData/MaterialData.cpp \
		srcs_bonus/classes/MaterialClasses/MaterialParser/MaterialParser.cpp \
		srcs_bonus/classes/WindowManagement/WindowManagement.cpp \
		srcs_bonus/classes/Utils/Utils.cpp \
		srcs_bonus/classes/Matrix/Matrix.cpp \

SRCS_TEST_BONUS := srcs_bonus/tester/tester.cpp \
		libs/glad/glad.cpp \
		srcs_bonus/classes/Utils/Utils.cpp \
		srcs_bonus/classes/ObjectClasses/Object/Object.cpp \
		srcs_bonus/classes/ObjectClasses/ObjectData/ObjectData.cpp \
		srcs_bonus/classes/ObjectClasses/ObjectParser/ObjectParser.cpp \
		srcs_bonus/classes/MaterialClasses/Material/Material.cpp \
		srcs_bonus/classes/MaterialClasses/MaterialData/MaterialData.cpp \
		srcs_bonus/classes/MaterialClasses/MaterialParser/MaterialParser.cpp \

OBJS := $(SRCS:.cpp=.o)

OBJS_TEST := $(SRCS_TEST:.cpp=.o)

OBJS_BONUS := $(SRCS_BONUS:.cpp=.o)

OBJS_TEST_BONUS := $(SRCS_TEST_BONUS:.cpp=.o)

NAME := scop

NAME_TEST := scop_test

NAME_BONUS := scop_bonus

NAME_TEST_BONUS := scop_test_bonus

COMPILER ?= c++

RM		:= rm -f

CFLAGS 	:= -Wall -Werror -Wextra -g -std=c++17

LIBRARIES := -Llibs/glfw3/build/src -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl

.cpp.o:
			${COMPILER} ${CFLAGS} -c $< -o ${<:.cpp=.o}

all: 		${NAME}

${NAME}:	${OBJS}
			${COMPILER} ${OBJS} -o ${NAME} ${LIBRARIES}

test: 		${NAME_TEST}

${NAME_TEST}:	${OBJS_TEST}
			${COMPILER} ${OBJS_TEST} -o ${NAME_TEST}

bonus: 		${NAME_BONUS}

${NAME_BONUS}:	${OBJS_BONUS}
			${COMPILER} ${OBJS_BONUS} -o ${NAME_BONUS} ${LIBRARIES}

testbonus: ${NAME_TEST_BONUS}

${NAME_TEST_BONUS}: ${OBJS_TEST_BONUS}
			${COMPILER} ${OBJS_TEST_BONUS} -o ${NAME_TEST_BONUS}

clean:
			${RM} ${OBJS} ${OBJS_BONUS} ${OBJS_TEST}

fclean: 	clean
			${RM} ${NAME} ${NAME_BONUS} ${NAME_TEST}

re:
			make fclean
			make
			make test
			make bonus
			make testbonus

.PHONY: 	all clean fclean re test bonus testbonus
