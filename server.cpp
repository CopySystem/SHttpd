#include "server.h"
#include "task.h"
#include "adt/threadpool.h"

#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <string.h>
#include <mutex>


#define DEBUG_IO

extern std::mutex io_ ;

std::string Server::base_path = "" ;

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
	
	/*
	 *	 获取可执行文件所有的目录，
	 *	 设置其目录下的www文件夹作为服务器根路径
	 * */
	char path[100] ;
	char link[100] ;
	sprintf( link , "/proc/%d/exe" , getpid() ) ;
	int i = readlink( link , path , sizeof( path ) ) , j ;
	for ( j = i - 1 ; j >= 0 && path[j] != '/' ; j-- ) ;
	path[j] = 0 ;
	Server::base_path = std::string(path) + "/www" ;
	
	/*
	 *	socket套接字初始化
	 * */
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
}

/*
 *	等待链接，分离线程
 * */
void Server::run() {
	std::cout << "server run" << "\n\n" ;
	struct sockaddr_in cli_addr ;
	int cli_socfd = -1 , size = sizeof(cli_addr) ;
	bzero( &cli_addr , sizeof(cli_addr) ) ;
	while ( true ) {
		cli_socfd = accept(this->socfd , 
						   reinterpret_cast<struct sockaddr*>(&cli_addr) ,
						   (socklen_t *)&size) ;
#ifdef DEBUG_IO
		io_.lock() ;
		std::cout << "link to : " << inet_ntoa(cli_addr.sin_addr) << "\n" ;
		io_.unlock() ;
#endif

		// create thread .. .. ..
		if ( cli_socfd != -1 ) {
			std::function<void()> fcn = std::bind(&Server::create_task,cli_socfd) ;
            tasks.submit( fcn ) ;
		} else {
#ifdef DEBUG_IO
			io_.lock() ;
			std::cout << "cli_socfd == -1" << "\n" ;
			io_.unlock() ;
#endif
		}

	}
}

void Server::create_task( int cli_socfd ) {
	Task cli_task( cli_socfd ) ;
	cli_task.run() ;
}


