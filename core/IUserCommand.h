#ifndef __I_USER_COMMAND__
#define __I_USER_COMMAND__

namespace core
{

class IUserCommand
{
public:
   virtual std::string toJson() const = 0;
};

}

#endif