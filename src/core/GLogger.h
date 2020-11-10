#ifndef __GLOGGER_H__
#define __GLOGGER_H__

#include <string>
#include <sstream>
#include <ostream>

namespace core
{
	//������ �����������
	enum LOG_LEVEL {
		LOG_LEVEL_INFO,
		LOG_LEVEL_ERROR
	};

	//��������� ����������� ������
	//�������� ������ ���� ���������� ���� ��� �������� ����������� �������
	//�� ���� ��� ���� ������� �����������
	class GLogger
	{
	public:
		static GLogger& get(); //�������� ��������� ������

		//Templated operator>> that uses the std::ostream: Everything that has defined 
		//an operator<< for the std::ostream (Everithing "printable" with std::cout 
		//and its colleages) can use this function.    
		template<typename T>
		GLogger& operator<< (const T& data)
		{
			std::stringstream outStream;
			outStream << data;
			log(outStream.str());
			return *this;
		}

		virtual void log(std::string info, LOG_LEVEL lvl = LOG_LEVEL_INFO) = 0; //�������� ���
	protected:
		//���������� �������� �������
		void setCurrentLogger(GLogger* logger);
	private:
		static GLogger* _currentLogger;
	};

}

#endif