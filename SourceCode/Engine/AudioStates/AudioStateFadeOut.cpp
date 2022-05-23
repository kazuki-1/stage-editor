#include "../Audio.h"
#include "AudioStateFadeOut.h"
using namespace AUDIO_STATES;



/*-------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFadeOut Class----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFadeOut Initialize()---------------------------------------------*/
/// <summary>
/// Called when transitioning to this state
/// </summary>
void AudioStateFadeOut::Initialize(Audio* parent)
{
	SetTargetVolume(0.0f);
}

/*----------------------------------------AudioStateFadeOut Execute()---------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform any functions </para>
/// </summary>
void AudioStateFadeOut::Execute(Audio* parent)
{
	float volume{ parent->Volume() };
	float vol_difference{ volume - fade_target };
	if (fabs(vol_difference) > 0.01f)
		parent->SetVolume(volume - fade_increment);
	else
		parent->GetStateMachine()->Transition(AudioStateEnum::AudioStatePause);
}
