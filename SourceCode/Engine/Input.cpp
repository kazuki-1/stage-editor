#include "Input.h"
#include "MODEL.h"
#include "Camera.h"
#include "COLLISION.h"
#pragma region KEYSTATE

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------INPUTMANAGER::KEYSTATE Class-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------INPUTMANAGER::KEYSTATE CheckLastState()------------------------------------------*/

void InputManager::KEYSTATE::CheckLastState()
{

	if (last_code != code && code != 0)
	{
		last_code = code;
		triggered = true;
	}
	else
		triggered = false;
}

/*-------------------------------------------INPUTMANAGER::KEYSTATE CheckReleasedState()------------------------------------------*/

void InputManager::KEYSTATE::CheckReleasedState()
{
	released = code == 0 && last_code != 0 ? true : false;
	last_code = released ? 0 : last_code;
}

/*-------------------------------------------INPUTMANAGER::KEYSTATE Held()------------------------------------------*/

bool InputManager::KEYSTATE::Held()
{
    return held;
}

/*-------------------------------------------INPUTMANAGER::KEYSTATE Released()------------------------------------------*/

bool InputManager::KEYSTATE::Released()
{
    return released;
}

/*-------------------------------------------INPUTMANAGER::KEYSTATE Triggered()------------------------------------------*/

bool InputManager::KEYSTATE::Triggered()
{
    return triggered;
}

#pragma endregion
#pragma region KEYBOARD

/*-------------------------------------------INPUTMANAGER::KEYBOARD Execute()------------------------------------------*/

void InputManager::KEYBOARD::Execute()
{
#pragma region KEYSTATE SETTINGS

	for (auto& k : Keys)
	{
		if (k.last_code != k.code && k.code != 0)
		{
			k.triggered = true;
			k.last_code = k.code;
		}
		else
			k.triggered = false;
		if (k.code == 0 && k.last_code != k.code)
		{
			k.released = true;
			k.last_code = k.code;
		}
		else
			k.released = false;
		k.last_code = k.code;
	}

#pragma endregion

#pragma region AXISES

	if (Held('A') || Held(VK_LEFT))
		axisX.x = -1;
	else if (Held('D') || Held(VK_RIGHT))
		axisX.x = 1;
	else
		axisX.x = 0;

	// Axis X Y Axle
	if (Held('W') || Held(VK_UP))
		axisX.y = 1;
	else if (Held('S') || Held(VK_DOWN))
		axisX.y = -1;
	else
		axisX.y = 0;


	// Axis Y Settings

	// Axis Y X Axle
	if (Held('J'))
		axisY.x = -1;
	else if (Held('L'))
		axisY.x = 1;
	else
		axisY.x = 0;

	// Axis Y Y Axle
	if (Held('I'))
		axisY.y = 1;
	else if (Held('K'))
		axisY.y = -1;
	else
		axisY.y = 0;
	if (axisX.x || axisX.y)
		axisX.Normalize();
	if (axisY.x || axisY.y)
		axisY.Normalize();

#pragma endregion

}

/*-------------------------------------------INPUTMANAGER::KEYBOARD KeyDown()------------------------------------------*/

void InputManager::KEYBOARD::KeyDown(unsigned int k)
{
	Keys[k].held = true;
	Keys[k].code = k;
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD KeyUp()------------------------------------------*/

void InputManager::KEYBOARD::KeyUp(unsigned int k)
{
	Keys[k].released = true;
	Keys[k].held = false;
	Keys[k].code = 0;
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD Held()------------------------------------------*/

bool InputManager::KEYBOARD::Held(unsigned int k)
{
	return Keys[k].Held();
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD Released()------------------------------------------*/

bool InputManager::KEYBOARD::Released(unsigned int k)
{
	return Keys[k].Released();
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD Triggered()------------------------------------------*/

bool InputManager::KEYBOARD::Triggered(unsigned int k)
{
	return Keys[k].Triggered();
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD KeyState()------------------------------------------*/

InputManager::KEYSTATE InputManager::KEYBOARD::KeyState(unsigned int k)
{
	return Keys[k];
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD AxisX()------------------------------------------*/

Vector2 InputManager::KEYBOARD::AxisX()
{
	return axisX;
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD AxisY()------------------------------------------*/

Vector2 InputManager::KEYBOARD::AxisY()
{
	return axisY;
}

#pragma endregion
#pragma region MOUSE


/*-------------------------------------------INPUTMANAGER::MOUSE Up()------------------------------------------*/

InputManager::KEYSTATE InputManager::MOUSE::MOUSE_WHEEL::Up()
{
	return up;
}

/*-------------------------------------------INPUTMANAGER::MOUSE Down()------------------------------------------*/

InputManager::KEYSTATE InputManager::MOUSE::MOUSE_WHEEL::Down()
{
	return down;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnPressLButton()------------------------------------------*/

void InputManager::MOUSE::OnPressLButton()
{
	lButton.held = true;
	lButton.code = WM_LBUTTONDOWN;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnReleaseLButton()------------------------------------------*/

void InputManager::MOUSE::OnReleaseLButton()
{
	lButton.released = true;
	lButton.held = false;
	lButton.code = 0;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnPressRButton()------------------------------------------*/

void InputManager::MOUSE::OnPressRButton()
{
	rButton.held = true;
	rButton.code = WM_RBUTTONDOWN;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnReleaseRButton()------------------------------------------*/

void InputManager::MOUSE::OnReleaseRButton()
{
	rButton.released = true;
	rButton.held = false;
	rButton.code = 0;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnPressMButton()------------------------------------------*/

void InputManager::MOUSE::OnPressMButton()
{
	mButton.held = true;
	mButton.code = WM_MBUTTONDOWN;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnReleaseMButton()------------------------------------------*/

void InputManager::MOUSE::OnReleaseMButton()
{
	mButton.released = true;
	mButton.held = false;
	mButton.code = 0;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnMouseWheelUp()------------------------------------------*/

void InputManager::MOUSE::OnMousewheelUp()
{
	wheel.up.held = true;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnMouseWheelDown()------------------------------------------*/

void InputManager::MOUSE::OnMouseWheelDown()
{
	wheel.down.held = true;
}

/*-------------------------------------------INPUTMANAGER::MOUSE ResetState()------------------------------------------*/

void InputManager::MOUSE::ResetState()
{
	wheel.up.held = wheel.down.held = false;
	lButton.CheckReleasedState();
	rButton.CheckReleasedState();
	mButton.CheckReleasedState();
	lButton.CheckLastState();
	rButton.CheckLastState();
	mButton.CheckLastState();
}

/*-------------------------------------------INPUTMANAGER::MOUSE LButton()------------------------------------------*/

InputManager::KEYSTATE InputManager::MOUSE::LButton()
{
	return lButton;
}

/*-------------------------------------------INPUTMANAGER::MOUSE RButton()------------------------------------------*/

InputManager::KEYSTATE InputManager::MOUSE::RButton()
{
	return rButton;
}

/*-------------------------------------------INPUTMANAGER::MOUSE MButton()------------------------------------------*/

InputManager::KEYSTATE InputManager::MOUSE::MButton()
{
	return mButton;
}

/*-------------------------------------------INPUTMANAGER::MOUSE Wheel()------------------------------------------*/

InputManager::MOUSE::MOUSE_WHEEL InputManager::MOUSE::Wheel()
{
	return wheel;
}

/*-------------------------------------------INPUTMANAGER::MOUSE fPosition()------------------------------------------*/

Vector2 InputManager::MOUSE::fPosition()
{
	return { (float)position.x, (float)position.y };
}

/*-------------------------------------------INPUTMANAGER::MOUSE SetPosition()------------------------------------------*/

void InputManager::MOUSE::SetPosition(int x, int y)
{
	position = { x ,y };
}

#pragma endregion
#pragma region ALTKEY

/*-------------------------------------------INPUTMANAGER::ALTKEY OnPress()------------------------------------------*/

void InputManager::ALTKEY::OnPress()
{
	state.held = true;
}

/*-------------------------------------------INPUTMANAGER::ALTKEY OnRelease()------------------------------------------*/

void InputManager::ALTKEY::OnRelease()
{
	state.held = false;
	state.released = true;
}

/*-------------------------------------------INPUTMANAGER::ALTKEY State()------------------------------------------*/

InputManager::KEYSTATE InputManager::ALTKEY::State()
{
	return state;
}

#pragma endregion
#pragma region INPUTMANAGER


/*-------------------------------------------INPUTMANAGER Constructor------------------------------------------*/

InputManager::InputManager()
{
	keyboard = std::make_shared<KEYBOARD>();
	mouse = std::make_shared<MOUSE>();
	altKeys = std::make_shared<ALTKEY>();
};

/*-------------------------------------------INPUTMANAGER Initialize()------------------------------------------*/

void InputManager::Initialize()
{

}

/*-------------------------------------------INPUTMANAGER Execute()------------------------------------------*/

void InputManager::Execute()
{
	ResetState();
	keyboard->Execute();
}

/*-------------------------------------------INPUTMANAGER ResetState()------------------------------------------*/

void InputManager::ResetState()
{
	mouse->ResetState();
}

/*-------------------------------------------INPUTMANAGER MouseRayCast()------------------------------------------*/

bool InputManager::MouseRayCast(MODEL* m, D3D11_VIEWPORT vp)
{
	Vector3 m_pos;
	Vector3 start, end;
	m_pos = { mouse->fPosition().x, mouse->fPosition().y, 0.0f};
	start.Load(XMVector3Unproject(m_pos.XMV(), vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth, DirectX11::Instance()->ProjectionMatrix(), Camera::Instance()->ViewMatrix(), m->TransformMatrix()));
	m_pos.z = 1.0f;
	end.Load(XMVector3Unproject(m_pos.XMV(), vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth, DirectX11::Instance()->ProjectionMatrix(), Camera::Instance()->ViewMatrix(), m->TransformMatrix()));
	COLLIDERS::RAYCASTDATA hr{};
	if (COLLIDERS::RayCast(start, end, m, hr, -1))
		return true;
	return false;
}

/*-------------------------------------------INPUTMANAGER Keyboard()------------------------------------------*/

std::shared_ptr<InputManager::KEYBOARD> InputManager::Keyboard()
{
	return keyboard;
}

/*-------------------------------------------INPUTMANAGER Mouse()------------------------------------------*/

std::shared_ptr<InputManager::MOUSE> InputManager::Mouse()
{
	return mouse;
}

/*-------------------------------------------INPUTMANAGER AltKeys()------------------------------------------*/

std::shared_ptr<InputManager::ALTKEY>InputManager::AltKeys()
{
	return altKeys;
}
#pragma endregion