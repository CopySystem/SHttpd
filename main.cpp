#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <mutex>
#include <unistd.h>

#include "server.h"

std::mutex io_ ;

int main( int argc , char **argv ) {
	Server T( 5005 ) ;
	T.run() ;
	return 0 ;
}
