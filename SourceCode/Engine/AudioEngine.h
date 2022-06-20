#pragma once
#include <xaudio2.h>
#include <x3daudio.h>
#include <wrl.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "Singleton.h"
using namespace Microsoft::WRL;

class Audio;
class AudioListener;
class Dynamic_Plane;

struct Obstructor_Parameter
{
    
    Dynamic_Plane* plane;
    float obstruction_rate{ 0.5f };
};


/// <summary>
/// Use this to create an Audio object by calling AudioEngine::Instance()->Insert(std::string name, std::wstring file_path)
/// </summary>
class AudioEngine : public Singleton<AudioEngine>
{
    ComPtr<IXAudio2>xAudio;
    IXAudio2MasteringVoice* masteringVoice{};

    // Parameters for 3DAudio calculations
    X3DAUDIO_DSP_SETTINGS dspSettings{};
    AudioListener* audioListener{};
    X3DAUDIO_HANDLE x3d_handle;
    int nChannels{};        // Output channels
    friend class Audio;
    std::map<std::string, std::shared_ptr<Audio>>audios;
    std::vector <Obstructor_Parameter>obstructors;

public:
    ~AudioEngine() { Finalize(); }
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
    /// <para> Create an Audio object and insert it into the map </para>
    /// <para> AUDIOを生成し、マップに登録</para>
    /// </summary>
    /// <param name="name"> : Name of audio file</param>
    /// <param name="file_path"> : File path of audio file</param>
    void Insert(std::string name, std::wstring file_path);
    /// <summary>
    /// Registers a obstruction entity
    /// </summary>
    /// <param name="p"></param>
    void InsertObstructor(Dynamic_Plane* plane, float rate);
    /// <summary>
    /// Called when switching between scenes
    /// </summary>
    void Cleanup();
    /// <summary>
    /// Destroy any instances of obstructors
    /// </summary>
    void CleanupObstructors();
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
    std::vector<Obstructor_Parameter>GetObstructors();

    // Targets the listener
    void SetAudioListener(AudioListener* l) { audioListener = l; };
    AudioListener* GetAudioListener() { return audioListener; }
};
