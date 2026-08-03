NAME		=	tokenizer_test

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

SRC			=	main.cpp \
				src/Config.cpp \
				src/ConfigParser.cpp \
				src/Directives.cpp \
				src/Server.cpp \
				src/Token.cpp \
				src/Tokenizer.cpp

OBJ			=	$(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I. -Isrc -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
