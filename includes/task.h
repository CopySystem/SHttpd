#include <string>
#include <vector>
#include <map>

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
		std::string header() ;
	private :
		int cli_socfd = -1    ;
		const std::string base_path = "www" ;
		std::vector<std::string> http_head ;
		std::string http_body  ;  
		std::vector<int> space ;
		std::map<std::string,std::string> other ;
} ;
