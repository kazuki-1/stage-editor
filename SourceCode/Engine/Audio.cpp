#include "Audio.h"
#include <x3daudio.h>
#include <assert.h>
#include <vector>
#include "IMGUI.h"
#define SAMPLING_RATE 48000
#define OBTUSE_ANGLE Math::ToRadians(180)
//#pragma comment(lib, "x3daudio")
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
/*---------------------------------------------AudioEngine Class-----------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------AudioEngine Initialize()----------------------------------------------------*/
/// <summary>
/// Initializes the audioengine, initializing the IXAudio2 and IXAudio2MasteringVoice
/// </summary>
/// <returns></returns>
HRESULT AudioEngine::Initialize()
{
    

    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    assert(hr == S_OK);
    hr = XAudio2Create(xAudio.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
    assert(hr == S_OK);



    hr = xAudio->CreateMasteringVoice(&masteringVoice);
    assert(hr == S_OK);
    Insert("Empty", L"./Data/Audio/empty_sound.wav");


    // Perform 3DAudio intiailization
    XAUDIO2_VOICE_DETAILS details;
    DWORD dwChannelMask;
    hr = masteringVoice->GetChannelMask(&dwChannelMask);
    masteringVoice->GetVoiceDetails(&details);

    dspSettings.SrcChannelCount = INPUT_CHANNELS;
    nChannels = dspSettings.DstChannelCount = details.InputChannels;
    dspSettings.pMatrixCoefficients = matrixCoefficients;
    
    hr = X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, x3d_handle);
    assert(hr == S_OK);
    

    return S_OK;



}

/*---------------------------------------------AudioEngine Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform functions </para>
/// <para> 垈･ﾕ･・`･爨ﾋｺﾓｳｹ </para>
/// </summary>
void AudioEngine::Execute()
{
    for (auto& a : audios)
        a.second->Execute();
}

/*---------------------------------------------AudioEngine Finalize()----------------------------------------------------*/
/// <summary>
/// Finalizes the class, generally not needed
/// </summary>
void AudioEngine::Finalize()
{
    for (auto& a : audios)
        a.second->Finalize();
    //delete SourceVoice;
    //delete MasteringVoice;
}

/*---------------------------------------------AudioEngine Insert()----------------------------------------------------*/
/// <summary>
/// <para> Create an AUDIO object and insert it into the map </para>
/// <para> AUDIOを生成し、マップに登録</para>
/// </summary>
/// <param name="name"> : Name of audio file</param>
/// <param name="file_path"> : File path of audio file</param>
void AudioEngine::Insert(std::string name, std::wstring file_path)
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

/*---------------------------------------------AudioEngine MasteringVoice()----------------------------------------------------*/

IXAudio2MasteringVoice* AudioEngine::MasteringVoice()
{
    return masteringVoice;
}

/*---------------------------------------------AudioEngine XAudio()----------------------------------------------------*/

ComPtr<IXAudio2>AudioEngine::XAudio()
{
    return xAudio;
}

/*---------------------------------------------AudioEngine Retrieve()----------------------------------------------------*/

std::shared_ptr<AUDIO>AudioEngine::Retrieve(std::string name)
{
    for (auto& a : audios)
    {
        if (name == a.first)
            return a.second;
    }
    return audios.find("Empty")->second;
}

/*---------------------------------------------AudioEngine Audios()----------------------------------------------------*/

std::map<std::string, std::shared_ptr<AUDIO>>AudioEngine::Audios()
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
    //stateMachine->FadeOut(1.0f);
    sourceVoice->Stop();

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
/// </summary>
void AUDIO::PerformDucking(float fade_vol)
{
    // Exits if not playing or is already ducking
    if (!isPlaying)
        return;
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
    // File creation
    HANDLE h = CreateFile(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (INVALID_HANDLE_VALUE == h)
        assert(!GetLastError());
    if (INVALID_SET_FILE_POINTER == SetFilePointer(h, 0, 0, FILE_BEGIN))
        assert(!GetLastError());
    DWORD dwChunkSize, dwChunkPos;

    // Riff header searching
    FindChunk(h, fourccRIFF, dwChunkSize, dwChunkPos);
    DWORD fileType;
    ReadChunk(h, &fileType, sizeof(DWORD), dwChunkPos);
    if (fileType != fourccWAVE)
        assert(!"Wrong Format");

    // FMT header searching
    FindChunk(h, fourccFMT, dwChunkSize, dwChunkPos);
    ReadChunk(h, &format, dwChunkSize, dwChunkPos);

    // Data header searching and insertion
    FindChunk(h, fourccDATA, dwChunkSize, dwChunkPos);
    BYTE* buffer = new BYTE[dwChunkSize];
    ReadChunk(h, buffer, dwChunkSize, dwChunkPos);

    XAUDIO2_BUFFER buf{};
    buf.AudioBytes = dwChunkSize;
    buf.pAudioData = buffer;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    XAUDIO2_SEND_DESCRIPTOR descriptor[1]{};
    descriptor[0].Flags = XAUDIO2_SEND_USEFILTER;
    descriptor[0].pOutputVoice = AudioEngine::Instance()->masteringVoice;

    XAUDIO2_VOICE_SENDS sendList{ 1, descriptor };

    HRESULT hr = AudioEngine::Instance()->XAudio()->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&format, 0, 2.0f, 0, &sendList);
    assert(hr == S_OK);
    this->buffer = buf;

    // AudioStateMachine initialization
    stateMachine = std::make_shared<AUDIO_STATES::AudioStateMachine>(this);
    stateMachine->Initialize();

    

    return hr;
}
float debug_volume[2];
/*---------------------------------------------AUDIO Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform functions </para>
/// <para> 垈･ﾕ･・`･爨ﾋｺﾓｳｹ </para>
/// </summary>
void AUDIO::Execute()
{
    stateMachine->Execute();

    // Perform 3DAudio calculations if emitter is targeted
    if (emitter)
    {
        if (isPlaying) {
            DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX;
            AudioEngine* audioEngine = AudioEngine::Instance();
            float* volumes = CalculateChannelVolumes(*audioEmitter, *audioEngine->audioListener);
            HRESULT hr = E_FAIL;
            hr = sourceVoice->SetOutputMatrix(audioEngine->masteringVoice, INPUT_CHANNELS, audioEngine->nChannels, volumes);
            int a = 0;
        }
    }
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

/*---------------------------------------------AUDIO RenderDebug()----------------------------------------------------*/

void AUDIO::RenderDebug()
{
    // ImGui::Begin("Channel volumes");
    // ImGui::InputFloat2("Channels", debug_volume);
    // ImGui::End();
}

/*---------------------------------------------AUDIO CalculateChannelVolumes()----------------------------------------------------*/
/// <summary>
/// Call this to calculate the volume for each channels
/// </summary>
/// <param name="channel_num"> : Number of channels</param>
/// <returns></returns>
float* AUDIO::CalculateChannelVolumes(AudioEmitter& emitter, AudioListener& listener)
{
    // Emitter parameters
    Vector3  e_Position;
    e_Position = emitter.position;

    // Listener parameters
    Vector3 l_Top, l_Front, l_Left, l_Right, l_Position;
    l_Position = listener.position;
    l_Top = listener.vTopVector;
    l_Front = listener.vFrontVector;
    l_Right = Vector3::Cross(l_Top, l_Front);
    l_Left = l_Right * -1;

    // Distance between emitter and listener
    Vector3 distance = l_Position - e_Position ;
    Vector3 direction = distance;
    direction.Normalize();

    float e_LeftAngle = Vector3::GetAngle(l_Left, direction);
    float e_RightAngle = Vector3::GetAngle(l_Right, direction);

    // Calculate the difference in angle to process the channel volumes
    float left_Difference{ fabsf(e_LeftAngle / OBTUSE_ANGLE) };
    float right_Difference{ fabsf(e_RightAngle / OBTUSE_ANGLE) };

    // Calculate the outputted volume based on the distance
    float distance_length{ distance.Length() };
    float volume = 1.0f - (distance_length / emitter.size);

    float output_volume[2]{ volume, volume };

    if (left_Difference > 1)
        left_Difference = 0;
    output_volume[0] *= 1.0f - left_Difference;

    if (right_Difference > 1)
        right_Difference = 0;
    output_volume[1] *= 1.0f - right_Difference;


    // Clamps the volume
    output_volume[0] = Math::Clamp(output_volume[0], 0.0f, 1.0f);
    output_volume[1] = Math::Clamp(output_volume[1], 0.0f, 1.0f);
    return output_volume;



}


/*---------------------------------------------AUDIO GetStateMachine()----------------------------------------------------*/

std::shared_ptr<AUDIO_STATES::AudioStateMachine>AUDIO::GetStateMachine()
{
    return stateMachine;
}

/*---------------------------------------------AUDIO FilePath()----------------------------------------------------*/

std::wstring AUDIO::FilePath()
{
    return file_path;
}