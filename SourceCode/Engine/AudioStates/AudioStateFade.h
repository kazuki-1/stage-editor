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
        /// </summary>
        void Initialize(Audio* parent) override;
        /// <summary>
        /// <para>Called when transitioning away from this state </para>
        /// </summary>
        void Finalize(Audio* parent) override;
        /// <summary>
        /// <para> Sets the volume to be faded to </para>
        /// </summary>
        void SetTargetVolume(float vol);
        /// <summary>
        /// <para> Sets the fade timer</para>
        /// </summary>
        void SetTime(float t);
        /// <summary>
        /// <para> Calculates the rate of fading. Call after SetVolume and SetTime</para>
        /// </summary>
        void CalculateRate(Audio* parent);
    };
}