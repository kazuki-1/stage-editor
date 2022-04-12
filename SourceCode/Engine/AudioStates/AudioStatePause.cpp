#include "AudioStates.h"
#include "AudioStatePause.h"
#include "../Audio.h"
using namespace AUDIO_STATES;
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStatePause Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStatePause Initialize()--------------------------------------------*/
/// <summary>
/// <para> Called when transitioned into this state </para>
/// <para> 他のステートから遷移のときに呼び出す　</para>
/// </summary>
void AudioStatePause::Initialize(AUDIO* parent)
{
    //float volume{ parent->Volume() };
    //float decrement{ volume / fade_time * 60.0f };
        parent->SourceVoice()->Stop();
        parent->SetVolume(0.0f);
}

/*----------------------------------------AudioStatePause Execute()--------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform any functions </para>
/// <para> 毎フE`ムに呼び出す </para>
/// </summary>
void AudioStatePause::Execute(AUDIO* parent)
{

    //float volume{ parent->Volume() };
    //if (volume > 0.01f)
    //    parent->SetVolume(volume - decrement);
    //else
    //{
    //    parent->SetVolume(0.0f);
    //    parent->SourceVoice()->Stop();
    //}
}

/*----------------------------------------AudioStatePause Execute()--------------------------------------------*/
/// <summary>
/// <para>Called when transitioning away from this state </para>
/// <para> ステート遷移の丒に呼び出す </para>
/// </summary>
void AudioStatePause::Finalize(AUDIO* parent)
{

}
