#include "../Audio.h"
#include "AudioStateFadeOut.h"
using namespace AUDIO_STATES;



/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFadeOut Class----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFadeOut Execute()---------------------------------------------*/
void AudioStateFadeOut::Execute(AUDIO* parent)
{
	float volume{ parent->Volume() };
	float vol_difference{ volume - fade_target };
	if (fabs(vol_difference) > 0.01f)
		parent->SetVolume(volume - fade_increment);
	else
		parent->stateMachine->Transition(AudioStateEnum::AudioStatePause);
}


/*----------------------------------------AudioStateFadeOut Initialize()---------------------------------------------*/
/// <summary>
/// Called when transitioning to this state
/// </summary>
void AudioStateFadeOut::Initialize(AUDIO* parent)
{
	SetTargetVolume(0.0f);
}
