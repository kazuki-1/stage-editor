#include "Input.h"
#include "MODEL.h"
#include "Camera.h"
#include "COLLISION.h"
#pragma region KEYSTATE

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------INPUTMANAGER::KEYSTATE Class-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------INPUTMANAGER::KEYSTATE CheckLastState()------------------------------------------*/

void INPUTMANAGER::KEYSTATE::CheckLastState()
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

void INPUTMANAGER::KEYSTATE::CheckReleasedState()
{
	released = code == 0 && last_code != 0 ? true : false;
	last_code = released ? 0 : last_code;
}

/*-------------------------------------------INPUTMANAGER::KEYSTATE Held()------------------------------------------*/

bool INPUTMANAGER::KEYSTATE::Held()
{
    return held;
}

/*-------------------------------------------INPUTMANAGER::KEYSTATE Released()------------------------------------------*/

bool INPUTMANAGER::KEYSTATE::Released()
{
    return released;
}

/*-------------------------------------------INPUTMANAGER::KEYSTATE Triggered()------------------------------------------*/

bool INPUTMANAGER::KEYSTATE::Triggered()
{
    return triggered;
}

#pragma endregion
#pragma region KEYBOARD

/*-------------------------------------------INPUTMANAGER::KEYBOARD Execute()------------------------------------------*/

void INPUTMANAGER::KEYBOARD::Execute()
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

void INPUTMANAGER::KEYBOARD::KeyDown(unsigned int k)
{
	Keys[k].held = true;
	Keys[k].code = k;
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD KeyUp()------------------------------------------*/

void INPUTMANAGER::KEYBOARD::KeyUp(unsigned int k)
{
	Keys[k].released = true;
	Keys[k].held = false;
	Keys[k].code = 0;
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD Held()------------------------------------------*/

bool INPUTMANAGER::KEYBOARD::Held(unsigned int k)
{
	return Keys[k].Held();
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD Released()------------------------------------------*/

bool INPUTMANAGER::KEYBOARD::Released(unsigned int k)
{
	return Keys[k].Released();
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD Triggered()------------------------------------------*/

bool INPUTMANAGER::KEYBOARD::Triggered(unsigned int k)
{
	return Keys[k].Triggered();
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD KeyState()------------------------------------------*/

INPUTMANAGER::KEYSTATE INPUTMANAGER::KEYBOARD::KeyState(unsigned int k)
{
	return Keys[k];
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD AxisX()------------------------------------------*/

Vector2 INPUTMANAGER::KEYBOARD::AxisX()
{
	return axisX;
}

/*-------------------------------------------INPUTMANAGER::KEYBOARD AxisY()------------------------------------------*/

Vector2 INPUTMANAGER::KEYBOARD::AxisY()
{
	return axisY;
}

#pragma endregion
#pragma region MOUSE


/*-------------------------------------------INPUTMANAGER::MOUSE Up()------------------------------------------*/

INPUTMANAGER::KEYSTATE INPUTMANAGER::MOUSE::MOUSE_WHEEL::Up()
{
	return up;
}

/*-------------------------------------------INPUTMANAGER::MOUSE Down()------------------------------------------*/

INPUTMANAGER::KEYSTATE INPUTMANAGER::MOUSE::MOUSE_WHEEL::Down()
{
	return down;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnPressLButton()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnPressLButton()
{
	lButton.held = true;
	lButton.code = WM_LBUTTONDOWN;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnReleaseLButton()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnReleaseLButton()
{
	lButton.released = true;
	lButton.held = false;
	lButton.code = 0;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnPressRButton()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnPressRButton()
{
	rButton.held = true;
	rButton.code = WM_RBUTTONDOWN;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnReleaseRButton()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnReleaseRButton()
{
	rButton.released = true;
	rButton.held = false;
	rButton.code = 0;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnPressMButton()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnPressMButton()
{
	mButton.held = true;
	mButton.code = WM_MBUTTONDOWN;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnReleaseMButton()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnReleaseMButton()
{
	mButton.released = true;
	mButton.held = false;
	mButton.code = 0;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnMouseWheelUp()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnMousewheelUp()
{
	wheel.up.held = true;
}

/*-------------------------------------------INPUTMANAGER::MOUSE OnMouseWheelDown()------------------------------------------*/

void INPUTMANAGER::MOUSE::OnMouseWheelDown()
{
	wheel.down.held = true;
}

/*-------------------------------------------INPUTMANAGER::MOUSE ResetState()------------------------------------------*/

void INPUTMANAGER::MOUSE::ResetState()
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

INPUTMANAGER::KEYSTATE INPUTMANAGER::MOUSE::LButton()
{
	return lButton;
}

/*-------------------------------------------INPUTMANAGER::MOUSE RButton()------------------------------------------*/

INPUTMANAGER::KEYSTATE INPUTMANAGER::MOUSE::RButton()
{
	return rButton;
}

/*-------------------------------------------INPUTMANAGER::MOUSE MButton()------------------------------------------*/

INPUTMANAGER::KEYSTATE INPUTMANAGER::MOUSE::MButton()
{
	return mButton;
}

/*-------------------------------------------INPUTMANAGER::MOUSE Wheel()------------------------------------------*/

INPUTMANAGER::MOUSE::MOUSE_WHEEL INPUTMANAGER::MOUSE::Wheel()
{
	return wheel;
}

/*-------------------------------------------INPUTMANAGER::MOUSE fPosition()------------------------------------------*/

Vector2 INPUTMANAGER::MOUSE::fPosition()
{
	return { (float)position.x, (float)position.y };
}

/*-------------------------------------------INPUTMANAGER::MOUSE SetPosition()------------------------------------------*/

void INPUTMANAGER::MOUSE::SetPosition(int x, int y)
{
	position = { x ,y };
}

#pragma endregion
#pragma region ALTKEY

/*-------------------------------------------INPUTMANAGER::ALTKEY OnPress()------------------------------------------*/

void INPUTMANAGER::ALTKEY::OnPress()
{
	state.held = true;
}

/*-------------------------------------------INPUTMANAGER::ALTKEY OnRelease()------------------------------------------*/

void INPUTMANAGER::ALTKEY::OnRelease()
{
	state.held = false;
	state.released = true;
}

/*-------------------------------------------INPUTMANAGER::ALTKEY State()------------------------------------------*/

INPUTMANAGER::KEYSTATE INPUTMANAGER::ALTKEY::State()
{
	return state;
}

#pragma endregion
#pragma region INPUTMANAGER


/*-------------------------------------------INPUTMANAGER Constructor------------------------------------------*/

INPUTMANAGER::INPUTMANAGER()
{
	keyboard = std::make_shared<KEYBOARD>();
	mouse = std::make_shared<MOUSE>();
	altKeys = std::make_shared<ALTKEY>();
};

/*-------------------------------------------INPUTMANAGER Initialize()------------------------------------------*/

void INPUTMANAGER::Initialize()
{

}

/*-------------------------------------------INPUTMANAGER Execute()------------------------------------------*/

void INPUTMANAGER::Execute()
{
	ResetState();
	keyboard->Execute();
}

/*-------------------------------------------INPUTMANAGER ResetState()------------------------------------------*/

void INPUTMANAGER::ResetState()
{
	mouse->ResetState();
}

/*-------------------------------------------INPUTMANAGER MouseRayCast()------------------------------------------*/

bool INPUTMANAGER::MouseRayCast(MODEL* m, D3D11_VIEWPORT vp)
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

std::shared_ptr<INPUTMANAGER::KEYBOARD> INPUTMANAGER::Keyboard()
{
	return keyboard;
}

/*-------------------------------------------INPUTMANAGER Mouse()------------------------------------------*/

std::shared_ptr<INPUTMANAGER::MOUSE> INPUTMANAGER::Mouse()
{
	return mouse;
}

/*-------------------------------------------INPUTMANAGER AltKeys()------------------------------------------*/

std::shared_ptr<INPUTMANAGER::ALTKEY>INPUTMANAGER::AltKeys()
{
	return altKeys;
}
#pragma endregion