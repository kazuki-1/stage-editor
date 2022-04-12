#include "AudioStateFade.h"
#include "../Audio.h"
using namespace AUDIO_STATES;


/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFade Class----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateFade Initialize()---------------------------------------------*/
/// <summary>
/// <para> Called when transitioned into this state </para>
/// </summary>
void AudioStateFade::Initialize(AUDIO* parent)
{
    float current_volume{ parent->Volume() };
    float fade_in_frame{ fade_in_time * 60.0f };
    float difference{ current_volume - fade_target };
    fade_increment = difference / fade_in_frame;

}

/*----------------------------------------AudioStateFade Finalize()-------------------------------------------*/
/// <summary>
/// <para>Called when transitioning away from this state </para>
/// </summary>
void AudioStateFade::Finalize(AUDIO* parent)
{
    parent->SetVolume(fade_target);
    fade_increment = fade_target = 0.0f;
}

/*----------------------------------------AudioStateFade SetVolume()-------------------------------------------*/
/// <summary>
/// <para> Sets the volume to be faded to </para>
/// </summary>
void AudioStateFade::SetTargetVolume(float vol)
{
    fade_target = vol;
}

/*----------------------------------------AudioStateFade SetTime()-------------------------------------------*/
/// <summary>
/// <para> Sets the fade timer</para>
/// </summary>
void AudioStateFade::SetTime(float t)
{
    fade_in_time = t;
}

/*----------------------------------------AudioStateFade CalculateRate()-------------------------------------------*/
/// <summary>
/// <para> Calculates the rate of fading. Call after SetVolume and SetTime</para>
/// </summary>
void AudioStateFade::CalculateRate(AUDIO* parent)
{
    float current_volume{ parent->Volume() };
    float fade_in_frame{ fade_in_time * 60.0f };
    float difference{ current_volume - fade_target };
    fade_increment = difference / fade_in_frame;

}
