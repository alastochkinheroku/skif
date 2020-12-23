#ifndef __TOLK_DLL_WRAPPER__
#define __TOLK_DLL_WRAPPER__

#include "ITolk.h"

class TolkDllWrapper : public core::ITolk
{
public:
	TolkDllWrapper();
	bool Load();
	const wchar_t* DetectScreenReader();
	bool IsSpeaking() const; 
	bool Speak(const wchar_t* str); 
	void Silence();
};

#endif