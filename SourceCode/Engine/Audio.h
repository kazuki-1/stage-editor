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
// Input channels used in 3d Audio
#define MAX_OUTPUT_CHANNELS 8
#define INPUT_CHANNELS 1
using namespace Microsoft::WRL;




class AUDIO;
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
/// <summary>
/// Use this to create an AUDIO object by calling AudioEngine::Instance()->Insert(std::string name, std::wstring file_path)
/// </summary>
class AudioEngine : public Singleton<AudioEngine>
{
    ComPtr<IXAudio2>xAudio;
    IXAudio2MasteringVoice* masteringVoice{};

    // Parameters for 3DAudio calculations
    X3DAUDIO_DSP_SETTINGS dspSettings;
    X3DAUDIO_LISTENER* listener;
    AudioListener* audioListener;
    X3DAUDIO_HANDLE x3d_handle;
    float matrixCoefficients[INPUT_CHANNELS * MAX_OUTPUT_CHANNELS];
    int nChannels{};
    friend class AUDIO;
    std::map<std::string, std::shared_ptr<AUDIO>>audios;


public:
    /// <summary>
    /// Initializes the audioengine, initializing the IXAudio2 and IXAudio2MasteringVoice
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame to perform functions </para>
    /// <para> 垈･ﾕ･・`･爨ﾋｺﾓｳｹ </para>
    /// </summary>
    void Execute();
    /// <summary>
    /// <para> Create an AUDIO object and insert it into the map </para>
    /// <para> AUDIOを生成し、マップに登録</para>
    /// </summary>
    /// <param name="name"> : Name of audio file</param>
    /// <param name="file_path"> : File path of audio file</param>
    void Insert(std::string name, std::wstring file_path);
    /// <summary>
    /// Finalizes the class, generally not needed
    /// </summary>
    void Finalize();
    IXAudio2MasteringVoice* MasteringVoice();
    ComPtr<IXAudio2>XAudio();
    std::shared_ptr<AUDIO>Retrieve(std::string name);
    std::map<std::string, std::shared_ptr<AUDIO>>Audios();

    // Targets the listener
    void SetListener(X3DAUDIO_LISTENER* l) { listener = l; }
    void SetAudioListener(AudioListener* l) { audioListener = l; };


    X3DAUDIO_LISTENER* GetListener() { return listener; }
    AudioListener* GetAudioListener() { return audioListener; }
};

class AUDIO
{
protected:
    IXAudio2SourceVoice* sourceVoice{};
    WAVEFORMATEXTENSIBLE format{};
    XAUDIO2_BUFFER buffer{};
    X3DAUDIO_EMITTER* emitter{nullptr};
    AudioEmitter* audioEmitter;

    std::wstring file_path;
    bool isDucking{};
    bool isPlaying{};
    float volume_before_ducking{};

    HRESULT FindChunk(HANDLE h, DWORD fourcc, DWORD& cSize, DWORD& cDataPosition);
    HRESULT ReadChunk(HANDLE h, void* buffer, DWORD buffer_Size, DWORD offset);
    std::shared_ptr<AUDIO_STATES::AudioStateMachine>stateMachine;

    friend class AudioEngine;
public:

    float volume{ 1.0f };
    float fade_in_volume{};
    AUDIO() {};
    AUDIO(std::wstring path);
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
    /// <summary>
    /// <para> Called to initialize the object </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame to perform functions </para>
    /// </summary>
    void Execute();
    float Volume();
    XAUDIO2_BUFFER Buffer();
    IXAudio2SourceVoice* SourceVoice();
    bool IsPlaying();
    bool IsDucking();
    void RenderDebug();
    /// <summary>
    /// Call this to calculate the volume for each channels
    /// </summary>
    /// <param name="channel_num"> : Number of channels</param>
    /// <returns></returns>
    float* CalculateChannelVolumes(AudioEmitter& emitter, AudioListener& listener);

    std::shared_ptr<AUDIO_STATES::AudioStateMachine>GetStateMachine();

    // Targets the emitter
    void SetEmitter(X3DAUDIO_EMITTER* e) { 
        emitter = e; }
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