#pragma once
#include <memory>
#include <DirectXMath.h>
#include <Windows.h>
#include <Library/Singleton.h>
#include <Library/Math.h>
#include <d3d11.h>
using namespace DirectX;
using namespace Math;
class MODEL;



class InputManager : public Singleton<InputManager>
{
public:
	class KEYBOARD;
	class MOUSE;
	class ALTKEY;
	class KEYSTATE
	{
		friend class KEYBOARD;
		friend class MOUSE;
		friend class ALTKEY;
		bool held{};				// Returns true if the key is pressed
		bool triggered{};
		bool released{};			// Returns true if the key is released
		long code{};
		long last_code{};
	public:
		void CheckLastState();
		void CheckReleasedState();
		bool Held();
		bool Triggered();
		bool Released();

	};
	class KEYBOARD
	{
		KEYSTATE Keys[256]{};
		Vector2 axisX{}, axisY{};

	public:
		KEYBOARD() {};
		//void ResetPressedState();
		//void ResetReleasedState();
		void Execute();

		void KeyDown(unsigned int k);
		void KeyUp(unsigned int k);

		bool Held(unsigned int k);
		bool Released(unsigned int k);
		bool Triggered(unsigned int k);

		KEYSTATE KeyState(unsigned int k);
		Vector2 AxisX();
		Vector2 AxisY();

	};
	class MOUSE
	{
		struct POSITION
		{
			int x{}, y{};
		}position;
		class MOUSE_WHEEL
		{
			friend class MOUSE;
			KEYSTATE up, down;
		public:
			KEYSTATE Up();
			KEYSTATE Down();

		}wheel;
		KEYSTATE lButton{}, rButton{}, mButton{};

	public:
		MOUSE() {};

		void OnPressLButton();
		void OnReleaseLButton();

		void OnPressRButton();
		void OnReleaseRButton();

		void OnPressMButton();
		void OnReleaseMButton();

		void OnMousewheelUp();
		void OnMouseWheelDown();

		void ResetState();

		KEYSTATE LButton();
		KEYSTATE RButton();
		KEYSTATE MButton();

		KEYSTATE* pLButton() { return &lButton; }
		KEYSTATE* pRButton() { return &rButton; }
		KEYSTATE* pMButton() { return &mButton; }

		MOUSE_WHEEL Wheel();
		Vector2 fPosition();
		void SetPosition(int x, int y);
	};
	class ALTKEY
	{
		KEYSTATE state;
	public:
		ALTKEY() {}
		void OnPress();
		void OnRelease();
		KEYSTATE State();
	};

private:
	std::shared_ptr<KEYBOARD>keyboard;
	std::shared_ptr<MOUSE>mouse;
	std::shared_ptr<ALTKEY>altKeys;

public:

	InputManager();

	void Initialize();
	void Execute();
	void ResetState();
	bool MouseRayCast(MODEL* m, D3D11_VIEWPORT vp);

	std::shared_ptr<KEYBOARD> Keyboard();
	std::shared_ptr<MOUSE> Mouse();
	std::shared_ptr<ALTKEY>AltKeys();
};


namespace InputController
{
	enum class MBS
	{
		LB, RB, MB
	};
	inline bool Pressed(unsigned int key)
	{
		return InputManager::Instance()->Keyboard()->Held(key);
	}
	inline bool Triggered(unsigned int key)
	{
		return InputManager::Instance()->Keyboard()->Triggered(key);
	}
	inline bool Released(unsigned int key)
	{
		InputManager::Instance()->Keyboard()->Released(key);
	}
	inline bool Clicked(MBS mouseButton)
	{
		switch (mouseButton)
		{
		case MBS::LB:
			return InputManager::Instance()->Mouse()->LButton().Held();
		case MBS::RB:
			return InputManager::Instance()->Mouse()->RButton().Held();
		case MBS::MB:
			return InputManager::Instance()->Mouse()->MButton().Held();

		}
		return false;
	}
	inline bool Triggered(MBS mouseButton)
	{
		switch (mouseButton)
		{
		case MBS::LB:
			return InputManager::Instance()->Mouse()->LButton().Triggered();
		case MBS::RB:
			return InputManager::Instance()->Mouse()->RButton().Triggered();
		case MBS::MB:
			return InputManager::Instance()->Mouse()->MButton().Triggered();

		}
		return false;
	}
	inline bool Released(MBS mouseButton)
	{
		switch (mouseButton)
		{
		case MBS::LB:
			return InputManager::Instance()->Mouse()->LButton().Released();
		case MBS::RB:
			return InputManager::Instance()->Mouse()->RButton().Released();
		case MBS::MB:
			return InputManager::Instance()->Mouse()->MButton().Released();

		}
		return false;
	}

}


