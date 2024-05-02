SRCS = main.cpp srcs/server/Server.cpp \
	srcs/config/DataConfig.cpp srcs/config/ParseConfigFile.cpp \
	srcs/httpstuff/Request.cpp srcs/httpstuff/Response.cpp srcs/httpstuff/RMethodsGet.cpp srcs/httpstuff/RMethodsDelete.cpp srcs/httpstuff/Client.cpp srcs/httpstuff/RMethodsPost.cpp \
	srcs/cgi/Cgi.cpp srcs/cgi/CgiOutput.cpp 

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

CPP = c++

FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJS)
	$(CPP) $(FLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp Server.hpp
	$(CPP) $(FLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re