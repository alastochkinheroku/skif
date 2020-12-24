#include "BassSoundPlayer.h"
#include "GLogger.h"
#include "bass.h"
#include "bassmidi.h"
#include "bassmix.h"
#include "bassenc.h"
#include "bassenc_ogg.h"

BassSoundPlayer::BassSoundPlayer(bool server_cast, std::string port):
	_is_server(server_cast),
	_port(port),
	_init(false)
{
}

static HSTREAM mixer;

void BassSoundPlayer::init()
{
	if (BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL) == 0) {
		core::GLogger::get()<< "Не удалось инициализировать BASS.";
		return;
	}

	if (_is_server)
	{
		mixer = BASS_Mixer_StreamCreate(44100, 2, 0);
		if (!mixer)
		{
			core::GLogger::get() << "Cannot create mixer, error: " << BASS_ErrorGetCode();
			return;
		}

		HENCODE encoder = BASS_Encode_OGG_Start(mixer, (const char*)NULL, 0, NULL, NULL);
		if (!encoder)
		{
			core::GLogger::get() << "Cannot create encoder, error: " << BASS_ErrorGetCode();
			return;
		}

		int port = BASS_Encode_ServerInit(encoder, _port.data(), 64000, 64000, 0, NULL, NULL); // start the server
		if (port == 0)
		{
			core::GLogger::get() << "Cannot init server, error: " << BASS_ErrorGetCode();
			return;
		}
		//чтобы не играл вживую
		BASS_ChannelPlay(mixer, FALSE);
		BASS_ChannelSetAttribute(mixer, BASS_ATTRIB_VOL, 0.0f);

		core::GLogger::get() << "Init server on port: " << _port;
	}

	//Подключение плагинов
	BASS_PluginLoad("bassmidi.dll", 0);
	//Загрузка звукового шрифта
	HSOUNDFONT newfont = BASS_MIDI_FontInit("chorium.sf2", 0);
	if (newfont) {
		BASS_MIDI_FONT sf;
		sf.font = newfont;
		sf.preset = -1; // use all presets
		sf.bank = 0; // use default bank(s)
		BASS_MIDI_StreamSetFonts(0, &sf, 1);    // set default soundfont
		//BASS_MIDI_FontFree(font);             // free old soundfont
	}
	
	_init = true;
}

#define MAX_SND_CH 4
static HCHANNEL sound_channel[MAX_SND_CH] = { 0,0,0,0 };

static HSTREAM sound_streams[MAX_SND_CH] = { 0,0,0,0 };

static void setLocalSoundLevel(int n_chan, int volume)
{
	if (n_chan >= 0 && n_chan < MAX_SND_CH)
	{
		if (sound_channel[n_chan])
		{
			if (volume < 0) volume = 0;
			if (volume > 100) volume = 100;
			float local_snd_lvl = volume / 100.0f;
			BASS_ChannelSetAttribute(sound_channel[n_chan], BASS_ATTRIB_VOL, local_snd_lvl);
		}
	}
}

void BassSoundPlayer::play_sound(int layer, std::string title, char* data, size_t data_size, bool repeat, int volume)
{
	if (!_init) return;
	if (!_is_server)
	{
		if (sound_channel[layer]) {
			BASS_ChannelStop(sound_channel[layer]);
			sound_channel[layer] = 0;
		}
		HSAMPLE samp = BASS_SampleLoad(TRUE, data, 0, data_size, 1, repeat ? BASS_SAMPLE_LOOP : 0);
		if (samp)
		{
			sound_channel[layer] = BASS_SampleGetChannel(samp, FALSE);
			setLocalSoundLevel(layer, volume);
			if (sound_channel[layer]) {
				BASS_ChannelPlay(sound_channel[layer], FALSE);
			}
		}
		else
		{
			int err_bass = BASS_ErrorGetCode();
			core::GLogger::get() << "Ошибка BASS " << err_bass << " для файла " << title;
		}
	}
	else
	{
		if (sound_streams[layer]) {
			int res = BASS_Mixer_ChannelRemove(sound_streams[layer]);
			if (res != TRUE)
			{
				core::GLogger::get() << "Cannot remove chan from mixer, error: " << BASS_ErrorGetCode();
				return;
			}

			sound_streams[layer] = 0;
		}

		sound_streams[layer] = BASS_StreamCreateFile(TRUE, data, 0, data_size, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT | (repeat ? BASS_SAMPLE_LOOP : 0));
		if (!sound_streams[layer])
		{
			core::GLogger::get() << "Cannot create stream, error: " << BASS_ErrorGetCode();
			return;
		}

		int res = BASS_Mixer_StreamAddChannel(mixer, sound_streams[layer], 0);
		if (res != TRUE)
		{
			core::GLogger::get() << "Cannot add chan to mixer, error: " << BASS_ErrorGetCode();
			return;
		}
	}
}

void BassSoundPlayer::stop_sound(int layer)
{
	if (!_init) return;
	if (!_is_server)
	{
		if (sound_channel[layer]) {
			BASS_ChannelStop(sound_channel[layer]);
			sound_channel[layer] = 0;
		}
	}
	else
	{
		if (sound_streams[layer])
		{
			int res = BASS_Mixer_ChannelRemove(sound_streams[layer]);
			if (res != TRUE)
			{
				core::GLogger::get() << "Cannot remove chan from mixer, error: " << BASS_ErrorGetCode();
				return;
			}

			BASS_ChannelStop(sound_streams[layer]);
			sound_streams[layer] = 0;
		}
	}
}

void BassSoundPlayer::stop_all_sound()
{
	for (auto n_chan = 0; n_chan < MAX_SND_CH; n_chan++)
	{
		stop_sound(n_chan);
	}
}
