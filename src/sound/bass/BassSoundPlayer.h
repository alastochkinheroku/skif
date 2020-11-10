#ifndef __BASS_SOUND_PLAYER__
#define __BASS_SOUND_PLAYER__

#include "IUserOutputNotifier.h"

class BassSoundPlayer : public core::ISoundPlayer 
{
public:
	BassSoundPlayer(bool server_cast = false, //������ ����� ������
					std::string port = "8001" //���� ��� �������
	);
    void init(); //������������� ������
	void play_sound(int layer, std::string title, char* data, size_t data_size, bool repeat, int volume);
	void stop_sound(int layer);
private:
	const bool _is_server;
	const std::string _port;
	bool _init;
};

#endif