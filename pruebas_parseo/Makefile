NAME		=	parser_test

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

SRC			=	main.cpp \
				src/BlockParser.cpp \
				src/Config.cpp \
				src/ConfigParser.cpp \
				src/Directives.cpp \
				src/Listen.cpp \
				src/Location.cpp \
				src/LocationParser.cpp \
				src/ServerParser.cpp \
				src/Token.cpp \
				src/Tokenizer.cpp \
				http/HTTPMethods.cpp \
				http/HTTPStatus.cpp

OBJ			=	$(SRC:.cpp=.o)
TEST_LOCATIONS	=	tests/test_locations.out

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I. -Isrc -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_LOCATIONS)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(TEST_LOCATIONS): tests/test_locations.cpp $(filter-out main.o,$(OBJ))
	$(CXX) $(CXXFLAGS) -I. -Isrc $^ -o $@

test: $(NAME) $(TEST_LOCATIONS)
	python3 tests/test_directives.py
	./$(TEST_LOCATIONS)

.PHONY: all clean fclean re test
