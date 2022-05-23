#pragma once
#include <map>
#include <memory>

class Audio;
namespace AUDIO_STATES
{
    enum class AudioStateEnum
    {
        Start =  -1, 
        AudioStatePlay, 
        AudioStatePause, 
        AudioStateFadeIn,
        AudioStateFadeOut, 
        End

    };
    class AudioStateBase
    {
    public:
        AudioStateBase() {};
        /// <summary>
        /// <para> Called when transitioned into this state </para>
        /// <para>他のステートから遷移したら呼び出す</para>
        /// </summary>
        virtual void Initialize(Audio* parent) {};
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// <para>毎フレームに呼び出す</para>
        /// </summary>
        virtual void Execute(Audio* parent) {};
        /// <summary>
        /// <para>Called when transitioning away from this state </para>
        /// <para> 他のステート遷移の場合に呼び出す </para>
        /// </summary>
        virtual void Finalize(Audio* parent) {};
    };


    // Audio state machine, internally used in Audio class
    class AudioStateMachine
    {
        std::map<AudioStateEnum, std::shared_ptr<AudioStateBase>>audioStates;
        std::shared_ptr<AudioStateBase> current_state{};
        Audio* parent;
        AudioStateEnum state_Enum{AudioStateEnum::Start};
    public:
        AudioStateMachine(Audio* p) : parent(p) {};
        /// <summary>
        /// <para> Called when AudioStateMachine is initialized </para>
        /// </summary>
        void Initialize();
        /// <summary>
        /// <para> Called to transition to a different state </para>
        /// </summary>
        void Transition(AudioStateEnum next_state);
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// </summary>
        void Execute();
        /// <summary>
        /// <para> Perform fade to the specified volume in the specified time</para>
        /// </summary>
        /// <param name="fade_time"> : Fade will be done in this. Unit is second</param>
        /// <param name="fade_vol"> : Fade volume target</param>
        void FadeTo(float fade_time = 1.0f, float fade_vol = 0.5f);
        /// <summary>
        /// Perform fade out on the volume within the specified time
        /// </summary>
        /// <param name="fade_time"></param>
        void FadeOut(float fade_time = 1.0f);
        /// <summary>
        /// <para> Returns the current audioStateEnum</para>
        /// </summary>
        /// <returns></returns>
        AudioStateEnum CurrentState();
        void DebugUI();
    };




}