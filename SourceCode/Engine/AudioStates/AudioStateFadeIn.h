#pragma once
#include "AudioStateFade.h"

namespace AUDIO_STATES
{
    // This state is used transitioning from paused to play
	class AudioStateFadeIn : public AudioStateFade
	{
        /// <summary>
        /// Called when transitioning to this state
        /// </summary>
        void Initialize(Audio* parent) override;
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// </summary>
        void Execute(Audio* parent) override;

	};
}