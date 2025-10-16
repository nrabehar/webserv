#include "webserv.hpp"

std::string UI::_js;
std::string UI::_css;

UI::~UI() {}


std::string UI::getHeader(const std::string &t, const std::string &c)
{

	std::ostringstream oss;

	oss << "<!Doctype html>"
			<< "<html>\n"
			<< "<head>\n"
			<< "<meta charset=\"UTF-8\">\n"
			<< "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
			<< "<title>" << t << "</title>\n"
			<< "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/7.0.0/css/all.min.css\" />\n"
			<< "<style>\n"
			<< c << "\n</style>\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "<header class=\"header\">\n"
			<< "<div class=\"header-container\">"
			<< "<a href=\"/\" class=\"logo\"><i class=\"fa-solid fa-earth-africa\"></i><span>WEBSERV</span></a>\n"
			<< "<button class=\"icon-btn source-code\"><i class=\"fa-brands fa-github icon\"></i></button>"
			<< "</header>\n</div>\n";

	return (oss.str());
}

std::string UI::getFooter()
{
	std::ostringstream oss;

	oss << "</body>"
			<< "</html>";
	
	return (oss.str());

}

const std::string & UI::getCss()
{
	std::ostringstream oss;

	if (!_css.empty())
		return (_css);

	oss << "*,*::before,*::after{margin:0;padding:0;box-sizing:border-box}\n"
		<< "a,a:active{text-decoration:none}\n"
		<< "body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;min-height:100vh;width:100vw;display:flex;flex-direction:column;background:rgb(13,17,23);color:#9ba3b4;scroll-behavior:smooth;overflow-x:hidden}\n"
		<< ".header{width:100%;z-index:100;position:sticky;background:rgba(13,17,23,.8);backdrop-filter:blur(4px);padding:.75rem 2rem;position:sticky;top:0;z-index:100}\n"
		<< ".header-container{padding:0;height:100%;max-width:1200px;text-align:center;width:100%;display:flex;justify-content:space-between;align-items:center;margin:0 auto}\n"
		<< ".logo{display:flex;align-items:center;justify-content:center;color:#f4f4f4;font-size:1.5rem;gap:6px}\n"
		<< ".logo img{width:55px;height:55px}\n"
		<< ".icon-btn{background:transparent;outline:none;border:none;border-radius:50%;display:flex;align-items:center;justify-content:center;width:40px;height:40px}\n"
		<< ".icon-btn:hover{background:rgba(193,198,204,0.2);cursor:pointer}\n"
		<< ".source-code{width:35px;height:35px;background-color:rgba(193,198,204,0.2)}\n"
		<< ".source-code:hover{background-color:rgba(193,198,204,0.4)}\n"
		<< ".source-code img{width:25px;height:25px}\n"
		<< ".nav{display:flex}\n"
		<< ".nav-link{font-size:1rem;color:#f4f4f4}\n"
		<< ".nav-link:hover{color:#109e82}\n"
		<< ".hero{text-align:center;height:100vh;min-height:600px;width:100%;display:flex;flex-direction:column;justify-content:center;align-items:center;margin-bottom:0;animation:heroFadeIn 1.2s cubic-bezier(.77,0,.175,1) both;position:relative;overflow:hidden}\n"
		<< ".hero h1{font-size:3rem;margin-bottom:1.5rem;color: #d80529ff;font-weight:800;letter-spacing:2px;animation:heroTitleUp 1.2s cubic-bezier(.77,0,.175,1) 0.2s both;text-shadow:0 0 20px rgba(213, 67, 72, 0.5)}\n"
		<< ".hero p{font-size:1.3rem;color:#9ba3b4;max-width:600px;margin:0 auto 2rem;animation:heroTextUp 1.2s cubic-bezier(.77,0,.175,1) 0.5s both}\n"
		<< ".cta-button{display:inline-block;padding:1rem 2rem;background:linear-gradient(135deg,#109e82,#4348d5);color:#fff;text-decoration:none;border-radius:50px;font-size:1.2rem;font-weight:600;transition:transform 0.3s,box-shadow 0.3s;box-shadow:0 4px 15px rgba(16,158,130,0.4);animation:heroTextUp 1.2s cubic-bezier(.77,0,.175,1) 0.7s both}\n"
		<< ".cta-button:hover{transform:translateY(-3px);box-shadow:0 8px 25px rgba(16,158,130,0.6)}\n"
		<< ".directory-listing{background-color:#fff;border:1px solid #ddd;border-radius:8px;padding:1rem;width:100%;max-width:800px;box-shadow:0 2px 4px rgba(0,0,0,0.1);max-height:500px;overflow-y:auto;min-height:400px; margin: 0 auto;}\n"
		<< ".directory-item{display:flex;align-items:center;justify-content:space-between;padding:0.5rem 0.5rem;border-bottom:1px solid #eee;border-radius:4px}\n"
		<< ".directory-item:hover{background-color:#f0f0f0}\n"
		<< ".file-icon{display:flex;align-items:center}\n"
		<< ".file-icon i{font-size:1.5rem;color:#4CAF50;margin-right:0.5rem}\n"
		<< ".details{display:flex;font-size:0.9rem;color:#666;align-items:center;gap:1rem}\n"
		<< ".details:first-child{font-weight:600}\n"
		<< ".actions button{background:none;border:none;cursor:pointer;font-size:1.2rem;color:#666;margin-left:0.5rem;transition:color 0.3s}\n"
		<< ".actions button:hover{transform:scale(1.1)}\n"
		<< ".actions .delete-btn{color:#e74c3c}\n"
		<< ".actions .delete-btn:hover{color:#c0392b}\n"
		<< "@keyframes heroFadeIn{0%{opacity:0;transform:scale(0.98)}100%{opacity:1;transform:scale(1)}}\n"
		<< "@keyframes heroTitleUp{0%{opacity:0;transform:translateY(40px)}100%{opacity:1;transform:translateY(0)}}\n"
		<< "@keyframes heroTextUp{0%{opacity:0;transform:translateY(60px)}100%{opacity:1;transform:translateY(0)}}\n"
		<< "@keyframes fadeInUp{0%{opacity:0;transform:translateY(30px)}100%{opacity:1;transform:translateY(0)}}\n"
		<< "@keyframes pulse{0%{transform:scale(1.1)}50%{transform:scale(1.2)}100%{transform:scale(1.1)}}\n"
		<< ".container{flex:1;width:100%;display:flex;flex-direction:column;margin:0 auto}\n"
		<< ".page-header{text-align:center;padding:4rem 0 2rem}\n"
		<< ".page-header h1{font-size:2.5rem;color:#4348d5;margin-bottom:1rem}\n"
		<< ".page-header p{font-size:1.2rem;color:#9ba3b4}\n"
		<< ".footer{background:#181A20;border-top:1px solid #23272F;padding:1rem 0;text-align:center;color:#E5E7EB;margin-top:2rem}\n"
		<< ".footer-content{display:flex;align-items:center;justify-content:center;gap:0.7rem;margin-bottom:0.7rem}\n"
		<< ".footer-logo{width:32px;height:32px;vertical-align:middle}\n"
		<< ".footer-title{font-size:1.2rem;font-weight:600;color:#109E82;letter-spacing:1px}\n"
		<< ".footer-links{display:flex;justify-content:center;gap:1.5rem;margin-bottom:0.7rem}\n"
		<< ".footer-links a{color:#4348D5;font-size:1rem;transition:color 0.2s}\n"
		<< ".footer-links a:hover{color:#109E82}\n"
		<< ".footer-copy{font-size:0.95rem;color:#666;margin-top:0.5rem}";

	_css = oss.str();
	return (_css);

}

std::string UI::getErrorPage(int code, const std::string & reason)
{

	std::ostringstream oss;
	if (_css.empty())
		getCss();

	std::string title = String::str(code) + " " + reason;

	oss << getHeader(title, getCss())
			<< "<main class=\"container\">\n"
			<< getHero(title, "")
			<< "</main>\n"
			<< getFooter();

	return (oss.str());

} 

std::string UI::getHero(const std::string & t, const std::string & d)
{

	std::ostringstream oss;

	oss << "<div class=\"hero\">"
			<< 		"<h1>" << t << "</h1>"
			<< 		"<p>" << d << "</p>"
			<< 		"<a href=\"/\" class=\"cta-button\">Go to home</a>"
			<< 	"</div>\n";
	
	return (oss.str());

}

std::string UI::getDirListing(const std::string & path, const std::string & uri)
{

	std::ostringstream oss;

	oss << getHeader("Index of " + uri, getCss())
			<< "<main class=\"container\">\n"
			<< 	"<div class=\"page-header\">\n"
			<< 		"<h1>Index of " << uri << "</h1>\n"
			<< 	"</div>\n"
			<< 	"<div class=\"directory-listing\">\n";
	
	DIR * dir = opendir(path.c_str());
	if (dir)
	{

		struct dirent * entry;
		while ((entry = readdir(dir)) != NULL)
		{

			std::string name = entry->d_name;
			if (name == ".")
				continue;

			std::string type = (entry->d_type == DT_DIR) ? "Directory" : Mime::getType(name);
			std::string icon_class = (entry->d_type == DT_DIR) ? "fa fa-folder" : getFileIcon(name);
			std::string href = uri;
			if (href[href.size() - 1] != '/')
				href += "/";
			href += name;
			std::string size = (entry->d_type == DT_DIR) ? "-" : getFileSize(path + "/" + name);
			std::string mod_time = getModTime(path + "/" + name);
			std::string perm = getPermission(path + "/" + name);
			std::string actions;
			if (entry->d_type != DT_DIR)
				actions += "<button class=\"delete-btn\" title=\"Delete\"><i class=\"fa fa-trash\"></i></button>";

			oss << "<div class=\"directory-item\">"
				<< 	"<a href=\"" << href << "\" class=\"file-icon\"><i class=\"" << icon_class << "\"></i>" << name << "</a>"
				<< 	"<div class=\"details\"><span>"<< perm << "</span><span>" << type << "</span><span>" << size << "</span><span>" << mod_time << "</span></div>"
				<< 	"<div class=\"actions\">" << actions << "</div>"
				<< "</div>\n";
		}

		closedir(dir);
	}
	oss << 	"</div>\n"
			<< "</main>\n"
			<< getActionScript()
			<< getFooter();
	return (oss.str());

}

std::string UI::getModTime(const std::string & file)
{
	struct stat file_stat;
	if (stat(file.c_str(), &file_stat) == 0)
	{
		char buffer[20];
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", localtime(&file_stat.st_mtime));
		return (std::string(buffer));
	}
	return ("-");
}

std::string UI::getFileSize(const std::string & file)
{
	struct stat file_stat;
	if (stat(file.c_str(), &file_stat) == 0)
	{

		std::ostringstream oss;
		
		off_t size = file_stat.st_size;

		if (size < 1024)
			return (String::str(size) + " B");
		else if (size < 1024 * 1024)
			oss << std::fixed << std::setprecision(1) << (size / 1024.0) << " KB";
		else if (size < 1024 * 1024 * 1024)
			oss << std::fixed << std::setprecision(1) << (size / (1024.0 * 1024.0)) << " MB";
		else
			oss << std::fixed << std::setprecision(1) << (size / (1024.0 * 1024.0 * 1024.0)) << " GB";
		
		return (oss.str());

	}
	return ("-");
}

std::string UI::getFileIcon(const std::string & file)
{
	std::string ext = file.substr(file.find_last_of('.') + 1);
	if (ext == "txt" || ext == "log" || ext == "md")
		return ("fa fa-file-alt");
	else if (ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "gif" || ext == "bmp" || ext == "svg")
		return ("fa fa-file-image");
	else if (ext == "mp4" || ext == "avi" || ext == "mkv" || ext == "mov")
		return ("fa fa-file-video");
	else if (ext == "mp3" || ext == "wav" || ext == "flac")
		return ("fa fa-file-audio");
	else if (ext == "pdf")
		return ("fa fa-file-pdf");
	else if (ext == "zip" || ext == "rar" || ext == "tar" || ext == "gz")
		return ("fa fa-file-archive");
	else if (ext == "html" || ext == "htm" || ext == "css" || ext == "js")
		return ("fa fa-file-code");
	else
		return ("fa fa-file");
}

std::string UI::getPermission(const std::string & file)
{
	struct stat file_stat;
	if (stat(file.c_str(), &file_stat) == 0)
	{
		std::string perm;
		perm += (file_stat.st_mode & S_IRUSR) ? "r" : "-";
		perm += (file_stat.st_mode & S_IWUSR) ? "w" : "-";
		perm += (file_stat.st_mode & S_IXUSR) ? "x" : "-";
		perm += (file_stat.st_mode & S_IRGRP) ? "r" : "-";
		perm += (file_stat.st_mode & S_IWGRP) ? "w" : "-";
		perm += (file_stat.st_mode & S_IXGRP) ? "x" : "-";
		perm += (file_stat.st_mode & S_IROTH) ? "r" : "-";
		perm += (file_stat.st_mode & S_IWOTH) ? "w" : "-";
		perm += (file_stat.st_mode & S_IXOTH) ? "x" : "-";
		return (perm);
	}
	return ("---------");
}

const std::string & UI::getActionScript()
{

	std::ostringstream	oss;

	if (!_js.empty())
		return (_js);

	oss << "<script>\n"
		<< "function deleteFile(filePath) {\n"
		<< "    if (confirm('Are you sure you want to delete ' + filePath + '?')) {\n"
		<< "        fetch(filePath, { method: 'DELETE' })\n"
		<< "        .then(response => {\n"
		<< "            if (response.ok) {\n"
		<< "                alert('File deleted successfully');\n"
		<< "                location.reload();\n"
		<< "            } else {\n"
		<< "                alert('Failed to delete file. Status: ' + response.status);\n"
		<< "            }\n"
		<< "        })\n"
		<< "        .catch(error => {\n"
		<< "            alert('Error deleting file: ' + error.message);\n"
		<< "        });\n"
		<< "    }\n"
		<< "}\n"
		<< "document.addEventListener('DOMContentLoaded', function() {\n"
		<< "    const deleteButtons = document.querySelectorAll('.delete-btn');\n"
		<< "    deleteButtons.forEach(button => {\n"
		<< "        button.addEventListener('click', function(e) {\n"
		<< "            e.preventDefault();\n"
		<< "            const filePath = this.closest('.directory-item').querySelector('a').getAttribute('href');\n"
		<< "            deleteFile(filePath);\n"
		<< "        });\n"
		<< "    });\n"
		<< "});\n"
		<< "</script>\n";

	_js = oss.str();
	return (_js);

}
