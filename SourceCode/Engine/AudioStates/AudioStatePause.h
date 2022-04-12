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
        /// <para> ���Υ��Ʃ`�Ȥ����w�ƤΤȤ��˺��ӳ�����</para>
        /// </summary>
        void Initialize(AUDIO* parent) override;
        /// <summary>
        /// <para> Called every frame to perform any functions </para>
        /// <para> ���ե�E`��˺��ӳ��� </para>
        /// </summary>
        void Execute(AUDIO* parent) override;
        /// <summary>
        /// <para>Called when transitioning away from this state </para>
        /// <para> ���Ʃ`���w�Ƥ΁E�˺��ӳ��� </para>
        /// </summary>
        void Finalize(AUDIO* parent) override;
    };
}