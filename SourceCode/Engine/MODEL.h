#pragma once
#include "MODEL_RESOURCE.h"
#include "ResourceManager.h"
#include "DirectX11.h"
#include "../../External/ImGui/imgui.h"
#include "DXMath.h"
#define FRAMETIME 0.0166f
using namespace Math;
typedef MODEL_RESOURCES::ANIMATION AN;

/// <summary>
/// Model class, used for applying a model
/// </summary>
class MODEL
{
#pragma region VARIABLES

    bool animPaused{};
    bool isTransitioning{};

    std::shared_ptr<MODEL_RESOURCES>resource{};
    Vector3 scale{ 1, 1, 1 };        // Scale
    Vector3 rotation{};            // Rotation
    Vector3 translation{};         // Position


    Vector4 quaternion;
    XMFLOAT4X4 transform{};         // Float4X4 Transform Matrix
    XMMATRIX m_Transform{};         // Transform Matrix


    float cur_AnimTimer{};       // Timer to control animations
    float next_AnimTimer{};
    int cur_AnimationTake{ -1 };    // Current animation take that is being used, -1 if no animation is being used
    int next_AnimationTake{ -1 };
    int cur_Keyframe{};
    int next_Keyframe{};
    MODEL_RESOURCES::ANIMATION::KEYFRAME* kfs{};

    struct BOUNDING_BOX
    {
        XMFLOAT3 Min, Max;
    };

    std::vector<std::shared_ptr<BOUNDING_BOX>>Boxes;
    std::vector<std::string>animationTakes;

#pragma endregion
public:
    MODEL() {};

#pragma region MAIN FUNCTIONS

    /// <summary>
    /// Initializes the Model Resource by attempting to retrieve Resource from ModelResourceManager
    /// Also Creates a vector of the names of Animation to use when rendering
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize(std::string model_path);
    /// <summary>
    /// Uses the scale, rotation and translation members to form a transformation matrix and convert it to XMFLOAT4X4
    /// </summary>
    void UpdateTransform();
    /// <summary>
    /// Renders the model
    /// </summary>
    /// <param name="SamplingRate"> : Animation sampling rate, defaulted to Sampling Rate retrieved from FbxParameters if not stated otherwise</param>
    void Render(float SamplingRate = 0, XMFLOAT4 colour = { 1, 1, 1, 1 });
    /// <summary>
    /// For debug. Renders the wireframe of the model
    /// </summary>
    void RenderWireframe(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f });
#pragma endregion
#pragma region SUB FUNCTIONS

    /// <summary>
    /// Resets the animation timer to 0
    /// </summary>
    void ResetTimer();
    /// <summary>
    /// Returns true if the current animation has finished
    /// </summary>
    /// <param name="s"></param>
    bool FinishedAnim();
    /// <summary>
    /// Returns true if the current animation keyframe is between start and end
    /// </summary>
    /// <returns></returns>
    bool InAnim(int start, int end);
    /// <summary>
    /// Pauses the animation 
    /// </summary>
    void PauseAnim();
    /// <summary>
    /// Resumes the animation if paused
    /// </summary>
    void ResumeAnim();

#pragma endregion

#pragma region PARAMATER SETTING

    void SetTranslation(Vector3 t);
    void SetRotation(Vector3 r);
    void SetScale(Vector3 s);
    void SetTransformation(Vector3 s, Vector3 r, Vector3 t);
    void SetTransformation(Vector3 s, Vector4 q, Vector3 t);
    void OffsetTransform(XMMATRIX mat);
    void SetTake(int take);
    void SetFrame(int frame);

#pragma endregion
#pragma region PARAMETER RETRIEVAL

    /// <summary>
    /// Returns the current animation take
    /// </summary>
    int CurrentTake();
    /// <summary>
    /// Returns the current animation frame
    /// </summary>
    /// <returns></returns>
    int CurrentFrame();
    /// <summary>
    /// Returns the scale of the model
    /// </summary>
    /// <returns></returns>
    Vector3 Scale();
    /// <summary>
    /// Returns the rotation of the model
    /// </summary>
    /// <returns></returns>
    Vector3 Rotation();
    /// <summary>
    /// Returns the translation of the model
    /// </summary>
    /// <returns></returns>
    Vector3 Translation();
    /// <summary>
    /// Returns the rotation quaternion
    /// </summary>
    /// <returns></returns>
    //VECTOR4 Quaternion();
    /// <summary>
    /// Returns the Transform matrix in XMFLOAT4X4 form
    /// </summary>
    /// <returns></returns>
    XMFLOAT4X4 Transform();
    /// <summary>
    /// Returns a transform matrix for retrieving axises
    /// </summary>
    /// <returns></returns>
    XMMATRIX TransformMatrix();
    /// <summary>
    /// Returns an array of the names of the animation takes
    /// </summary>
    /// <returns></returns>
    std::vector<std::string>AnimationTakes();
    /// <summary>
    /// Returns the model resource
    /// </summary>
    /// <returns></returns>
    std::shared_ptr<MODEL_RESOURCES>Resource();
    /// <summary>
    /// Returns the coordinates of the bounding box
    /// </summary>
    /// <returns></returns>
    std::vector<std::shared_ptr<BOUNDING_BOX>>GetBB();

    /// <summary>
    /// Returns the Right Axis
    /// </summary>
    Vector3 Right();

    /// <summary>
    /// Returns the Up axis
    /// </summary>
    /// <returns></returns>
    Vector3 Up();

    /// <summary>
    /// Retrieves the forward axis
    /// </summary>
    /// <returns></returns>
    Vector3 Forward();
    /// <summary>
/// Retrieve the axises in XMFLOAT3 form
/// </summary>
/// <param name="r"> : The Right axis will be stored here</param>
/// <param name="u"> : The Up axis will be stored here</param>
/// <param name="f"> : The Forward axis will be stored here</param>
    void RetrieveAxisesQ(Vector3* r, Vector3* u, Vector3* f);
    /// <summary>
    /// Retrieves the next animation take
    /// </summary>
    /// <returns></returns>
    int NextTake() { return next_AnimationTake; }
    /// <summary>
    /// Retrueves the next animation keyframe
    /// </summary>
    /// <returns></returns>
    int NextFrame() { return next_Keyframe; }

#pragma endregion

};

