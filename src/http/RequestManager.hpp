#ifndef	REQUEST_MANAGER_HPP
#define	REQUEST_MANAGER_HPP

#include "HttpRequest.hpp"
#include <map>

class	RequestManager
{

	private:

		struct ReqState
		{
			bool	_is_complete;
			std::string	_req_buf;
		};

		std::map<int, ReqState> _reqs;

	public:

		RequestManager();
		~RequestManager();

		bool	isReqComplete(int fd) const;
		void	addReqData(int fd, const char *data, size_t size);


	private:

		RequestManager(const RequestManager &);
		RequestManager	&operator=(const RequestManager &);

};


#endif