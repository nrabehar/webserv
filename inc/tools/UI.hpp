#ifndef UI_HPP
#define UI_HPP

#include <string>
#include <dirent.h>

class UI
{

	private:

		static std::string _js;
		static std::string _css;
		static std::string _html_header;
		static std::string _html_footer;
		static std::string _html_nav;		

	public:

		~UI();
		static std::string getErrorPage(int code, const std::string & reason);
		static std::string getDirListing(const std::string & path, const std::string & uri);

	private:


		UI();
		UI(const UI &);
		UI & operator=(const UI &);

	  static std::string getCss();
		static std::string getHeader(const std::string & t, const std::string & c);
		static std::string getHero(const std::string & t, const std::string & d);
		static std::string getFooter();

		static std::string getModTime(const std::string & file);
		static std::string getFileSize(const std::string & file);
		static std::string getFileIcon(const std::string & file);
		static std::string getPermission(const std::string & file);

		static std::string getActionScript();

};


#endif // UI_HPP