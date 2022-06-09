#include "../includes/task.h"
#include "../includes/server.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <fstream>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

extern std::mutex io_ ;

// 创建实例时将给定的socket描述字保存为属性
Task::Task(int fd ) :
	cli_socfd(fd) {
}

// 析构时关闭描述字
Task::~Task() {
	io_.lock() ;
	std::cout << "close \n\n" ;
	io_.unlock() ;
	close(this->cli_socfd) ;
}

// Task运行解析HTTP请求，然后关闭连接
void Task::run() {
	this->read_lines() ;
	this->read_property() ;
	this->response() ;
}

void Task::read_lines() {
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
	int a = 0 , b = 0 , c = 0 ;
	std::string http ; // http请求的完整内容
	while ( (len=read( this->cli_socfd , buffer , BUFSIZ )) && a+b+c < 3) {
		// 如果描述字尚未准备好就休眠100ms
		if ( len == -1 ) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100)) ;
			a = 1 ;
			if ( b ) c = 1 ;
			continue ;
		} 
		b = 1 ;
		buffer[len] = 0 ;
		http += std::string(buffer) ;
	}

	// 读取http头
	std::istringstream in(http) ;
	do {
		len = in.getline(buffer , BUFSIZ).gcount() ;
		if ( len == 2 ) continue ; // 如果是空行就跳过
		buffer[len -  2] = 0 ;
		this->http_head.push_back( std::string(buffer) ) ;
	} while ( in.gcount() != 2 ) ;
	// 读取http体
	while ( !in.eof() ) {
		in.read(buffer , BUFSIZ) ;
		buffer[in.gcount()] = 0 ;
		this->http_body += std::string( buffer ) ;
	}
}

void Task::read_property(){
	for ( int i = 0 ; i < this->http_head[0].length() ; i++ )
		if ( this->http_head[0][i] == ' ' ) 
			this->space.push_back( i ) ;
	for ( int i = 1 ; i < this->http_head.size() ; i++  ) {
		std::string &str = this->http_head[i] ;
		int len = str.length() ;
		for ( int j = 0 ; j < len ; j++ ) 
			if ( str[j] == ':' ) 
				this->other[str.substr(0 , j)] = str.substr(j + 1 , len);
	}
}

std::string Task::request_url() {
	return this->http_head[0].substr(
					this->space[0] + 1, 
					this->space[1] - this->space[0] - 1 ) ;
}

std::string Task::http_method() {
	return this->http_head[0].substr( 0 , this->space[0] ) ;
}

std::string Task::http_vertion() {
	return this->http_head[0].substr(
					this->space[1] + 1 ,
					this->http_head[0].length() - this->space[1] - 1) ;
}

void Task::response() {
	std::string url = this->request_url() ;
	char buffer[BUFSIZ + 4] ;
	// 根路径所对应的文件
	if ( url == "/" ) 
		url = Server::base_path + "/index.html" ;
	else 
		url = Server::base_path + url ;
	io_.lock() ;
	std::cout << "GET : " << url << "\n" ;
	io_.unlock() ;
	std::ifstream f(url , std::fstream::in) ;

	// 返回的http包head和body
	std::string head ;
	std::string body ;
	// 检查路径是否正确，是否存在文件。
	if ( !f.good() ) {
		// 404 NOT FOUND
		f.open( "www/404.html" , std::fstream::in ) ;
		while ( f.good() ) {
			f.read(buffer , BUFSIZ) ;
			buffer[f.gcount()] = 0 ;
			body += std::string(buffer) ;
		}
	} else {
		while ( !f.eof() ) {
			f.read( buffer , BUFSIZ ) ;
			buffer[f.gcount()] = 0 ;
			body += std::string(buffer) ;
		}
	}
	std::string response = this->create_header((int)body.size()) + body ;
	// string.c_str()返回时会向末尾添加空字符
	write( this->cli_socfd , response.c_str() , response.length() + 1 ) ;
	f.close() ;
}

std::string Task::create_header(int content_length) {
	char buffer[BUFSIZ] ;
	const char *head = "HTTP/1.0 200 OK\r\nServer: Martin Server\r\nContent-Type: text/html\r\nConnection: Close\r\nContent-Length: %d\r\n\r\n" ;
	memset( buffer , 0 , BUFSIZ ) ;
	sprintf( buffer , head , content_length ) ;	
	return std::string(buffer) ;
}
