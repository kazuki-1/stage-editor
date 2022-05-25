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

enum AUDIO_CALCULATION_FLAGS
{
    CALCULATE_CHANNELS = 0X1,  
    CALCULATE_DOPPLER = 0X2
};


using namespace Microsoft::WRL;





enum ChannelDirections
{
    FrontLeft, 
    FrontRight, 
    BackLeft, 
    BackRight, 

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
    float doppler_frequency{};
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
/// Use this to create an Audio object by calling AudioEngine::Instance()->Insert(std::string name, std::wstring file_path)
/// </summary>
class AudioEngine : public Singleton<AudioEngine>
{
    ComPtr<IXAudio2>xAudio;
    IXAudio2MasteringVoice* masteringVoice{};

    // Parameters for 3DAudio calculations
    X3DAUDIO_DSP_SETTINGS dspSettings;
    AudioListener* audioListener;
    X3DAUDIO_HANDLE x3d_handle;
    float matrixCoefficients[INPUT_CHANNELS * MAX_OUTPUT_CHANNELS];
    int nChannels{};
    friend class Audio;
    std::map<std::string, std::shared_ptr<Audio>>audios;


public:
    ~AudioEngine() { Finalize(); }
    /// <summary>
    /// Initializes the audioengine, initializing the IXAudio2 and IXAudio2MasteringVoice
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame to perform functions </para>
    /// <para> °₯Υ₯E`₯ΰ€ΛΊτ€Σ³φ€Ή </para>
    /// </summary>
    void Execute();
    /// <summary>
    /// <para> Create an Audio object and insert it into the map </para>
    /// <para> AUDIOπΆ¬΅A}bvΙo^</para>
    /// </summary>
    /// <param name="name"> : Name of audio file</param>
    /// <param name="file_path"> : File path of audio file</param>
    void Insert(std::string name, std::wstring file_path);
    /// <summary>
    /// Called when switching between scenes
    /// </summary>
    void Cleanup();
    /// <summary>
    /// Finalizes the class, generally not needed
    /// </summary>
    void Finalize();
    /// <summary>
    /// Delists the audio file from the map
    /// </summary>
    /// <param name="audio_name"></param>
    void Delist(std::string audio_name);
    void Delist(std::shared_ptr<Audio>audio);

    IXAudio2MasteringVoice* MasteringVoice();
    ComPtr<IXAudio2>XAudio();
    std::shared_ptr<Audio>Retrieve(std::string name);
    std::map<std::string, std::shared_ptr<Audio>>Audios();

    // Targets the listener
    void SetAudioListener(AudioListener* l) { audioListener = l; };
    AudioListener* GetAudioListener() { return audioListener; }
};

class Audio
{
protected:
    IXAudio2SourceVoice* sourceVoice{};
    WAVEFORMATEXTENSIBLE format{};
    XAUDIO2_BUFFER buffer{};
    AudioEmitter* audioEmitter;

    std::wstring file_path;
    bool isDucking{};
    bool isPlaying{};
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
    /// <para> I[fBIπ[v·ιBftHgψΝ³ΐ . Play()πΔΡo·OΙΔΡo·</para>
    /// </summary>
    void Loop(UINT loopCount = XAUDIO2_LOOP_INFINITE);
    /// <summary>
    /// <para> Loops the audio from parameters begin till end. CALL BEFORE Play() </para>
    /// <para> I[fBIπψΕ ιΝΝΰΙ[v·ι. Play()πΔΡo·OΙΔΡo· </para>
    /// </summary>
    void LoopInRange(UINT begin, UINT end);
    /// <summary>
    /// <para> Perform a fadeIn effect from the audio. CALL BEFORE Play() </para>
    /// <para> I[fBIΙtF[hCGtFNgπ©―ιBPlay()πΔΡo·OΙΔΡo· </para>
    /// </summary>
    /// <param name="fade_time"> : Time taken for effect to finish</param>
    void FadeIn(float fade_time);
    /// <summary>
    /// <para> Perform a fadeOut effect from the audio. CALL BEFORE Play() </para>
    /// <para> I[fBIΙtF[hAEgGtFNgπ©―ιBPlay()πΔΡo·OΙΔΡo· </para>
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
    /// <para> obt@[πΒ©ΑΔsourceVoiceπΆ¬·ι</para>
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

    std::vector<float>CalculateChannelVolumes(AudioEmitter& emitter, AudioListener& listener, int input_channels, int output_channels = 2, UINT flags = AUDIO_CALCULATION_FLAGS::CALCULATE_CHANNELS);

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