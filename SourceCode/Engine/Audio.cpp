#include "Audio.h"
#include <x3daudio.h>
#include <assert.h>
#include <vector>
#include "IMGUI.h"
#include "PERFORMANCE_COUNTER.h"
#define SPEED_OF_SOUND_PER_FRAME SPEED_OF_SOUND / PerformanceCounter::Instance()->FPS()

#define SAMPLING_RATE 48000
#define OBTUSE_ANGLE Math::ToRadians(180)
#define SPEED_OF_SOUND X3DAUDIO_SPEED_OF_SOUND
#define DEFAULT_FREQUENCY 1.0f
#define VELOCITY_OF_SOUND 5.0f


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

    // Coinitialization is called here, so calling it again is not needed
    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    assert(hr == S_OK);
    hr = XAudio2Create(xAudio.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
    assert(hr == S_OK);



    hr = xAudio->CreateMasteringVoice(&masteringVoice, 2, 48000);
    assert(hr == S_OK);
    Insert("Empty", L"./Data/Audio/empty_sound.wav");


    // Perform 3DAudio intiailization
    XAUDIO2_VOICE_DETAILS details;
    DWORD dwChannelMask;
    hr = masteringVoice->GetChannelMask(&dwChannelMask);
    masteringVoice->GetVoiceDetails(&details);

    //DSP settings in case needed
    dspSettings.SrcChannelCount = 2;
    nChannels = dspSettings.DstChannelCount = details.InputChannels;
    
    hr = X3DAudioInitialize(dwChannelMask, SPEED_OF_SOUND, x3d_handle);
    assert(hr == S_OK);
    

    return S_OK;



}

/*---------------------------------------------AudioEngine Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform functions </para>
/// </summary>
void AudioEngine::Execute()
{
    for (auto& a : audios)
        a.second->Execute();
}

/*---------------------------------------------AudioEngine Cleanup()----------------------------------------------------*/
/// <summary>
/// Called when switching between scenes
/// </summary>
void AudioEngine::Cleanup()
{
    audios.clear();
}

/*---------------------------------------------AudioEngine Finalize()----------------------------------------------------*/
/// <summary>
/// Finalizes the class, generally not needed
/// </summary>
void AudioEngine::Finalize()
{
    for (auto& a : audios)
        a.second->Finalize();
}

/*---------------------------------------------AudioEngine Delist()----------------------------------------------------*/
/// <summary>
/// Delists the audio file from the map
/// </summary>
/// <param name="audio_name"></param>
void AudioEngine::Delist(std::string audio_name)
{
    audios.erase(audio_name);
}

void AudioEngine::Delist(std::shared_ptr<Audio>audio)
{
    
    for (auto& a : audios)
    {
        if (audio == a.second)
        {
            audios.erase(a.first);
            return;
        }
    }
}


/*---------------------------------------------AudioEngine Insert()----------------------------------------------------*/
/// <summary>
/// <para> Create an Audio object and insert it into the map </para>
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
    std::shared_ptr<Audio>audio = std::make_shared<Audio>(file_path);
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

std::shared_ptr<Audio>AudioEngine::Retrieve(std::string name)
{
    for (auto& a : audios)
    {
        if (name == a.first)
            return a.second;
    }
    return audios.find("Empty")->second;
}

/*---------------------------------------------AudioEngine Audios()----------------------------------------------------*/

std::map<std::string, std::shared_ptr<Audio>>AudioEngine::Audios()
{
    return audios;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Audio Class----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Audio Contructor----------------------------------------------------*/

Audio::Audio(std::wstring path) : file_path(path)
{

}

/*---------------------------------------------Audio FindChunk()----------------------------------------------------*/

HRESULT Audio::FindChunk(HANDLE h, DWORD fourcc, DWORD& cSize, DWORD& cDataPosition)
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

/*---------------------------------------------Audio ReadChunk()----------------------------------------------------*/

HRESULT Audio::ReadChunk(HANDLE h, void* buffer, DWORD buffer_Size, DWORD offset)
{
    HRESULT hr{ S_OK };

    if (SetFilePointer(h, offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        assert(!"Error");

    DWORD read;
    if (!ReadFile(h, buffer, buffer_Size, &read, 0))
        assert(!"Error");
    return hr;
}

/*---------------------------------------------Audio Initialize()----------------------------------------------------*/
/// <summary>
/// <para> Called to initialize the object </para>
/// </summary>
/// <returns></returns>
HRESULT Audio::Initialize()
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

/*---------------------------------------------Audio Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform functions </para>
/// </summary>
void Audio::Execute()
{
    stateMachine->Execute();
    AudioEngine* audioEngine = AudioEngine::Instance();

    // Perform 3DAudio calculations if emitter is targeted
    if (audioEmitter && audioEngine->audioListener)
    {
        if (isPlaying) {

            UINT calc_flag{};
            calc_flag |= AUDIO_CALCULATION_FLAGS::CALCULATE_CHANNELS | AUDIO_CALCULATION_FLAGS::CALCULATE_DOPPLER;
            channel_volumes = CalculateChannelVolumes(*audioEmitter, *audioEngine->audioListener, format.Format.nChannels, audioEngine->nChannels, calc_flag);
            HRESULT hr = sourceVoice->SetOutputMatrix(audioEngine->masteringVoice, format.Format.nChannels, audioEngine->nChannels, channel_volumes.data());
            hr = sourceVoice->SetFrequencyRatio(audioEmitter->doppler_frequency);

        }
    }
}

/*---------------------------------------------Audio Play()----------------------------------------------------*/

void Audio::Play()
{
    if (isPlaying)
        return;
    HRESULT hr = sourceVoice->SubmitSourceBuffer(&buffer);
    assert(hr == S_OK);
    sourceVoice->Start();
    isPlaying = true;

}

/*---------------------------------------------Audio FadeInAndPlay()----------------------------------------------------*/

void Audio::FadeInAndPlay(float fade_time)
{
    Play();
    FadeIn(fade_time);
}

/*---------------------------------------------Audio FadeOutAndPause()----------------------------------------------------*/

void Audio::FadeOutAndPause(float fade_time)
{
    FadeOut(1.0f);
    if (volume <= 0.0f)
        Stop();
}

/*---------------------------------------------Audio Stop()----------------------------------------------------*/

void Audio::Stop()
{
    if (!isPlaying)
        return;
    sourceVoice->Stop();
    isPlaying = false;
}

/*---------------------------------------------Audio Finalize()----------------------------------------------------*/

void Audio::Finalize()
{
    sourceVoice->DestroyVoice();
}

/*---------------------------------------------Audio Loop()----------------------------------------------------*/

void Audio::Loop(UINT loopCount)
{
    buffer.LoopCount = loopCount;
}

/*---------------------------------------------Audio Loop()----------------------------------------------------*/

void Audio::LoopInRange(UINT begin, UINT end)
{
    buffer.LoopBegin = begin * format.Samples.wSamplesPerBlock;
    buffer.LoopLength = end * format.Samples.wSamplesPerBlock;
    buffer.PlayLength = buffer.LoopLength;
}

/*---------------------------------------------Audio FadeIn()----------------------------------------------------*/

void Audio::FadeIn(float fade_time)
{
    FadeTo(1.0f, fade_time);
}

/*---------------------------------------------Audio FadeOut()----------------------------------------------------*/

void Audio::FadeOut(float fade_time)
{
    stateMachine->FadeOut(fade_time);
}

/*---------------------------------------------Audio DisableLoop()----------------------------------------------------*/

void Audio::FadeTo(float fade_vol, float fade_time)
{
    stateMachine->FadeTo(fade_time, fade_vol);
}


/*---------------------------------------------Audio DisableLoop()----------------------------------------------------*/

void Audio::DisableLoop()
{
    buffer.LoopBegin = buffer.LoopLength = buffer.LoopCount = 0;
}

/*---------------------------------------------Audio SetVolume()----------------------------------------------------*/

void Audio::SetVolume(float vol)
{
    volume = vol;
    sourceVoice->SetVolume(volume);
}

/*---------------------------------------------Audio SetBuffer()----------------------------------------------------*/

void Audio::SetBuffer(XAUDIO2_BUFFER buffer)
{
    this->buffer = buffer;
}

/*---------------------------------------------Audio PerformDucking()----------------------------------------------------*/
/// <summary>
/// <para> Performs ducking and fades the volume to 0.3f</para>
/// </summary>
void Audio::PerformDucking(float fade_vol)
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

/*---------------------------------------------Audio StopDucking()----------------------------------------------------*/
/// <summary>
/// <para> stops the ducking state</para>
/// <para> ･ﾀ･ﾃ･ｭ･ｰ､ﾐﾖｹ､ｹ､・/para>
/// </summary>
void Audio::StopDucking()
{
    if (!isDucking)
        return;
    isDucking = false;
    if (isPlaying)
        FadeTo(volume_before_ducking, 0.5f);
}

/*---------------------------------------------Audio Volume()----------------------------------------------------*/

float Audio::Volume()
{
    return volume;
}

/*---------------------------------------------Audio Buffer()----------------------------------------------------*/

XAUDIO2_BUFFER Audio::Buffer()
{
    return buffer;
}

/*---------------------------------------------Audio SourceVoice()----------------------------------------------------*/

IXAudio2SourceVoice* Audio::SourceVoice()
{
    return sourceVoice;
}

/*---------------------------------------------Audio IsPlaying()----------------------------------------------------*/

bool Audio::IsPlaying()
{
    return isPlaying;
}

/*---------------------------------------------Audio IsDucking()----------------------------------------------------*/

bool Audio::IsDucking()
{
    return isDucking;
}

/*---------------------------------------------Audio RenderDebug()----------------------------------------------------*/

void Audio::RenderDebug()
{
    if (audioEmitter)
    {
        ImGui::Begin("Channel volumes");
        int ind{};
        for (auto& v : channel_volumes)
        {
            std::string preview{ "Channel volume " + std::to_string(ind) };
            ImGui::DragFloat(preview.c_str(), &v);
            ++ind;
        }
        ImGui::DragFloat("Frequency", &audioEmitter->doppler_frequency);
        ImGui::End();
    }
}

/*---------------------------------------------Audio CalculateChannelVolumes()----------------------------------------------------*/
/// <summary>
/// Call this to calculate the volume for each channels
/// </summary>
/// <param name="channel_num"> : Number of channels</param>
/// <returns></returns>
std::vector<float>Audio::CalculateChannelVolumes(AudioEmitter& emitter, AudioListener& listener, int input_channels, int output_channels, UINT flags)
{

    std::vector<float>output;
    std::vector<float>volumes;


    // Perform calculation of channel volumes
    if (flags & AUDIO_CALCULATION_FLAGS::CALCULATE_CHANNELS)
    {
        // Emitter parameters
        Vector3  e_Position;
        e_Position = emitter.position;

        // Listener parameters
        Vector3 l_Top, l_Front, l_Back, l_Left, l_Right, l_Position;
        l_Position = listener.position;
        l_Top = listener.vTopVector;
        l_Front = listener.vFrontVector;
        l_Right = Vector3::Cross(l_Top, l_Front);
        l_Left = l_Right * -1.0f;
        l_Back = l_Front * -1.0f;

        l_Front.Normalize();
        l_Back.Normalize();
        l_Left.Normalize();
        l_Right.Normalize();


        // Distance between emitter and listener
        Vector3 distance = l_Position - e_Position;
        Vector3 direction = distance;
        direction.Normalize();



        // FrontLeft and FrontRight
        {
            Vector3 front_dir = direction;
            front_dir += l_Front;
            front_dir.Normalize();


            float e_LeftAngle = Vector3::GetAngle(l_Left, front_dir);
            float e_RightAngle = Vector3::GetAngle(l_Right, front_dir);

            // Calculate the difference in angle to process the channel volumes
            float left_Difference{ fabsf(e_LeftAngle / OBTUSE_ANGLE) };
            float right_Difference{ fabsf(e_RightAngle / OBTUSE_ANGLE) };

            // Calculate the outputted volume based on the distance
            float distance_length{ distance.Length() };
            float volume = 1.0f - (distance_length / emitter.size);

            // if the difference is 1, it means that it is directly facing the source, so it will output the maximum volume
            if (left_Difference > 1)
                left_Difference = 0;
            if (right_Difference > 1)
                right_Difference = 0;



            volumes.push_back(volume * left_Difference);
            volumes.back() = Math::Clamp(volumes.back(), 0.0f, 1.0f);
            volumes.push_back(volume * right_Difference);
            volumes.back() = Math::Clamp(volumes.back(), 0.0f, 1.0f);

        }

        // BackLeft and BackRight
        {
            Vector3 back_dir = direction;
            back_dir -= l_Front;
            back_dir.Normalize();


            float e_LeftAngle = Vector3::GetAngle(l_Left, back_dir);
            float e_RightAngle = Vector3::GetAngle(l_Right, back_dir);

            // Calculate the difference in angle to process the channel volumes
            float left_Difference{ fabsf(e_LeftAngle / OBTUSE_ANGLE) };
            float right_Difference{ fabsf(e_RightAngle / OBTUSE_ANGLE) };

            // Calculate the outputted volume based on the distance
            float distance_length{ distance.Length() };
            float volume = 1.0f - (distance_length / emitter.size);

            // if the difference is 1, it means that it is directly facing the source, so it will output the maximum volume
            if (left_Difference > 1)
                left_Difference = 0;
            if (right_Difference > 1)
                right_Difference = 0;



            volumes.push_back(volume * left_Difference);
            volumes.back() = Math::Clamp(volumes.back(), 0.0f, 1.0f);
            volumes.push_back(volume * right_Difference);
            volumes.back() = Math::Clamp(volumes.back(), 0.0f, 1.0f);

        }




        if (input_channels * output_channels > 2)
        {
            // Creates a multi channel array
            for (int index = 0; index < input_channels * output_channels; ++index)
            {
                float cur_vol = volumes[index % output_channels];
                output.push_back(cur_vol);
            }
        }
        else
        {
            // Creates a double channel array
            output.push_back(volumes[0]);
            output.push_back(volumes[1]);
        }
        // If the distance is 0, it will result in a invalid calculation in the angle, so just return the same volume
        if (distance.Length() == 0)
        {
            for (auto& v : output)
                v = volume;
        }

    }
    // Perform calculation of doppler frequency
    if (flags & AUDIO_CALCULATION_FLAGS::CALCULATE_DOPPLER)
    {
        Vector3 distance = listener.position - emitter.position;
        Vector3 direction = distance;
        direction.Normalize();
        // The distance being 0 will cause X2AudioSourceVoice->SetFrequencyRatio() to fail, so we set the frequency as the default frequency
        if (distance.Length() == 0)
            emitter.doppler_frequency = DEFAULT_FREQUENCY;
        else {



            // Determine if the listener is going towards or away from the source
            float factor{ 1 }, emitter_factor{ 1 };
            Vector3 l_VelocityNormal{ listener.velocity }, e_VelocityNormal{emitter.velocity};
            l_VelocityNormal.Normalize();
            e_VelocityNormal.Normalize();

            if (listener.velocity.Length() > 0)
            {
                factor = Vector3::Dot(direction, l_VelocityNormal);
                factor = Math::Sign(factor) * -1;

            }if (emitter.velocity.Length() > 0)
                emitter_factor = Math::Sign(Vector3::Dot(direction, e_VelocityNormal));

            // Perform calculations
            float cur_freq = 
                (
                (SPEED_OF_SOUND_PER_FRAME + (listener.velocity.Length() * factor) ) /
                (SPEED_OF_SOUND_PER_FRAME - (emitter.velocity.Length() * emitter_factor))
                ) *
                DEFAULT_FREQUENCY;

            // Lerps the doppler shift so it doesnt change abruptly
            emitter.doppler_frequency = Math::Lerp(emitter.doppler_frequency, cur_freq, 0.3f);
        }
    }
    return output;
}

/*---------------------------------------------Audio GetStateMachine()----------------------------------------------------*/

std::shared_ptr<AUDIO_STATES::AudioStateMachine>Audio::GetStateMachine()
{
    return stateMachine;
}

/*---------------------------------------------Audio FilePath()----------------------------------------------------*/

std::wstring Audio::FilePath()
{
    return file_path;
}