#include "AudioStatePlay.h"
#include "../Audio.h"
using namespace AUDIO_STATES;
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStatePlay Class----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------AudioStatePlay Initialize()---------------------------------------------*/
/// <summary>
/// <para> Called when transitioned into this state </para>
/// <para> ���Υ��Ʃ`�Ȥ����w�ƤΤȤ��˺��ӳ�����</para>
/// </summary>
void AudioStatePlay::Initialize(AUDIO* parent)
{
    parent->Play();

    //float volume{ parent->Volume() };
    //float dist{ 1.0f - volume };
    //increment = dist / (fade_time * 60.0f);
}

/*----------------------------------------AudioStatePlay Execute()---------------------------------------------*/
/// <summary>
/// <para> Called every frame to perform any functions </para>
/// <para> ���ե�E`��˺��ӳ��� </para>
/// </summary>
void AudioStatePlay::Execute(AUDIO* parent)
{
    //float volume{ parent->Volume() };
    //if (volume < 1.0f)
    //    volume += increment;

}

/*----------------------------------------AudioStatePlay Finalize()---------------------------------------------*/
/// <summary>
/// <para>Called when transitioning away from this state </para>
/// <para> ���Ʃ`���w�Ƥ΁E�˺��ӳ��� </para>
/// </summary>
void AudioStatePlay::Finalize(AUDIO* parent)
{

}
