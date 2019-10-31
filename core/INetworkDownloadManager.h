#ifndef __I_NETWORK_DW_MANAGER__
#define __I_NETWORK_DW_MANAGER__

class INetworkDownloadManager
{
public:
     virtual std::string getGameList(std::string httpResourseWithXml) = 0;
     virtual std::string parseXmlToGameListJson(std::string xml) const = 0;
     virtual std::filesystem::path downloadToTemp(int gameId) = 0;
};

#endif