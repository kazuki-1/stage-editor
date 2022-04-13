#include "../../Engine/IMGUI.h"
#include "AudioStates.h"
#include "AudioStatePlay.h"
#include "AudioStateFade.h"
#include "AudioStateFadeIn.h"
#include "AudioStateFadeOut.h"
#include "AudioStatePause.h"
using namespace AUDIO_STATES;

/*-------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateMachine Class----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStateMachine Initialize()---------------------------------------------*/
/// <summary>
/// <para> Called when AudioStateMachine is initialized </para>
/// </summary>
void AudioStateMachine::Initialize()
{
    if (audioStates.size() > 0)
        audioStates.clear();
    audioStates.emplace(AudioStateEnum::AudioStatePlay, std::make_shared<AudioStatePlay>());
    audioStates.emplace(AudioStateEnum::AudioStatePause, std::make_shared<AudioStatePause>());
    audioStates.emplace(AudioStateEnum::AudioStateFadeIn, std::make_shared<AudioStateFadeIn>());
    audioStates.emplace(AudioStateEnum::AudioStateFadeOut, std::make_shared<AudioStateFadeOut>());
    current_state = {};
    Transition(AudioStateEnum::AudioStatePause);

}

/*----------------------------------------AudioStateMachine Transition()---------------------------------------------*/
/// <summary>
/// <para> Called to transition to a different state </para>
/// <para> ���Ʃ`�Ȥ��w�Ƥ���E�</para>
/// </summary>
void AudioStateMachine::Transition(AudioStateEnum next_state)
{
    if (state_Enum == next_state)
        return;
    state_Enum = next_state;
    if (current_state)
        current_state->Finalize(parent);
    current_state = audioStates.find(next_state)->second;
    current_state->Initialize(parent);
}

/*----------------------------------------AudioStateMachine Execute()---------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform any functions </para>
/// </summary>
void AudioStateMachine::Execute()
{
    if (current_state)
        current_state->Execute(parent);
}

/*----------------------------------------AudioStateMachine FadeTo()---------------------------------------------*/
/// <summary>
/// <para> Perform fade to the specified volume in the specified time</para>
/// </summary>
/// <param name="fade_time"> : Fade will be done in this. Unit is second</param>
/// <param name="fade_vol"> : Fade volume target</param>
void AudioStateMachine::FadeTo(float fade_time, float fade_vol)
{
    if (state_Enum == AudioStateEnum::AudioStateFadeIn)
        return;
    Transition(AudioStateEnum::AudioStateFadeIn);
    dynamic_cast<AudioStateFadeIn*>(current_state.get())->SetTime(fade_time);
    dynamic_cast<AudioStateFadeIn*>(current_state.get())->SetTargetVolume(fade_vol);
    dynamic_cast<AudioStateFadeIn*>(current_state.get())->CalculateRate(parent);
}

/*----------------------------------------AudioStateMachine FadeOut()---------------------------------------------*/
/// <summary>
/// Perform fade out on the volume within the specified time
/// </summary>
/// <param name="fade_time"></param>
void AudioStateMachine::FadeOut(float fade_time)
{
    if (state_Enum == AudioStateEnum::AudioStateFadeOut)
        return;
    Transition(AudioStateEnum::AudioStateFadeOut);
    dynamic_cast<AudioStateFadeOut*>(current_state.get())->SetTime(fade_time);
    dynamic_cast<AudioStateFadeOut*>(current_state.get())->CalculateRate(parent);
}

/*----------------------------------------AudioStateMachine CurrentState()---------------------------------------------*/
/// <summary>
/// <para> Returns the current audioStateEnum</para>
/// <para> �F�ڤΥ��Ʃ`��Enum�򷵤� </para>
/// </summary>
/// <returns></returns>
AudioStateEnum AudioStateMachine::CurrentState()
{
    return state_Enum;
}

/*----------------------------------------AudioStateMachine DebugUI()---------------------------------------------*/
void AudioStateMachine::DebugUI()
{
    
    switch (state_Enum)
    {
    case AudioStateEnum::AudioStatePlay:
        ImGui::Text("AudioStatePlay");
        break;
    case AudioStateEnum::AudioStatePause:
        ImGui::Text("AudioStatePause");
        break;    
    case AudioStateEnum::AudioStateFadeIn:
        ImGui::Text("AudioStateFadeIn");
        break;
    case AudioStateEnum::AudioStateFadeOut:
        ImGui::Text("AudioStateFadeOut");
        break;
    }
}