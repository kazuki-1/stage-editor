#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <memory>
#include <map>
#include <xaudio2.h>
#include <x3daudio.h>
#include "Singleton.h"
#include "Math.h"
#include "AudioStates/AudioStates.h"
using namespace Microsoft::WRL;


// Input channels used in 3d Audio
#define MAX_OUTPUT_CHANNELS 8
#define INPUT_CHANNELS 2
#define SPEED_OF_SOUND X3DAUDIO_SPEED_OF_SOUND
#define SPEED_OF_SOUND_PER_FRAME SPEED_OF_SOUND / PerformanceCounter::Instance()->FPS()
#define SAMPLING_RATE 48000
#define OBTUSE_ANGLE Math::ToRadians(180)
#define DEFAULT_FREQUENCY 1.0f

enum AUDIO_CALCULATION_FLAGS
{
    CALCULATE_CHANNELS = 0X1,  
    CALCULATE_DOPPLER = 0X2
};
enum ChannelDirections
{
    FrontLeft, 
    FrontRight, 
    BackLeft, 
    BackRight, 

};
enum class SoundEffects
{
    Echo
};



class Audio;
class AudioEmitter
{
public:
    AudioEmitter() {};


    Math::Vector3 position{};
    Math::Vector3 velocity{};
    Math::Vector3 vTopVector{};
    Math::Vector3 vFrontVector{};
    float minimum_distance{};
    float size{};
    float doppler_factor{};
};

class AudioListener
{
public:
    AudioListener() {};

    Math::Vector3 position{};
    Math::Vector3 velocity{};
    Math::Vector3 vTopVector{};
    Math::Vector3 vFrontVector{};
};


class Audio
{
protected:
    ComPtr<IUnknown>reverbEffect{};
    IXAudio2SourceVoice* sourceVoice{};
    IXAudio2SubmixVoice* submixVoice{};
    WAVEFORMATEXTENSIBLE format{};
    XAUDIO2_BUFFER buffer{};
    XAUDIO2_BUFFER effect_buffer{};
    AudioEmitter* audioEmitter;

    std::wstring file_path;
    bool isDucking{};
    bool isPlaying{};
    bool hasEffect{};
    float volume_before_ducking{};

    HRESULT FindChunk(HANDLE h, DWORD fourcc, DWORD& cSize, DWORD& cDataPosition);
    HRESULT ReadChunk(HANDLE h, void* buffer, DWORD buffer_Size, DWORD offset);
    std::shared_ptr<AUDIO_STATES::AudioStateMachine>stateMachine;
    std::vector<float>channel_volumes;
    friend class AudioEngine;
public:

    float volume{ 1.0f };
    float fade_in_volume{};
    Audio() {};
    Audio(std::wstring path);
    ~Audio()
    {
        int test;
        test = 0;
    }

    /// <summary>
    /// <para> Called to initialize the object </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame to perform functions </para>
    /// </summary>
    void Execute();
    virtual void Play();
    /// <summary>
    /// <para> Call this to perform fade in and play the file </para>
    /// </summary>
    /// <param name="fade_time"></param>
    void FadeInAndPlay(float fade_time);
    void FadeOutAndPause(float fade_time);
    void Stop();
    void Finalize();
    /// <summary>
    /// <para> Loops the audio. Default parameter is infinite. CALL BEFORE Play()</para>
    /// <para> オーディオをループする。デフォルト引数は無限 . Play()を呼び出す前に呼び出す</para>
    /// </summary>
    void Loop(UINT loopCount = XAUDIO2_LOOP_INFINITE);
    /// <summary>
    /// <para> Loops the audio from parameters begin till end. CALL BEFORE Play() </para>
    /// <para> オーディオを引数である範囲内にループする. Play()を呼び出す前に呼び出す </para>
    /// </summary>
    void LoopInRange(UINT begin, UINT end);
    /// <summary>
    /// <para> Perform a fadeIn effect from the audio. CALL BEFORE Play() </para>
    /// <para> オーディオにフェードインエフェクトをかける。Play()を呼び出す前に呼び出す </para>
    /// </summary>
    /// <param name="fade_time"> : Time taken for effect to finish</param>
    void FadeIn(float fade_time);
    /// <summary>
    /// <para> Perform a fadeOut effect from the audio. CALL BEFORE Play() </para>
    /// <para> オーディオにフェードアウトエフェクトをかける。Play()を呼び出す前に呼び出す </para>
    /// </summary>
    /// <param name="fade_time"> : Time taken for effect to finish</param>
    void FadeOut(float fade_time);
    /// <summary>
    /// <para> Perform a fade effect to the target volume. CALL BEFORE Play() </para>
    /// </summary>
    /// <param name = "fade_vol"> : Volume to be faded to </param>
    /// <param name="fade_time"> : Time taken for effect to finish</param>
    void FadeTo(float fade_vol, float fade_time = 1.0f);
    void DisableLoop();
    void SetVolume(float vol);
    /// <summary>
    /// <para> Creates the sourceVoice using the buffer </para>
    /// <para> バッファーをつかってsourceVoiceを生成する</para>
    /// </summary>
    /// <param name="buffer"></param>
    void SetBuffer(XAUDIO2_BUFFER buffer);
    /// <summary>
    /// <para> Performs ducking and fades the volume to 0.3f</para>
    /// </summary>
    void PerformDucking(float fade_vol);
    /// <summary>
    /// <para> stops the ducking state</para>
    /// </summary>
    void StopDucking();
    std::wstring FilePath();
    float Volume();
    XAUDIO2_BUFFER Buffer();
    IXAudio2SourceVoice* SourceVoice();
    bool IsPlaying();
    bool IsDucking();
    void RenderDebug();
    
    std::vector<float>CalculateChannelVolumes(AudioEmitter& emitter, AudioListener& listener, int input_channels, int output_channels = 2, UINT flags = AUDIO_CALCULATION_FLAGS::CALCULATE_CHANNELS);
    void PerformObstructionCalculation();
    void Synthesize(SoundEffects effect);
    void RevertSynthesis();

    std::shared_ptr<AUDIO_STATES::AudioStateMachine>GetStateMachine();

    // Targets the emitter
    void SetAudioEmitter(AudioEmitter* e) { audioEmitter = e; }
};


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