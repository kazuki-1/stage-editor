#pragma once
#include <Library/Manager.h>
#include <Library/Singleton.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Comm/AkCommunication.h>
#include <AkFilePackageLowLevelIO.h>
class GameObject;

class WWiseController : public Singleton<WWiseController>, public Manager
{
public:
    void Initialize() override;
    void RegisterGameObject(GameObject* object);
    void ClearGameObjects();
    void Execute() override;
    void Render() override;
    void Finalize() override;
};