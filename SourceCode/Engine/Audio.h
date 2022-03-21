#pragma once
#include <d3d11.h>
#include <xaudio2.h>
#include <wrl.h>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"

using namespace Microsoft::WRL;
class AUDIO;
/// <summary>
/// Use this to create an AUDIO object by calling AUDIOENGINE::Instance()->Insert(std::string name, std::wstring file_path)
/// </summary>
class AUDIOENGINE : public SINGLETON<AUDIOENGINE>
{
    ComPtr<IXAudio2>xAudio;
    IXAudio2MasteringVoice* masteringVoice;
    //IXAudio2SourceVoice* SourceVoice;
    std::map<std::string, std::shared_ptr<AUDIO>>audios;

    WAVEFORMATEXTENSIBLE format;

public:
    /// <summary>
    /// Initializes the audioengine, initializing the IXAudio2 and IXAudio2MasteringVoice
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
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
    
};

class AUDIO
{
    std::wstring file_path;
    bool isPlaying{};
    IXAudio2SourceVoice* sourceVoice{};
    WAVEFORMATEXTENSIBLE format{};
    XAUDIO2_BUFFER buffer{};
    HRESULT FindChunk(HANDLE h, DWORD fourcc, DWORD& cSize, DWORD& cDataPosition);
    HRESULT ReadChunk(HANDLE h, void* buffer, DWORD buffer_Size, DWORD offset);
public:
    float volume{ 1.0f };
    AUDIO(std::wstring path);
    void Play();
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
    void DisableLoop();
    void SetVolume(float vol);

    std::wstring FilePath();

    HRESULT Initialize();
    float Volume();
    XAUDIO2_BUFFER Buffer();
    IXAudio2SourceVoice* SourceVoice();
    bool IsPlaying();
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