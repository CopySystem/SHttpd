#include <string>
#include <vector>
#include <map>

#include <memory>
#include <chrono>

class Task {
	public :
		Task(int) ;
		~Task() ;
		void run() ; 			// 运行Task，解析HTTP请求
		void read_lines() ;		// 读取HTTP请求头中的一行
		void read_property() ;	// 读取属性：url 请求方法 版本 .. .. ..
		std::string request_url() ;
		std::string http_method() ;
		std::string http_vertion() ;
		void response() ;
		std::string create_header(int) ;
	private :
		int cli_socfd = -1    ;
		std::vector<std::string> http_head ;
		std::string http_body  ;  
		std::vector<int> space ;
		std::map<std::string,std::string> other ;

		double IOTime = 0.0 ;
		double CPUTime = 0.0 ;

} ;
