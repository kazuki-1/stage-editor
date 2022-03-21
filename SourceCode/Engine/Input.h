#pragma once
#include <memory>
#include "Singleton.h"
#include <DirectXMath.h>
#include <Windows.h>
#include "DXMath.h"
#include <d3d11.h>
using namespace DirectX;
using namespace Math;
class MODEL;



class INPUTMANAGER : public SINGLETON<INPUTMANAGER>
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
		VECTOR2 axisX{}, axisY{};

	public:
		KEYBOARD() {};
		void ResetPressedState();
		void ResetReleasedState();
		void Execute();

		void KeyDown(unsigned int k);
		void KeyUp(unsigned int k);

		bool Held(unsigned int k);
		bool Released(unsigned int k);
		bool Triggered(unsigned int k);

		KEYSTATE KeyState(unsigned int k);
		VECTOR2 AxisX();
		VECTOR2 AxisY();

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
		VECTOR2 fPosition();
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

	INPUTMANAGER();

	void Initialize();
	void Execute();
	void ResetState();
	void DragMousePosition(VECTOR2* v, KEYSTATE* k);
	bool MouseRayCast(MODEL* m, D3D11_VIEWPORT vp);

	std::shared_ptr<KEYBOARD> Keyboard();
	std::shared_ptr<MOUSE> Mouse();
	std::shared_ptr<ALTKEY>AltKeys();
};


namespace IN_BOOL
{
	enum MBS
	{
		LB, RB, MB
	};
	inline bool Pressed(unsigned int key)
	{
		return INPUTMANAGER::Instance()->Keyboard()->Held(key);
	}
	inline bool Released(unsigned int key)
	{
		INPUTMANAGER::Instance()->Keyboard()->Released(key);
	}
	inline bool Clicked(MBS mb)
	{
		switch (mb)
		{
		case LB:
			return INPUTMANAGER::Instance()->Mouse()->LButton().Held();
		case RB:
			return INPUTMANAGER::Instance()->Mouse()->RButton().Held();
		case MB:
			return INPUTMANAGER::Instance()->Mouse()->MButton().Held();

		}
	}
	inline bool Released(MBS mb)
	{
		switch (mb)
		{
		case LB:
			return INPUTMANAGER::Instance()->Mouse()->LButton().Released();
		case RB:
			return INPUTMANAGER::Instance()->Mouse()->RButton().Released();
		case MB:
			return INPUTMANAGER::Instance()->Mouse()->MButton().Released();

		}
	}

}