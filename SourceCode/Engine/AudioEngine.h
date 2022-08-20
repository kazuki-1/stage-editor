#pragma once
#include <xaudio2.h>
#include <x3daudio.h>
#include <wrl.h>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <xaudio2fx.h>
#include <Library/Singleton.h>
using namespace Microsoft::WRL;

class Audio;
class AudioListener;
class Dynamic_Plane;


//XAUDIO2FX_REVERB_I3DL2_PARAMETERS REVERB_PRESETS[] =
//{
//    XAUDIO2FX_I3DL2_PRESET_FOREST,
//    XAUDIO2FX_I3DL2_PRESET_DEFAULT,
//    XAUDIO2FX_I3DL2_PRESET_GENERIC,
//    XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
//    XAUDIO2FX_I3DL2_PRESET_ROOM,
//    XAUDIO2FX_I3DL2_PRESET_BATHROOM,
//    XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
//    XAUDIO2FX_I3DL2_PRESET_STONEROOM,
//    XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
//    XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
//    XAUDIO2FX_I3DL2_PRESET_CAVE,
//    XAUDIO2FX_I3DL2_PRESET_ARENA,
//    XAUDIO2FX_I3DL2_PRESET_HANGAR,
//    XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
//    XAUDIO2FX_I3DL2_PRESET_HALLWAY,
//    XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
//    XAUDIO2FX_I3DL2_PRESET_ALLEY,
//    XAUDIO2FX_I3DL2_PRESET_CITY,
//    XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
//    XAUDIO2FX_I3DL2_PRESET_QUARRY,
//    XAUDIO2FX_I3DL2_PRESET_PLAIN,
//    XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
//    XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
//    XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
//    XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
//    XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
//    XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
//    XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
//    XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
//    XAUDIO2FX_I3DL2_PRESET_PLATE,
//};




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
