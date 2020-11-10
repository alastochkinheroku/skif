#ifndef __DOMAIN_EXCEPTION__
#define __DOMAIN_EXCEPTION__

#include <stdexcept>

namespace core
{

	class DomainException : public std::logic_error
	{
	public:
		explicit DomainException(const std::string& msg) : std::logic_error(msg) {}
	};

}

#endif