#ifndef __I_REPOSITORY_PROVIDER__
#define __I_REPOSITORY_PROVIDER__

#include <string>
#include "GameDescription.h"

namespace core
{

class IRepositoryProvider
{
public:
   virtual std::string gameFileNameFromShortId(ShortId sid) const = 0; //�������� ��� ���� �� ShortId
   virtual GameList parseOrCreateConfigFromLocal(std::string fullPath) = 0; //���������� ��� ������� ������������ ��� ��� � ��������� �����������
   virtual void installFromRemote(std::string fullPath) = 0; //����������� � ���������� ����, ��������� �� ���������� �����������
   virtual GameList updateConfigFromRemote(const GameList& glist, std::wstring remoteSpec) = 0; //�������� ������������ ��� ���� �� ������������, ��������� �� �����������
};

}

#endif