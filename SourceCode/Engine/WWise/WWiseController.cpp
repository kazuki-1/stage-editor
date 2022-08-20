#include "WWiseController.h"

#include <assert.h>
#include <filesystem>
#include "../../GAMEOBJECT.h"
#include "../../../WWise/SoundBank/Wwise_IDs.h"
#include "../../../External/WWISE/Common/Win32/AkDefaultIOHookBlocking.h"
#include "../../../External/WWISE/Common/Win32/AkFileHelpers.h"
#include "../../../External/WWISE/Common/Win32/AkFilePackageLowLevelIOBlocking.h"

CAkFilePackageLowLevelIOBlocking package;

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------WWiseController Class--------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------WWiseController Initialize()-------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/

void WWiseController::Initialize()
{

    AKRESULT  result = AK_Fail;

    // WWiseMemoryManager initialization
    AkMemSettings  memorySettings;
    AK::MemoryMgr::GetDefaultSettings(memorySettings);
    result = AK::MemoryMgr::Init(&memorySettings);
    assert(result == AK_Success);

    // WWiseStreamManager Initialization
    AkStreamMgrSettings streamMgrSettings;
    AK::StreamMgr::GetDefaultSettings(streamMgrSettings);
    assert(AK::StreamMgr::Create(streamMgrSettings) != nullptr);
    AK::StreamMgr::SetCurrentLanguage(AKTEXT("English (US)"));


    

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
    result = package.Init(deviceSettings);



    package.SetBasePath(L"./WWise/SoundBank/Windows");
    AK::StreamMgr::SetFileLocationResolver(&package);

    // WWiseSoundEngine Initialization

    AkInitSettings initSettings;
    AkPlatformInitSettings platformSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformSettings);
    result = AK::SoundEngine::Init(&initSettings, &platformSettings);
    assert( result == AK_Success);

    // WWiseMusicEngine Initialization

    AkMusicSettings musicSettings;
    AK::MusicEngine::GetDefaultInitSettings(musicSettings);
    result = AK::MusicEngine::Init(&musicSettings);
    assert( result == AK_Success);

    // WWiseSpatialAudio Initialization

    AkSpatialAudioInitSettings spatialAudioSettings;    // Automatically filled when constructed
    result = AK::SpatialAudio::Init(spatialAudioSettings);
    assert(result == AK_Success);

    // WWiseCommunication intialization
    
    //AkCommSettings commSettings;
    //AK::Comm::GetDefaultInitSettings(commSettings);
    //assert(AK::Comm::Init(commSettings) == AK_Success);


    std::filesystem::path path{ "Init.bnk" };

    AkBankID bankID;

    result = AK::SoundEngine::LoadBank(path.c_str(), bankID);
    assert(result == AK_Success);
    
    path.replace_filename("Test.bnk");

    result = AK::SoundEngine::LoadBank(path.c_str(), bankID);
    assert(result == AK_Success);


    AkUniqueID ids[] = { AK::EVENTS::PLAYMUSIC, AK::EVENTS::WALKONGRAVEL };
    AkInt32 event_count = 2;

    result = AK::SoundEngine::PrepareEvent(AK::SoundEngine::PreparationType::Preparation_Load, ids, event_count);
    int test = 0;


}

/*-------------------------------------------------------------WWiseController RegisterGameObject()-------------------------------------------------------*/

void WWiseController::RegisterGameObject(GameObject* object)
{
    AKRESULT result = AK::SoundEngine::RegisterGameObj(object->Data()->GetID(), object->Data()->Name().c_str());
    int test = 0;
}

/*-------------------------------------------------------------WWiseController RegisterGameObjects()-------------------------------------------------------*/

void WWiseController::ClearGameObjects()
{
    AK::SoundEngine::UnregisterAllGameObj();
}

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------WWiseController Execute()----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/

void WWiseController::Execute()
{

}

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------WWiseController Render()-----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/

void WWiseController::Render()
{
    AK::SoundEngine::RenderAudio();
}

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------WWiseController Finalize()---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------------*/

void WWiseController::Finalize()
{
    ClearGameObjects();
    //AK::Comm::Term();
    //

    std::filesystem::path path{ "Test.bnk" };

    AK::SoundEngine::ClearBanks();



    AK::MusicEngine::Term();
    AK::SoundEngine::Term();
    package.Term();
    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();
    AK::MemoryMgr::Term();

}