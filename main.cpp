#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <mutex>

#include "includes/server.h"

std::mutex io_ ;

int main( int argc , char **argv ) {
	Server T( 5000 ) ;
	T.run() ;
	return 0 ;
}
