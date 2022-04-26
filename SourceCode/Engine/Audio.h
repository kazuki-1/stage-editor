#pragma once
#include <d3d11.h>
#include <xaudio2.h>
#include <wrl.h>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include "AudioStates/AudioStates.h"

using namespace Microsoft::WRL;
class AUDIO;
/// <summary>
/// Use this to create an AUDIO object by calling AudioEngine::Instance()->Insert(std::string name, std::wstring file_path)
/// </summary>
class AudioEngine : public Singleton<AudioEngine>
{

    ComPtr<IXAudio2>xAudio;
    IXAudio2MasteringVoice* masteringVoice{};
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
    /// <para> Called every frame to perform functions </para>
    /// <para> ���ե�E`��˺��ӳ��� </para>
    /// </summary>
    void Execute();
    /// <summary>
    /// <para> Create an AUDIO object and insert it into the map </para>
    /// <para> AUDIO�𐶐����A�}�b�v�ɓo�^</para>
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
protected:
    IXAudio2SourceVoice* sourceVoice{};
    WAVEFORMATEXTENSIBLE format{};
    XAUDIO2_BUFFER buffer{};


    std::wstring file_path;
    bool isDucking{};
    bool isPlaying{};
    float volume_before_ducking{};

    HRESULT FindChunk(HANDLE h, DWORD fourcc, DWORD& cSize, DWORD& cDataPosition);
    HRESULT ReadChunk(HANDLE h, void* buffer, DWORD buffer_Size, DWORD offset);
    std::shared_ptr<AUDIO_STATES::AudioStateMachine>stateMachine;
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
    /// <para> �I�[�f�B�I�����[�v����B�f�t�H���g�����͖��� . Play()���Ăяo���O�ɌĂяo��</para>
    /// </summary>
    void Loop(UINT loopCount = XAUDIO2_LOOP_INFINITE);
    /// <summary>
    /// <para> Loops the audio from parameters begin till end. CALL BEFORE Play() </para>
    /// <para> �I�[�f�B�I�������ł���͈͓��Ƀ��[�v����. Play()���Ăяo���O�ɌĂяo�� </para>
    /// </summary>
    void LoopInRange(UINT begin, UINT end);
    /// <summary>
    /// <para> Perform a fadeIn effect from the audio. CALL BEFORE Play() </para>
    /// <para> �I�[�f�B�I�Ƀt�F�[�h�C���G�t�F�N�g��������BPlay()���Ăяo���O�ɌĂяo�� </para>
    /// </summary>
    /// <param name="fade_time"> : Time taken for effect to finish</param>
    void FadeIn(float fade_time);
    /// <summary>
    /// <para> Perform a fadeOut effect from the audio. CALL BEFORE Play() </para>
    /// <para> �I�[�f�B�I�Ƀt�F�[�h�A�E�g�G�t�F�N�g��������BPlay()���Ăяo���O�ɌĂяo�� </para>
    /// </summary>
    /// <param name="fade_time"> : Time taken for effect to finish</param>
    void FadeOut(float fade_time);
    /// <summary>
    /// <para> Perform a fade effect to the target volume. CALL BEFORE Play() </para>
    /// <para> ָ������E��ܥ�E�`��ޤǥե��`�ɥ��ե����Ȥ�E���E</para>
    /// </summary>
    /// <param name = "fade_vol"> : Volume to be faded to </param>
    /// <param name="fade_time"> : Time taken for effect to finish</param>
    void FadeTo(float fade_vol, float fade_time = 1.0f);
    void DisableLoop();
    void SetVolume(float vol);
    /// <summary>
    /// <para> Creates the sourceVoice using the buffer </para>
    /// <para> �o�b�t�@�[��������sourceVoice�𐶐�����</para>
    /// </summary>
    /// <param name="buffer"></param>
    void SetBuffer(XAUDIO2_BUFFER buffer);
    /// <summary>
    /// <para> Performs ducking and fades the volume to 0.3f</para>
    /// <para>���å��󥰤��Ф��������򣰣������{������E</para>
    /// </summary>
    void PerformDucking(float fade_vol);
    /// <summary>
    /// <para> stops the ducking state</para>
    /// <para> ���å��󥰤���ֹ����E/para>
    /// </summary>
    void StopDucking();
    std::wstring FilePath();
    /// <summary>
    /// <para> Called to initialize the object </para>
    /// <para> ���֥������Ȥ���ڻ�����E</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame to perform functions </para>
    /// <para> ���ե�E`��˺��ӳ��� </para>
    /// </summary>
    void Execute();
    float Volume();
    XAUDIO2_BUFFER Buffer();
    IXAudio2SourceVoice* SourceVoice();
    bool IsPlaying();
    bool IsDucking();
    std::shared_ptr<AUDIO_STATES::AudioStateMachine>GetStateMachine();
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