#include "Audio.h"
#include <assert.h>
#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------AUDIOENGINE Class-----------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------AUDIOENGINE Initialize()----------------------------------------------------*/
/// <summary>
/// Initializes the audioengine, initializing the IXAudio2 and IXAudio2MasteringVoice
/// </summary>
/// <returns></returns>
HRESULT AUDIOENGINE::Initialize()
{


    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    assert(hr == S_OK);
    hr = XAudio2Create(xAudio.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
    assert(hr == S_OK);

    hr = xAudio->CreateMasteringVoice(&masteringVoice, XAUDIO2_DEFAULT_CHANNELS, 48000);
    assert(hr == S_OK);


    return S_OK;
    


}

/*---------------------------------------------AUDIOENGINE Finalize()----------------------------------------------------*/
/// <summary>
/// Finalizes the class, generally not needed
/// </summary>
void AUDIOENGINE::Finalize()
{
    for (auto& a : audios)
        a.second->Finalize();
    //delete SourceVoice;
    //delete MasteringVoice;
}

/*---------------------------------------------AUDIOENGINE Insert()----------------------------------------------------*/
/// <summary>
/// <para> Create an AUDIO object and insert it into the map </para>
/// <para> AUDIOÇê∂ê¨ÇµÅAÉ}ÉbÉvÇ…ìoò^</para>
/// </summary>
/// <param name="name"> : Name of audio file</param>
/// <param name="file_path"> : File path of audio file</param>
void AUDIOENGINE::Insert(std::string name, std::wstring file_path)
{
    // Check if instance exists in map
    for (auto& a : audios)
    {
        if (a.first == name && a.second->FilePath() == file_path)
            return;

    }
    // Create and insert
    std::shared_ptr<AUDIO>audio = std::make_shared<AUDIO>(file_path);
    //audio->Populate(file_path);
    audios.insert(std::make_pair(name, audio));
    audios.find(name)->second->Initialize();
}

/*---------------------------------------------AUDIOENGINE MasteringVoice()----------------------------------------------------*/

IXAudio2MasteringVoice* AUDIOENGINE::MasteringVoice()
{
    return masteringVoice;
}

/*---------------------------------------------AUDIOENGINE XAudio()----------------------------------------------------*/

ComPtr<IXAudio2>AUDIOENGINE::XAudio()
{
    return xAudio;
}

/*---------------------------------------------AUDIOENGINE Retrieve()----------------------------------------------------*/

std::shared_ptr<AUDIO>AUDIOENGINE::Retrieve(std::string name)
{
    return audios.find(name)->second;
}

/*---------------------------------------------AUDIOENGINE Audios()----------------------------------------------------*/

std::map<std::string, std::shared_ptr<AUDIO>>AUDIOENGINE::Audios()
{
    return audios;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------AUDIO Class----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------AUDIO Contructor----------------------------------------------------*/

AUDIO::AUDIO(std::wstring path) : file_path(path)
{

}

/*---------------------------------------------AUDIO FindChunk()----------------------------------------------------*/

HRESULT AUDIO::FindChunk(HANDLE h, DWORD fourcc, DWORD& cSize, DWORD& cDataPosition)
{
    HRESULT hr{ S_OK };
    if (INVALID_SET_FILE_POINTER == SetFilePointer(h, 0, 0, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD Type, dataSize, RIFFSize, fileType{}, bytesRead{}, offset{};

    while (hr == S_OK)
    {
        DWORD read;
        if (!ReadFile(h, &Type, sizeof(DWORD), &read, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());
        if (!ReadFile(h, &dataSize, sizeof(DWORD), &read, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (Type)
        {
        case fourccRIFF:
            RIFFSize = dataSize;
            dataSize = 4;
            if (!ReadFile(h, &fileType, sizeof(DWORD), &read, 0))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;
        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(h, dataSize, NULL, FILE_CURRENT))
                hr = HRESULT_FROM_WIN32(GetLastError());
        }
        offset += sizeof(DWORD) * 2;
        if (Type == fourcc)
        {
            cSize = dataSize;
            cDataPosition = offset;
            return S_OK;

        }

        offset += dataSize;
        if (bytesRead >= dataSize)
            return E_FAIL;
    }
    return S_OK;
}

/*---------------------------------------------AUDIO ReadChunk()----------------------------------------------------*/

HRESULT AUDIO::ReadChunk(HANDLE h, void* buffer, DWORD buffer_Size, DWORD offset)
{
    HRESULT hr{ S_OK };

    if (SetFilePointer(h, offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        assert(!"Error");
    
    DWORD read;
    if (!ReadFile(h, buffer, buffer_Size, &read, 0))
        assert(!"Error");
    return hr;
}

/*---------------------------------------------AUDIO Play()----------------------------------------------------*/

void AUDIO::Play()
{
    if (isPlaying)
        return;
    HRESULT hr = sourceVoice->SubmitSourceBuffer(&buffer);
    assert(hr == S_OK);
    sourceVoice->Start();
    sourceVoice->SetVolume(volume);
    isPlaying = true;

}

/*---------------------------------------------AUDIO FadeInAndPlay()----------------------------------------------------*/

void AUDIO::FadeInAndPlay(float fade_time)
{
    Play();
    FadeIn(fade_time);
}

void AUDIO::FadeOutAndPause(float fade_time)
{
    FadeOut(1.0f);
    if (volume <= 0.0f)
        Stop();
}

/*---------------------------------------------AUDIO Stop()----------------------------------------------------*/

void AUDIO::Stop()
{
    if (!isPlaying)
        return;
    sourceVoice->Stop();
    //sourceVoice->DestroyVoice();
    isPlaying = false;
}

/*---------------------------------------------AUDIO Finalize()----------------------------------------------------*/

void AUDIO::Finalize()
{
    sourceVoice->DestroyVoice();
}

/*---------------------------------------------AUDIO Loop()----------------------------------------------------*/

void AUDIO::Loop(UINT loopCount)
{
    buffer.LoopCount = loopCount;
}

/*---------------------------------------------AUDIO Loop()----------------------------------------------------*/

void AUDIO::LoopInRange(UINT begin, UINT end) 
{
    buffer.LoopBegin = begin * format.Samples.wSamplesPerBlock;
    buffer.LoopLength = end * format.Samples.wSamplesPerBlock;
    buffer.PlayLength = buffer.LoopLength;
}

/*---------------------------------------------AUDIO FadeIn()----------------------------------------------------*/

void AUDIO::FadeIn(float fade_time)
{
   
    if (volume >= 1.0f)
        return;
    float rate = { 1.0f / 60.0f / fade_time };
    
    volume += rate;
    volume = min(volume, 1.0f);
    sourceVoice->SetVolume(volume);
}

/*---------------------------------------------AUDIO FadeOut()----------------------------------------------------*/

void AUDIO::FadeOut(float fade_time)
{
    if (volume <= 0.0f)
        return;
    float rate = { 1.0f / 60.0f / fade_time };
    volume -= rate;
    volume = max(volume, 0.0f);
    sourceVoice->SetVolume(volume);
    if (volume <= 0.0f)
        Stop();

}

/*---------------------------------------------AUDIO DisableLoop()----------------------------------------------------*/

void AUDIO::DisableLoop()
{
    buffer.LoopBegin = buffer.LoopLength = buffer.LoopCount = 0;
}

/*---------------------------------------------AUDIO SetVolume()----------------------------------------------------*/

void AUDIO::SetVolume(float vol)
{
    volume = vol;
}

/*---------------------------------------------AUDIO Initialize()----------------------------------------------------*/

HRESULT AUDIO::Initialize()
{
    HANDLE h = CreateFile(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (INVALID_HANDLE_VALUE == h)
        assert(!GetLastError());
    if (INVALID_SET_FILE_POINTER == SetFilePointer(h, 0, 0, FILE_BEGIN))
        assert(!GetLastError());
    DWORD dwChunkSize, dwChunkPos;
    FindChunk(h, fourccRIFF, dwChunkSize, dwChunkPos);
    DWORD fileType;
    ReadChunk(h, & fileType, sizeof(DWORD), dwChunkPos);
    if (fileType != fourccWAVE)
        assert(!"Wrong Format");
    FindChunk(h, fourccFMT, dwChunkSize, dwChunkPos);
    ReadChunk(h, &format, dwChunkSize, dwChunkPos);

    FindChunk(h, fourccDATA, dwChunkSize, dwChunkPos);
    BYTE* buffer = new BYTE[dwChunkSize];
    ReadChunk(h, buffer, dwChunkSize, dwChunkPos);


    XAUDIO2_BUFFER buf{};
    buf.AudioBytes = dwChunkSize;
    buf.pAudioData = buffer;
    buf.Flags = XAUDIO2_END_OF_STREAM;
    HRESULT hr = AUDIOENGINE::Instance()->XAudio()->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&format);
    assert(hr == S_OK);
    this->buffer = buf;
    //delete[] buffer;
    //return buf;
    return hr;
}

/*---------------------------------------------AUDIO Volume()----------------------------------------------------*/

float AUDIO::Volume()
{
    return volume;
}


/*---------------------------------------------AUDIO Buffer()----------------------------------------------------*/

XAUDIO2_BUFFER AUDIO::Buffer()
{
    return buffer;
}

/*---------------------------------------------AUDIO SourceVoice()----------------------------------------------------*/

IXAudio2SourceVoice* AUDIO::SourceVoice()
{
    return sourceVoice;
}

/*---------------------------------------------AUDIO IsPlaying()----------------------------------------------------*/

bool AUDIO::IsPlaying()
{
    return isPlaying;
}

/*---------------------------------------------AUDIO FilePath()----------------------------------------------------*/

std::wstring AUDIO::FilePath()
{
    return file_path;
}