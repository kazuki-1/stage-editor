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
    Insert("Empty", L"./Data/Audio/empty_sound.wav");
    return S_OK;



}

/*---------------------------------------------AUDIOENGINE Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform functions </para>
/// <para> 垈･ﾕ･・`･爨ﾋｺﾓｳｹ </para>
/// </summary>
void AUDIOENGINE::Execute()
{
    for (auto& a : audios)
        a.second->Execute();
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
/// <para> AUDIOを生成し、マップに登録</para>
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
    for (auto& a : audios)
    {
        if (name == a.first)
            return a.second;
    }
    return audios.find("Empty")->second;
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

    DWORD Type{}, dataSize{}, RIFFSize{}, fileType{}, bytesRead{}, offset{};

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
    isPlaying = true;
    //sourceVoice->Start();
    //sourceVoice->SetVolume(volume);
    //stateMachine->FadeTo(1.0f, 1.0f);

}

/*---------------------------------------------AUDIO FadeInAndPlay()----------------------------------------------------*/

void AUDIO::FadeInAndPlay(float fade_time)
{
    Play();
    FadeIn(fade_time);
}

/*---------------------------------------------AUDIO FadeOutAndPause()----------------------------------------------------*/

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
    stateMachine->FadeOut(1.0f);

    //sourceVoice->Stop();
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
    FadeTo(1.0f, fade_time);
}

/*---------------------------------------------AUDIO FadeOut()----------------------------------------------------*/

void AUDIO::FadeOut(float fade_time)
{
    stateMachine->FadeOut(fade_time);
}

/*---------------------------------------------AUDIO DisableLoop()----------------------------------------------------*/

void AUDIO::FadeTo(float fade_vol, float fade_time)
{
    stateMachine->FadeTo(fade_time, fade_vol);
    // Check if audio is playing
    //if (!isPlaying || volume == fade_vol)
    //    return;

    //// Calculate increment rate
    //float increment = { 1.0f / 60.0f / fade_time };
    //
    //
    //if (fabsf(volume - fade_vol) <= 0.01f)
    //{
    //    volume = fade_vol;
    //    return;
    //}   
    //volume <= fade_vol ? volume += increment : volume -= increment;
    //
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
    sourceVoice->SetVolume(volume);
}

/*---------------------------------------------AUDIO SetBuffer()----------------------------------------------------*/

void AUDIO::SetBuffer(XAUDIO2_BUFFER buffer)
{
    this->buffer = buffer;
}

/*---------------------------------------------AUDIO PerformDucking()----------------------------------------------------*/
/// <summary>
/// <para> Performs ducking and fades the volume to 0.3f</para>
/// <para>･ﾀ･ﾃ･ｭ･ｰ､ﾐ､､｡｢ﾒｿ､ｰ｣ｮ｣ｳ､ﾋﾕ{ﾕ訷ｹ､・</para>
/// </summary>
void AUDIO::PerformDucking(float fade_vol)
{
    if (isDucking)
        return;
    volume_before_ducking = volume;
    isDucking = true;
    if (isPlaying)
    {
        FadeTo(fade_vol, 0.5f);
    }
}

/*---------------------------------------------AUDIO StopDucking()----------------------------------------------------*/
/// <summary>
/// <para> stops the ducking state</para>
/// <para> ･ﾀ･ﾃ･ｭ･ｰ､ﾐﾖｹ､ｹ､・/para>
/// </summary>
void AUDIO::StopDucking()
{
    if (!isDucking)
        return;
    isDucking = false;
    if (isPlaying)
        FadeTo(volume_before_ducking, 0.5f);
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
    ReadChunk(h, &fileType, sizeof(DWORD), dwChunkPos);
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
    stateMachine = std::make_shared<AUDIO_STATES::AudioStateMachine>(this);
    stateMachine->Initialize();
    return hr;
}

/*---------------------------------------------AUDIO Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform functions </para>
/// <para> 垈･ﾕ･・`･爨ﾋｺﾓｳｹ </para>
/// </summary>
void AUDIO::Execute()
{
    stateMachine->Execute();
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

/*---------------------------------------------AUDIO IsDucking()----------------------------------------------------*/

bool AUDIO::IsDucking()
{
    return isDucking;
}

/*---------------------------------------------AUDIO FilePath()----------------------------------------------------*/

std::wstring AUDIO::FilePath()
{
    return file_path;
}