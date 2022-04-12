#pragma once
#include "AudioStates.h"

namespace AUDIO_STATES
{
    class AudioStatePause : public AudioStateBase
    {
        float fade_time{ 1.0f };
        float decrement{};
    public:
        AudioStatePause() {};
        /// <summary>
        /// <para> Called when transitioned into this state </para>
        /// <para> 他のステートから遷移のときに呼び出す　</para>
        /// </summary>
        void Initialize(AUDIO* parent) override;
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// <para> 毎フE`ムに呼び出す </para>
        /// </summary>
        void Execute(AUDIO* parent) override;
        /// <summary>
        /// <para>Called when transitioning away from this state </para>
        /// <para> ステート遷移の丒に呼び出す </para>
        /// </summary>
        void Finalize(AUDIO* parent) override;
    };
}