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






class AudioController : public Singleton<AudioController>
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
    /// <para> オーディオをマップに登録するようにクラスを初期化する</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Insert the audio and bind it to the state. The audio is retrieved from AudioEngine, so perform AudioEngine::Insert() before doing this </para>
    /// <para> AUDIOENGINEマップからAUDIOを検索して、stateにバインドする </para>
    /// </summary>
    /// <param name="state"> : The audio will be binded to this state</param>
    /// <param name="name"> : Name of audio. Will be retrieved from AudioEngine</param>
    void Insert(AudioStates state, std::string name);
    /// <summary>
    /// <para> Called every frame. Performs fadeIn and fadeOut effects of current and next bgm</para>
    /// <para> 各Audioのフェードインとフェードアウトエフェクトを処理するように毎フレームに呼び出す</para> 
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
    /// Called when transitioning into this state
    /// </summary>
    void Enter();
    /// <summary>
    /// Changes the current BGM to the next BGM
    /// </summary>
    void Exit();
    /// <summary>
    /// <para> Performs transition to the next audio </para>
    /// <para> 次のオーディオにトランジションする</para>
    /// </summary>
    /// <param name="next_state"> : State to be transitioned to </param>
    void Transition(AudioStates next_state);
    /// <summary>
    /// <Para> Call in Execute() to perform fadeOut and pause </para> 
    /// <para> Execute()に呼び出したらフェードアウトしてオーディオをPauseする </para>
    /// </summary>
    void Pause();
    /// <summary>
    /// <para> Call in Execute() to perform fadeOut and resume</para> 
    /// <para> Execute()に呼び出したらフェードアウトしてオーディオをResumeする </para>
    /// </summary>
    void Resume();
    /// <summary>
    /// <para> Performs audio ducking. dock_target variable will be exempt </para>
    /// </summary>
    /// <param name="dock_target"></param>
    void PerformDucking(std::shared_ptr<AUDIO>dock_target = {});
    /// <summary>
    /// <para> Stops audio ducking if is ducking. </para>
    /// </summary>
    void StopDucking();
    /// <summary>
    /// <para> Returns true if audio is in ducking state </para>
    /// </summary>
    /// <returns></returns>
    bool IsDucking();
};