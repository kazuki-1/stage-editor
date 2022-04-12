#pragma once
#include "AudioStates.h"

namespace AUDIO_STATES
{
    class AudioStateFade : public AudioStateBase
    {
    protected:
        float fade_target{ 0.5f };   // fade in will fade to this volume
        float fade_in_time{ 1.0f };     // Fade in will occur within this time, measured in seconds
        float fade_increment{};      // Volume will increase or decrease by this amount when fading
    public:
        AudioStateFade() {};
        /// <summary>
        /// <para> Called when transitioned into this state </para>
        /// <para> ﾋ訷ﾎ･ｹ･ﾆｩ`･ﾈ､ｫ､鱇wﾒﾆ､ﾎ､ﾈ､ｭ､ﾋｺﾓｳｹ｡｡</para>
        /// </summary>
        void Initialize(AUDIO* parent) override;
        /// <summary>
        /// <para>Called when transitioning away from this state </para>
        /// <para> ･ｹ･ﾆｩ`･ﾈﾟwﾒﾆ､ﾎ・､ﾋｺﾓｳｹ </para>
        /// </summary>
        void Finalize(AUDIO* parent) override;
        /// <summary>
        /// <para> Sets the volume to be faded to </para>
        /// <para> ･ﾕ･ｧｩ`･ﾉ･ﾜ･・蟀`･爨Oｶｨ､ｹ､・</para>
        /// </summary>
        void SetTargetVolume(float vol);
        /// <summary>
        /// <para> Sets the fade timer</para>
        /// <para> ･ﾕ･ｧｩ`･ﾉ･ｿ･､･爨Oｶｨ､ｹ､・/para>
        /// </summary>
        void SetTime(float t);
        /// <summary>
        /// <para> Calculates the rate of fading. Call after SetVolume and SetTime</para>
        /// <para> ･ﾕ･ｧｩ`･ﾉﾋﾙｶﾈ､桐网ｹ､・｣SetVolume､ﾈSetTimeｺﾓｳｷ､ﾆ､ｫ､鮗ﾓｳｹ </para>
        /// </summary>
        void CalculateRate(AUDIO* parent);
    };
}