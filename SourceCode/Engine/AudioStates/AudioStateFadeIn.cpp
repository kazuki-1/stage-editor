#include "../Audio.h"
#include "AudioStateFadeIn.h"
using namespace AUDIO_STATES;

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFadeIn Class----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFadeIn Initialize()---------------------------------------------*/
/// <summary>
/// Called when transitioning to this state
/// </summary>
void AudioStateFadeIn::Initialize(Audio* parent)
{
	AudioStateFade::Initialize(parent);
	if (!parent->IsPlaying())
		parent->Play();
}



/*----------------------------------------AudioStateFadeIn Execute()---------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform any functions </para>
/// </summary>
void AudioStateFadeIn::Execute(Audio* parent)
{
	float volume{ parent->Volume() };
	if (fabsf(volume - fade_target) > 0.01f)
		parent->SetVolume(volume - fade_increment);
	else
		parent->GetStateMachine()->Transition(AudioStateEnum::AudioStatePlay);
}

