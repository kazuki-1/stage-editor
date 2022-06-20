#include "AudioEngine.h"
#include "Audio.h"

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

/*---------------------------------------------AudioEngine CleanupObstructors()----------------------------------------------------*/
/// <summary>
/// Destroy any instances of obstructors
/// </summary>
void AudioEngine::CleanupObstructors()
{
    obstructors.clear();
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
/// <para> AUDIOÇê∂ê¨ÇµÅAÉ}ÉbÉvÇ…ìoò^</para>
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

/*---------------------------------------------AudioEngine InsertObstructor()----------------------------------------------------*/
/// <summary>
/// Registers a obstruction entity
/// </summary>
/// <param name="p"></param>
void AudioEngine::InsertObstructor(Dynamic_Plane* p, float rate)
{
    obstructors.push_back({p, rate});
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

/*---------------------------------------------AudioEngine GetObstructors()----------------------------------------------------*/

std::vector<Obstructor_Parameter>AudioEngine::GetObstructors()
{
    return obstructors;
}