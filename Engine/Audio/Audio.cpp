
// Includes
//=========

#include <Engine/Audio/Audio.h>

#include <memory>
#include <xaudio2.h>


// Static Data
//============

namespace
{
	std::shared_ptr<IXAudio2> s_XAudio2;
	std::shared_ptr<IXAudio2MasteringVoice> s_masterVoice;
}




eae6320::cResult eae6320::Audio::Initialize()
{
	cResult result = Results::Success;

	HRESULT hr;
	IXAudio2* XAudio2 = nullptr;
	IXAudio2MasteringVoice* masterVoice = nullptr;
	
	if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		return result;

	if (FAILED(hr = XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return result;

	if (FAILED(hr = XAudio2->CreateMasteringVoice(&masterVoice)))
		return result;

	s_XAudio2 = std::shared_ptr<IXAudio2>(XAudio2);
	s_masterVoice = std::shared_ptr<IXAudio2MasteringVoice>(masterVoice);

	return result;
}



eae6320::cResult eae6320::Audio::CleanUp()
{

	if (s_XAudio2 != nullptr)
	{
		s_XAudio2->Release();
		s_XAudio2.reset();
	}

	if (s_masterVoice != nullptr)
	{
		s_masterVoice.reset();
	}

	return Results::Success;
}
