#include "../includes/task.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

extern std::mutex io_ ;

// 创建实例时将给定的socket描述字保存为属性
Task::Task(int fd) :
	cli_socfd(fd) {
}

// 析构时关闭描述字
Task::~Task() {
	close(this->cli_socfd) ;
}

// Task运行解析HTTP请求，然后关闭连接
void Task::run() {
	int len = 0 ;
	char buffer[BUFSIZ + 4] ;
	memset( buffer , 0 , BUFSIZ ) ;

	io_.lock() ;
	// set O_NONBLOCK
	int val ;
	if ( ( val = fcntl( this->cli_socfd , F_GETFL , 0 )  < 0) ) 
		std::cout << "fcntl \"get\" error\n" ;
	if ( fcntl( this->cli_socfd , F_SETFL ,val | O_NONBLOCK ) < 0 )
		std::cout << "fcntl \"SET\" error\n" ;
	io_.unlock() ;

	// read all http request characrers
	// 注意，这里读取所有的字符，意味着也读取了 '\r' 和 '\n'
	std::string http ; // http请求的完整内容
	while ( len = read( this->cli_socfd , buffer , BUFSIZ ) ) {
		// 如果描述字尚未准备好就休眠100ms
		if ( len == -1 ) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100)) ;
			continue ;
		}
		buffer[len] = 0 ;
		http += std::string(buffer) ;
	}

	// 读取http头
	std::istringstream in(http) ;
	do {
		len = in.getline(buffer , BUFSIZ).gcount() ;
		buffer[len -  2] = 0 ;
		this->http_head.push_back( std::string(buffer) ) ;
	} while ( in.gcount() != 2 ) ;
	// 读取http体
	while ( !in.eof() ) {
		in.read(buffer , BUFSIZ) ;
		buffer[in.gcount()] = 0 ;
		this->http_body += std::string( buffer ) ;
	}

	io_.lock() ;
	std::cout << "header : \n" ;
	for ( auto& str : this->http_head )
		std::cout << str << "\n" ;
	std::cout << "body : \n" ;
	std::cout << this->http_body ;
	io_.unlock() ;
}




