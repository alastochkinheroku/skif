#include "tolk_dll.h"
#include "Tolk.h"

TolkDllWrapper::TolkDllWrapper()
{
}

bool TolkDllWrapper::Load()
{
	Tolk_Load();
	//попытка вывода в SAPI
	Tolk_TrySAPI(true);
	return Tolk_IsLoaded();
}

const wchar_t* TolkDllWrapper::DetectScreenReader()
{
	return Tolk_DetectScreenReader();
}

bool TolkDllWrapper::IsSpeaking() const
{
	return Tolk_IsSpeaking();
}

bool TolkDllWrapper::Speak(const wchar_t* str)
{
	return Tolk_Output(str);
}

void TolkDllWrapper::Silence()
{
	Tolk_Silence();
}
