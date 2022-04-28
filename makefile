vpath %.h ./includes
vpath %.cpp .
vpath %.cpp ./src

NAME   = shttpd
TARGET = main.o server.o task.o

$(NAME) : $(TARGET)
	g++ -o $(NAME) $(TARGET) -lpthread -std=c++11

main.o : main.cpp server.h
server.o : server.cpp server.h task.h
task.o : task.cpp task.h

.PHONY : clear
clear : 
	rm $(TARGET)

delete :
	rm $(NAME) $(TARGET) 
