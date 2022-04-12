#pragma once
#include "../Engine/Singleton.h"
#include <map>
#include <memory>
#include <d3d11.h>
#include <string>
class AUDIOSTATE_BASE;
class AUDIO;
enum class AudioStates
{
    State_Idle, 
    State_Tension, 
    State_Climax, 
    State_Cooldown
};






class AudioController : public SINGLETON<AudioController>
{
    std::map<AudioStates, std::shared_ptr<AUDIO>>audioMap;
    std::shared_ptr<AUDIO>cur_BGM;
    std::shared_ptr<AUDIO>next_BGM;
    std::shared_ptr<AUDIO>ducking_target;
    bool pause{};
    bool isDucking{};
    bool transitioning{};

public:
    /// <summary>
    /// <para> Initializes the engine by inserting the audio into the map </para>
    /// <para> �I�[�f�B�I���}�b�v�ɓo�^����悤�ɃN���X������������</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Insert the audio and bind it to the state. The audio is retrieved from AUDIOENGINE, so perform AUDIOENGINE::Insert() before doing this </para>
    /// <para> AUDIOENGINE�}�b�v����AUDIO���������āAstate�Ƀo�C���h���� </para>
    /// </summary>
    /// <param name="state"> : The audio will be binded to this state</param>
    /// <param name="name"> : Name of audio. Will be retrieved from AUDIOENGINE</param>
    void Insert(AudioStates state, std::string name);
    /// <summary>
    /// Plays the current BGM
    /// </summary>
    void Enter();
    /// <summary>
    /// <para> Called every frame. Performs fadeIn and fadeOut effects of current and next bgm</para>
    /// <para> �eAudio�̃t�F�[�h�C���ƃt�F�[�h�A�E�g�G�t�F�N�g����������悤�ɖ��t���[���ɌĂяo��</para> 
    /// </summary>
    void Execute();
    /// <summary>
    /// Renders the Debug UI
    /// </summary>
    void DebugUI();
    /// <summary>
    /// Call at the end of scene
    /// </summary>
    void Finalize();
    /// <summary>
    /// Changes the current BGM to the next BGM
    /// </summary>
    void Exit();
    /// <summary>
    /// <para> Performs transition to the next audio </para>
    /// <para> ���̃I�[�f�B�I�Ƀg�����W�V��������</para>
    /// </summary>
    /// <param name="next_state"> : State to be transitioned to </param>
    void Transition(AudioStates next_state);
    /// <summary>
    /// <Para> Call in Execute() to perform fadeOut and pause </para> 
    /// <para> Execute()�ɌĂяo������t�F�[�h�A�E�g���ăI�[�f�B�I��Pause���� </para>
    /// </summary>
    void Pause();
    /// <summary>
    /// <para> Call in Execute() to perform fadeOut and resume</para> 
    /// <para> Execute()�ɌĂяo������t�F�[�h�A�E�g���ăI�[�f�B�I��Resume���� </para>
    /// </summary>
    void Resume();
    /// <summary>
    /// <para> Performs audio ducking. dock_target variable will be exempt </para>
    /// <para> ���`�ǥ������å��󥰤��Ф���dock_target�ϳ��⤵��E�E/para>
    /// </summary>
    /// <param name="dock_target"></param>
    void PerformDucking(std::shared_ptr<AUDIO>dock_target = {});
    /// <summary>
    /// <para> Stops audio ducking if is ducking. </para>
    /// <para> ���å��󥰤�ֹ�ᤁE</para>
    /// </summary>
    void StopDucking();
    /// <summary>
    /// <para> Returns true if audio is in ducking state </para>
    /// <para> ���`�ǥ����ϥ��å��󥰥��Ʃ`�ȤǤ���E�True�򷵤�</para>
    /// </summary>
    /// <returns></returns>
    bool IsDucking();
};