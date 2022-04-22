#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

class Server {
	public :
		Server(int) ;
		~Server() ;

		void run() ;
		void sys_err(std::string &&) ;
		static void create_task(int);

	private :
		int socfd = -1 , port = -1 ;
		std::string hostname , ip ;
		struct sockaddr_in addr ;

} ;
