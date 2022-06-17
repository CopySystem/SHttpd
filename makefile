vpath %.h ./adt
vpath %.cpp .
vpath %.cpp ./adt

NAME   = shttpd
TARGET = main.o server.o task.o threadpool.o threadsafequeue.o

$(NAME) : $(TARGET)
	g++ -o $(NAME) $(TARGET) -lpthread -std=c++11

main.o : main.cpp server.h
server.o : server.cpp server.h task.h
task.o : task.cpp task.h
threadpool.o : threadpool.cpp threadpool.h threadsafequeue.h
threadsafequeue.o : threadsafequeue.cpp threadsafequeue.h

.PHONY : clear
clear : 
	rm $(TARGET)

delete :
	rm $(NAME) $(TARGET) 
