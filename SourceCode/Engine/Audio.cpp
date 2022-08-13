#include "Audio.h"
#include "../GAMEOBJECT.h"
#include "../Components/MESH.h"
#include "COLLISION.h"
#include <x3daudio.h>
#include <assert.h>
#include <vector>
#include "IMGUI.h"
#include "PERFORMANCE_COUNTER.h"
#include "AudioEngine.h"
#include <xaudio2fx.h>
#include "DEBUG_PRIMITIVE.h"
#include "AudioStates/Synthesizer.h"

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
/*---------------------------------------------Audio Class-----------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Audio Contructor------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/

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


    

    AudioEngine* audioEngine = AudioEngine::Instance();

    XAUDIO2_BUFFER buf{};
    buf.AudioBytes = dwChunkSize;
    buf.pAudioData = buffer;
    buf.Flags = XAUDIO2_END_OF_STREAM;

    XAUDIO2_SEND_DESCRIPTOR descriptor[2]{};
    descriptor[0].Flags = XAUDIO2_SEND_USEFILTER;
    descriptor[0].pOutputVoice = audioEngine->masteringVoice;
    //descriptor[1].Flags = XAUDIO2_SEND_USEFILTER;
    //descriptor[1].pOutputVoice = submixVoice;

    // Reverb submix voice
    UINT reverb_flags{};

#ifdef _DEBUG && defined(USING_XAUDIO2_7_DIRECTX)

    reverb_flags |= 1;

#endif
    assert(XAudio2CreateReverb(reverbEffect.GetAddressOf(), reverb_flags) == S_OK);
    XAUDIO2_EFFECT_DESCRIPTOR effect_desc[] = { {reverbEffect.Get(), true, 1} };
    XAUDIO2_EFFECT_CHAIN effect_chain = { 1, effect_desc };

    assert(audioEngine->XAudio()->CreateSubmixVoice(&submixVoice, INPUT_CHANNELS, SAMPLING_RATE, 0, 0, 0, &effect_chain) == S_OK);
    XAUDIO2FX_REVERB_PARAMETERS reverb_param{};
    //ReverbConvertI3DL2ToNative(&REVERB_PRESETS[0], &reverb_param);
    //submixVoice->SetEffectParameters(0, &reverb_param, sizeof(reverb_param));

    




    // Source voice creation
    XAUDIO2_VOICE_SENDS sendList{ 1, descriptor };
    HRESULT hr = audioEngine->XAudio()->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&format, 0, 2.0f, 0, &sendList);
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
            PerformObstructionCalculation();
            HRESULT hr = sourceVoice->SetOutputMatrix(audioEngine->masteringVoice, format.Format.nChannels, audioEngine->nChannels, channel_volumes.data());
            hr = sourceVoice->SetFrequencyRatio(audioEmitter->doppler_factor);

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

    // Shows the channel volume and frequencies of the audio emitter
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
        ImGui::DragFloat("Frequency", &audioEmitter->doppler_factor);
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
        Vector3  e_Position = emitter.position;

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
            float difference = distance_length / emitter.size;
            difference = Math::Clamp(difference, 0.0f, 1.0f);
            float volume = this->volume - difference;

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
            float difference = distance_length / emitter.size;
            difference = Math::Clamp(difference, 0.0f, 1.0f);
            float volume = this->volume - difference;

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
            emitter.doppler_factor = DEFAULT_FREQUENCY;

        else 
        {
            // Determine if the listener is going towards or away from the source
            float listener_factor{ 1 }, emitter_factor{ 1 };
            Vector3 l_VelocityNormal{ listener.velocity }, e_VelocityNormal{emitter.velocity};
            l_VelocityNormal.Normalize();
            e_VelocityNormal.Normalize();

            if (listener.velocity.Length() > 0)
            {
                listener_factor = Vector3::Dot(direction, l_VelocityNormal);
                listener_factor = Math::Sign(listener_factor) * -1;

            }if (emitter.velocity.Length() > 0)
                emitter_factor = Math::Sign(Vector3::Dot(direction, e_VelocityNormal));

            // Perform calculations
            float cur_freq = 
                (
                (SPEED_OF_SOUND_PER_FRAME + (listener.velocity.Length() * listener_factor) ) /
                (SPEED_OF_SOUND_PER_FRAME - (emitter.velocity.Length() * emitter_factor))
                ) *
                DEFAULT_FREQUENCY;

            // Lerps the doppler shift so it doesnt change abruptly
            emitter.doppler_factor = Math::Lerp(emitter.doppler_factor, cur_freq, 0.3f);
        }
    }

    for (auto& vol : output)
        vol = Math::Clamp(vol, 0.0f, 1.0f);


    return output;
}

/*---------------------------------------------Audio StopDucking()----------------------------------------------------*/

void Audio::PerformObstructionCalculation()
{

    if (!audioEmitter)
        return;
    AudioListener* listener = AudioEngine::Instance()->GetAudioListener();

    Vector3 start = audioEmitter->position;
    Vector3 end = listener->position;

    for (auto& obstructor : AudioEngine::Instance()->GetObstructors())
    {
        Dynamic_Plane* plane = obstructor.plane;
        COLLIDERS::RayCastResults results;
        if (!COLLIDERS::RayCast(start, end, plane, results))
            continue;

        // Horizontal influence
        // The further the listener is away from the center point on a horizontal plane
        // The weaker the obstruction
        float max_influence = { plane->GetSize().Length() / 2 };
        float point_influence = { fabsf((plane->GetCenter() - results.position).Length()) };
        point_influence = fabsf(max_influence - point_influence);
        point_influence = point_influence / max_influence;
        point_influence = obstructor.obstruction_rate * point_influence;

        // Depth influence
        // The further away the listener is away from the obstructor
        // The weaker the obstruction
        float depth_influence = (end - results.position).Length();
        depth_influence = 1.0f - depth_influence / plane->GetSize().x;
        depth_influence = min(depth_influence, 1.0f);


        for (auto& volume : channel_volumes)
            volume *= 1.0f - (point_influence * depth_influence);
        break;

    }


}

/*---------------------------------------------Audio Synthesize()----------------------------------------------------*/

void Audio::Synthesize(SoundEffects effect)
{
    if (hasEffect)
        return;
    switch (effect)
    {
    case SoundEffects::Echo:
    {
        effect_buffer = buffer;
        BYTE* audio_buffer = new BYTE[buffer.AudioBytes];
        memcpy(audio_buffer, effect_buffer.pAudioData, effect_buffer.AudioBytes);
        
        Synthesizer::Echo<BYTE>(audio_buffer, 0.5f, 2, effect_buffer.AudioBytes);
        memcpy((void*)(effect_buffer.pAudioData), audio_buffer, effect_buffer.AudioBytes);
        effect_buffer.pAudioData = audio_buffer;
        break;
    }
    }


    sourceVoice->FlushSourceBuffers();
    sourceVoice->SubmitSourceBuffer(&effect_buffer);
    

    hasEffect = true;

}

/*---------------------------------------------Audio RevertSynthesis()----------------------------------------------------*/

void Audio::RevertSynthesis()
{
    if (!hasEffect)
        return;

    sourceVoice->FlushSourceBuffers();
    sourceVoice->SubmitSourceBuffer(&buffer);
    hasEffect = false;


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