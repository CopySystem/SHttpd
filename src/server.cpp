#include "../includes/server.h"
#include "../includes/task.h"

#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <string.h>
#include <mutex>

extern std::mutex io_ ;

void Server::sys_err(std::string &&msg) {
	io_.lock() ;
	std::cout << msg << std::endl ;
	exit(0) ;
}

Server::~Server() {
	close(this->socfd) ;
}

Server::Server(int p = 80) :
	port( p ) {
	this->socfd = socket( AF_INET , SOCK_STREAM , 0 ) ;

	bzero( &this->addr , sizeof(addr) ) ;
	this->addr.sin_family = AF_INET ;
	this->addr.sin_port = htons(this->port) ;
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	
	if ( bind( this->socfd , 
			   (struct sockaddr*)&this->addr , 
			   sizeof( addr ) ) 
		< 0 ) 
		this->sys_err("bind address error") ;	
	if ( listen( this->socfd , 128 ) < 0 )
		this->sys_err("set listen error") ;

	std::cout << "init end" << std::endl ;
}

void Server::run() {
	std::cout << "server run" << "\n" ;
	struct sockaddr_in cli_addr ;
	int cli_socfd = -1 , size = sizeof(cli_addr) ;
	bzero( &cli_addr , sizeof(cli_addr) ) ;
	while ( true ) {
		cli_socfd = accept(this->socfd , 
						   reinterpret_cast<struct sockaddr*>(&cli_addr) ,
						   (socklen_t *)&size) ;

		// create thread .. .. ..
		if ( cli_socfd != -1 ) {
			std::thread task( &Server::create_task , cli_socfd ) ;
			task.detach() ;
		} else {
			io_.lock() ;
			std::cout << "cli_socfd == -1" << "\n" ;
			io_.unlock() ;
		}

	}
}

void Server::create_task( int cli_socfd ) {
	Task cli_task( cli_socfd ) ;
	cli_task.run() ;
}


