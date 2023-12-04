
// Includes
//=========

#include <Engine/Audio/Audio.h>
#include <Engine/UserOutput/UserOutput.h>

#include <atlstr.h>
#include <memory>
#include <xaudio2.h>



// Type Define
//============

#ifdef _XBOX 
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif



// Static Data
//============

namespace
{
	std::shared_ptr<IXAudio2> s_XAudio2;
	std::shared_ptr<IXAudio2MasteringVoice> s_masterVoice;
}


// Helper Declarations
//============

namespace
{
    HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);

    HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
}


// Interface
//============

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
	}


	return Results::Success;
}


void eae6320::Audio::Play(const char* fileName)
{
    WAVEFORMATEXTENSIBLE wfx = { 0 };
    XAUDIO2_BUFFER buffer = { 0 };

    const TCHAR* strFileName = TEXT("data/audios/maintheme.wav");

    HANDLE hFile = CreateFile(
        strFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        auto result = HRESULT_FROM_WIN32(GetLastError());
    }

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    {
        auto result = HRESULT_FROM_WIN32(GetLastError());
    }

    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //check the file type, should be fourccWAVE or 'XWMA'
    FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
    if (filetype != fourccWAVE)
    {
        UserOutput::ConsolePrint("Audio System Fail! \n");
        return;
    }

    FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

    FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

    buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
    buffer.pAudioData = pDataBuffer;  //buffer containing audio data
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    HRESULT hr;
    IXAudio2SourceVoice* pSourceVoice;
    if (FAILED(hr = s_XAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx)))
    {
        UserOutput::ConsolePrint("Audio System Fail! \n");
    }

    if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
    {
        UserOutput::ConsolePrint("Audio System Fail! \n");
    }

    if (FAILED(hr = pSourceVoice->Start(0)))
    {
        UserOutput::ConsolePrint("Audio System Fail! \n");
    }
}


// Helper Definitions
//============

namespace
{

    HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
    {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());
        DWORD dwChunkType;
        DWORD dwChunkDataSize;
        DWORD dwRIFFDataSize = 0;
        DWORD dwFileType;
        DWORD bytesRead = 0;
        DWORD dwOffset = 0;
        while (hr == S_OK)
        {
            DWORD dwRead;
            if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            switch (dwChunkType)
            {
            case fourccRIFF:
                dwRIFFDataSize = dwChunkDataSize;
                dwChunkDataSize = 4;
                if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                    hr = HRESULT_FROM_WIN32(GetLastError());
                break;
            default:
                if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                    return HRESULT_FROM_WIN32(GetLastError());
            }
            dwOffset += sizeof(DWORD) * 2;
            if (dwChunkType == fourcc)
            {
                dwChunkSize = dwChunkDataSize;
                dwChunkDataPosition = dwOffset;
                return S_OK;
            }
            dwOffset += dwChunkDataSize;
            if (bytesRead >= dwRIFFDataSize) return S_FALSE;
        }
        return S_OK;
    }


    HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
    {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());
        DWORD dwRead;
        if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

}