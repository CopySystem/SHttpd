#include <string>
#include <vector>

class Task {
	public :
		Task(int) ;
		~Task() ;
		void run() ; 		// 运行Task，解析HTTP请求
		void readline() ;	// 读取HTTP请求头中的一行
	private :
		int cli_socfd = -1    ;
		std::vector<std::string> http_head ;
		std::string http_body ;  
} ;
