NAME		=	webserv

CC			=	c++
CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98

# directories
SRC_DIR		=	srcs/
OBJ_DIR		=	objs/
INC_DIR		=	includes/

# controll codes
RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m
UP			=	\033[A
CUT			=	\033[K

#source files
SRC_FILES	=	main.cpp \
				server/Server.cpp \
				server/Client.cpp \
				config/Config.cpp \
				config/ConfigParser.cpp \
				http/HTTPException.cpp \
				http/HTTPRequest.cpp \
				http/HTTPRequestParser.cpp \
				http/HTTPResponse.cpp \
				http/HttpStatus.cpp \

OBJ_FILES	=	$(SRC_FILES:.cpp=.o)

#paths
SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ			=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

#all rule
all: $(NAME)

#compile the executable
$(NAME): $(OBJ)
	@echo -e "$(YELLOW)Compiling [$(NAME)]...$(RESET)"
	@$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo -e "$(GREEN)Finished [$(NAME)]$(RESET)"

#compile objects
$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	@echo -e "$(YELLOW)Compiling [$@]...$(RESET)"
	@$(CC) $(CPPFLAGS) -I $(INC_DIR) -o $@ -c $<
	@printf "$(UP)$(CUT)"
	@echo -e "$(GREEN)Finished [$@]$(RESET)"
	@printf "$(UP)$(CUT)"

#clean rule
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
	rm -rf $(OBJ_DIR); \
	echo -e "$(BLUE)Deleting all objects $(RESET)"; else \
	echo "No objects to remove."; \
	fi;

#fclean rule
fclean: clean
	@if [ -f "$(NAME)" ]; then \
	rm -f $(NAME); \
	echo -e "$(BLUE)Deleting $(NAME) $(RESET)"; else \
	echo "No Executable to remove."; \
	fi;

#re rule
re: fclean all

#cgi standalone test (no depende de $(OBJ) ni toca objs/, para no interferir
#con el resto del build mientras el CGI no esta integrado en el server)
TEST_CGI_BIN	=	test_cgi

test-cgi:
	@echo -e "$(YELLOW)Compiling [$(TEST_CGI_BIN)]...$(RESET)"
	@$(CC) $(CPPFLAGS) -I $(INC_DIR) -I $(INC_DIR)cgi \
		$(SRC_DIR)cgi/CgiHandler.cpp tests/cgi/test_cgi.cpp \
		-o $(TEST_CGI_BIN)
	@echo -e "$(GREEN)Finished [$(TEST_CGI_BIN)]$(RESET)"
	@./$(TEST_CGI_BIN)

clean-test-cgi:
	@rm -f $(TEST_CGI_BIN)

#phony
.PHONY: all clean fclean re test-cgi clean-test-cgi