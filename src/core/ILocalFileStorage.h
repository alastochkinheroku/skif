#ifndef __I_LOCAL_FILE_STORAGE__
#define __I_LOCAL_FILE_STORAGE__

#include <string>
#include "GameDescription.h"

namespace core
{

//��������� ��������� ������������, ��������� ���� � ����� ������ ���� �� ID, ����� ��������� ������������� ��� ��� ������������ �������
class ILocalFileStorage
{
public:
   virtual bool haveGame(ShortId sid) const = 0;
   virtual std::string getFilePath(ShortId sid) const = 0; //�������� ���� � ���� �� ShortId
   virtual bool installFromRemote(ShortId sid, std::string fullPath) = 0; //����������� � ���������� ����, ��������� �� ���������� �����������
};

}

#endif