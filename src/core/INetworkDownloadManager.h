#ifndef __I_NETWORK_DW_MANAGER__
#define __I_NETWORK_DW_MANAGER__

#include <string>

namespace core
{

//менеджер загрузки умеет только грузить файл с удаленного репозитория в локальную папку
class INetworkDownloadManager
{
public:
    virtual bool downloadResource(std::string net_resource, std::string local_path) = 0;
};

}

#endif