#pragma once
#include "AudioStateFade.h"

namespace AUDIO_STATES
{
    class AudioStateFadeOut : public AudioStateFade
    {
        /// <summary>
        /// Called when transitioning to this state
        /// </summary>
        void Initialize(AUDIO* parent) override;
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// <para> ö∞•’•ÅE`•‡§À∫Ù§”≥ˆ§π </para>
        /// </summary>
        void Execute(AUDIO* parent) override;

    };
}