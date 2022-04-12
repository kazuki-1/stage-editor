#pragma once
#include <map>
#include <memory>

// Audio state machine, internally used in AUDIO class
class AUDIO;
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
        /// <para> ﾋ訷ﾎ･ｹ･ﾆｩ`･ﾈ､ｫ､鱇wﾒﾆ､ﾎ､ﾈ､ｭ､ﾋｺﾓｳｹ｡｡</para>
        /// </summary>
        virtual void Initialize(AUDIO* parent) {};
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// <para> 垈･ﾕ･・`･爨ﾋｺﾓｳｹ </para>
        /// </summary>
        virtual void Execute(AUDIO* parent) {};
        /// <summary>
        /// <para>Called when transitioning away from this state </para>
        /// <para> ･ｹ･ﾆｩ`･ﾈﾟwﾒﾆ､ﾎ・､ﾋｺﾓｳｹ </para>
        /// </summary>
        virtual void Finalize(AUDIO* parent) {};
    };


    class AudioStateMachine
    {
        std::map<AudioStateEnum, std::shared_ptr<AudioStateBase>>audioStates;
        std::shared_ptr<AudioStateBase> current_state{};
        AUDIO* parent;
        AudioStateEnum state_Enum{AudioStateEnum::Start};
    public:
        AudioStateMachine(AUDIO* p) : parent(p) {};
        /// <summary>
        /// <para> Called when AudioStateMachine is initialized </para>
        /// <para> ･ｹ･ﾆｩ`･ﾈ･ﾞ･ｷ･ｬﾉ嵭ﾉ瓶､ﾋｺﾓｳｹ </para>
        /// </summary>
        void Initialize();
        /// <summary>
        /// <para> Called to transition to a different state </para>
        /// <para> ･ｹ･ﾆｩ`･ﾈ､wﾒﾆ､ｹ､・｡</para>
        /// </summary>
        void Transition(AudioStateEnum next_state);
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// <para> 垈･ﾕ･・`･爨ﾋｺﾓｳｹ </para>
        /// </summary>
        void Execute();
        /// <summary>
        /// <para> Perform fade to the specified volume in the specified time</para>
        /// <para> ﾖｸｶｨ､ｵ､・ｿﾒｿ､ｸｶｨ､ｵ､・ｿ瓶馮､ﾋ･ﾕ･ｧｩ`･ﾉ </para>
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
        /// <para> ｬFﾔﾚ､ﾎ･ｹ･ﾆｩ`･ﾈEnum､ｵ､ｹ </para>
        /// </summary>
        /// <returns></returns>
        AudioStateEnum CurrentState();
        void DebugUI();
    };




}