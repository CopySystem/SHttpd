vpath %.h ./includes
vpath %.cpp .
vpath %.cpp ./src

TARGET = main.o server.o task.o

main : $(TARGET)
	g++ -o main $(TARGET) -lpthread -std=c++11

main.o : main.cpp server.h
server.o : server.cpp server.h task.h
task.o : task.cpp task.h

.PHONY : clear
clear : 
	rm $(TARGET)

