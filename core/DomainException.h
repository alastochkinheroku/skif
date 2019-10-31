#ifndef __DOMAIN_EXCEPTION__
#define __DOMAIN_EXCEPTION__

#include <exception>

namespace core
{

	struct DomainException : public std::exception
	{
	};

}

#endif