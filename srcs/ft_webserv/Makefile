# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ide-dieg <ide-dieg@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/01 13:08:20 by ide-dieg          #+#    #+#              #
#    Updated: 2026/07/01 13:47:40 by ide-dieg         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDES = -Iincludes

NAME = webserv

SRCS_DIR = srcs

OBJ_DIR = objs

SRCS =	main.cpp \
		Config.cpp


OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): ide-dieg cpp_title $(OBJS)
	@echo "$(YELLOW)Compiling $(NAME)...$(GREEN)"
	@$(CC) $(CFLAGS) -o $@ $(OBJS)
	@tput cuu1 && tput el
	@echo "$(GREEN)Compilation complete!$(NC)"

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJ_DIR)
	@echo "$(YELLOW)Bounding $< to object file...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@tput cuu1 && tput el
	@echo "$(GREEN)Object file $< created!$(NC)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "$(RED)Cleaning up object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@tput cuu1 && tput el
	@echo "$(GREEN)Object files cleaned!$(NC)"

fclean: clean
	@echo "$(RED)Cleaning up $(NAME)...$(NC)"
	@rm -f $(NAME)
	@tput cuu1 && tput el
	@echo "$(GREEN)All clean!$(NC)"	

re: fclean all

ROJO = \033[0;31m
NC = \033[0m
NARANJA = \033[0;33m
AZUL = \033[0;34m
VERDE = \033[0;32m

ide-dieg:
	@echo "$(ROJO)  __  ____  ____     ____  __  ____  ___        $(AZUL) ██╗  ██╗██████╗ "
	@echo "$(ROJO) (  )(    \(  __)___(    \(  )(  __)/ __)       $(AZUL) ██║  ██║╚════██╗"
	@echo "$(ROJO)  )(  ) D ( ) _)(___)) D ( )(  ) _)( (_ \       $(AZUL) ███████║ █████╔╝"
	@echo "$(ROJO) (__)(____/(____)   (____/(__)(____)\___/       $(AZUL) ╚════██║██╔═══╝ "
	@echo "$(NARANJA) _  _  _  _  __  ____   __   __         _  _    $(AZUL)      ██║███████╗"
	@echo "$(NARANJA)( \/ )/ )( \(  )(    \ / _\ (  )   ___ / )( \   $(AZUL)      ╚═╝╚══════╝"
	@echo "$(NARANJA)/ \/ \\\\\\\\ \/ / )(  ) D (/    \/ (_/\(___)) __ (   $(VERDE)       ██╗  "
	@echo "$(NARANJA)\_)(_/ \__/ (__)(____/\_/\_/\____/     \_)(_/   $(VERDE)   ██╗ ╚═██╗"
	@echo "$(AZUL)  __  ____  _  _  _  _  ____  ____  __ _   __   $(VERDE)   ╚═╝   ██║"
	@echo "$(AZUL) (  )/ ___)( \/ )/ )( \(  __)(  _ \(  ( \ / _\  $(VERDE)   ██╗   ██║"
	@echo "$(AZUL)  )( \___ \/ \/ \) __ ( ) _)  )   //    //    \ $(VERDE)   ╚═╝ ██╔═╝ "
	@echo "$(AZUL) (__)(____/\_)(_/\_)(_/(____)(__\_)\_)__)\_/\_/ $(VERDE)       ╚═╝  $(NC)"

cpp_title:
	@echo
	@echo "██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗"
	@echo "██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║"
	@echo "██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║"
	@echo "██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝"
	@echo "╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ "
	@echo " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  "
	
.PHONY: all clean fclean re ide-dieg cpp_title
