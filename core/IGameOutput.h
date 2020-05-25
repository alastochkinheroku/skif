#ifndef __I_GAME_OUTPUT__
#define __I_GAME_OUTPUT__

namespace core
{

class IGameOutput
{
public:
   virtual std::string toString() const = 0;
};

}

#endif