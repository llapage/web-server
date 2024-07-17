# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llapage <llapage@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 10:54:31 by llapage           #+#    #+#              #
#    Updated: 2024/06/11 14:20:15 by llapage          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
#-------------------SOURCES PATH----------------------
SOURCES     = srcs/
INCLUDES	= includes/
#-------------------COMPILATION----------------------
CC        	=   c++
FLAGS    	= 	-Wall -Werror -Wextra -g -std=c++98
#FLAGS   	=   -Wall -Werror -Wextra -g -fsanitize=address -std=c++98
#-------------------SOURCES FILES----------------------

SRCS        =	main.cpp \
				srcs/buffer/BufferManager.cpp \
				srcs/buffer/FileBuffer.cpp \
				srcs/buffer/SocketBuffer.cpp \
				srcs/utils/Converter.cpp \
				srcs/utils/SignalHandler.cpp \
				srcs/parsing/Grammar.cpp \
				srcs/parsing/GrammarRule.cpp \
				srcs/parsing/GrammarSymbol.cpp \
				srcs/parsing/SymbolMatching.cpp \
				srcs/parsing/Recognizer.cpp \
				srcs/parsing/EarleyItem.cpp \
				srcs/parsing/Tokenizer.cpp \
				srcs/parsing/TerminalSymbol.cpp \
				srcs/parsing/NonTerminalSymbol.cpp \
				srcs/parsing/ParseTree.cpp \
				srcs/parsing/Parser.cpp \
				srcs/parsing/ParsingUtils.cpp \
				srcs/configuration/ConfigurationBlock.cpp \
				srcs/configuration/Defaults.cpp \
				srcs/configuration/ConfigurationLoader.cpp \
				srcs/configuration/BlockList.cpp \
				srcs/constants/HttpHeaderHelper.cpp \
				srcs/constants/HttpHelper.cpp \
				srcs/constants/HttpMethodHelper.cpp \
				srcs/constants/HttpStatusCodeHelper.cpp \
				srcs/constants/HttpVersionHelper.cpp \
				srcs/constants/LogLevelHelper.cpp \
				srcs/core/EventManager.cpp \
				srcs/core/PollingService.cpp \
				srcs/exception/ExceptionHandler.cpp \
				srcs/factory/Factory.cpp \
				srcs/logger/Logger.cpp \
				srcs/logger/LoggerConfiguration.cpp \
				srcs/network/Server.cpp \
				srcs/network/Socket.cpp \
				srcs/connection/RequestHandler.cpp \
				srcs/connection/ClientHandler.cpp \
				srcs/connection/Connection.cpp \
				srcs/connection/ConnectionManager.cpp \
				srcs/connection/Session.cpp \
				srcs/pollfd/PollfdManager.cpp \
				srcs/pollfd/PollfdQueue.cpp \
				srcs/request/Request.cpp \
				srcs/request/RequestParser.cpp \
				srcs/request/RequestState.cpp \
				srcs/response/RFCCgiResponseGenerator.cpp \
				srcs/response/UploadResponseGenerator.cpp \
				srcs/response/Response.cpp \
				srcs/response/Router.cpp \
				srcs/response/Route.cpp \
				srcs/response/StaticFileResponseGenerator.cpp \
				srcs/response/DeleteResponseGenerator.cpp

#-------------------OBJECTS----------------------
OBJS        =   $(SRCS:.cpp=.o)
#-------------------HEADERS----------------------
I_H_LIB     =   $(addprefix( -include, $(H_LIB)))
#-------------------COLORS-----------------------
RED         =   \x1b[31m
YELLOW      =   \x1b[33m
GREEN       =   \x1b[32m
NO_COLOR    =   \x1b[0m
BOLD        =   \x1b[1m
BOLD_OFF    =   \x1b[21m
#-------------------RULES-----------------------
%.o: %.cpp
			@printf "$(YELLOW)Generating $(NAME) objects... %-33.33s\r$(NO_COLOR)" $@
			@$(CC) $(FLAGS) -c $< -o $@
$(NAME):	$(OBJS)
			@printf "$(GREEN)Compiling $(NAME)... %33s\r$(NO_COLOR)" " "
			@$(CC) $(FLAGS) $(OBJS)  -o $(NAME) -I$(INCLUDES) -I$(SOURCES)
			@echo "\n$(GREEN)$(BOLD)$@ done !$(BOLD_OFF)$(NO_COLOR)"
all:	$(NAME)

clean:
		@echo "$(RED)Deleting objects...$(NO_COLOR)"
		@rm -rf $(OBJS)
fclean:	clean
		@echo "$(RED)Deleting executables...$(NO_COLOR)"
		@rm -f $(NAME)
re:	fclean all
.PHONY: all clean fclean bonus re
