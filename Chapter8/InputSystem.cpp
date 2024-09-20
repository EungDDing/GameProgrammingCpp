#include "InputSystem.h"
#include "SDL2/SDL.h"
#include <cstring>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const {
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const {
	if (mPrevState[keyCode] == 0) {
		if (mCurrState[keyCode] == 0) {
			return ENone;
		}
		else {
			return EPressed;
		}
	}
	else { // prev state must be 1
		if (mCurrState[keyCode] == 0) {
			return EReleased;
		}
		else {
			return EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const {
	return (SDL_BUTTON(button) & mCurrButtons);
}

ButtonState MouseState::GetButtonState(int button) const {
	int mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0) {
		if ((mask & mCurrButtons) == 0) {
			return ENone;
		}
		else {
			return EPressed;
		}
	}
	else {
		if ((mask & mCurrButtons) == 0) {
			return EReleased;
		}
		else {
			return EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const {
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const {
	if (mPrevButtons[button] == 0) {
		if (mCurrButtons[button] == 0) {
			return ENone;
		}
		else {
			return EPressed;
		}
	}
	else {
		if (mCurrButtons[button] == 0) {
			return EReleased;
		}
		else {
			return EHeld;
		}
	}
}

bool InputSystem::Initialize() {
	// keyboard
	// assign current state pointer
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	// clear previous state memory
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

	// mouse
	// just set everything to 0
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	// controller
	// get the connected controller, if it exists
	mController = SDL_GameControllerOpen(0);
	// initialize controller state
	mState.Controller.mIsConnected = (mController != nullptr);
	memset(mState.Controller.mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(mState.Controller.mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

	return true;
}

void InputSystem::Shutdown() {

}

void InputSystem::PrepareForUpdate() {
	// copy current state to previous
	// keyboard
	memcpy(mState.Keyboard.mPrevState, mState.Keyboard.mCurrState, SDL_NUM_SCANCODES);

	// mouse
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// controller
	memcpy(mState.Controller.mPrevButtons, mState.Controller.mCurrButtons, SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update() {
	// mouse
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative) {
		mState.Mouse.mCurrButtons = SDL_GetRelativeMouseState(&x, &y);
	}
	else {
		mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
	}

	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// controller
	// buttons
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
		mState.Controller.mCurrButtons[i] =
			SDL_GameControllerGetButton(mController, SDL_GameControllerButton(i));
	}

	// triggers
	mState.Controller.mLeftTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	mState.Controller.mRightTrigger =
		Filter1D(SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

	// sticks
	x = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_LEFTX);
	y = -SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_LEFTY);
	mState.Controller.mLeftStick = Filter2D(x, y);

	x = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_RIGHTX);
	y = -SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_RIGHTY);
	mState.Controller.mRightStick = Filter2D(x, y);
}

void InputSystem::ProcessEvent(SDL_Event& event) {
	switch (event.type) 
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;
	default:
		break;
	}
}

void InputSystem::SetRelativeMouseMode(bool value) {
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mState.Mouse.mIsRelative = value;
}

float InputSystem::Filter1D(int input) {
	// a value < dead zone is interpreted as 0%
	const int deadZone = 250;
	// a value > max value is interpreted as 100%
	const int maxValue = 30000;

	float retVal = 0.0f;

	// take absolute value of input
	int absValue = input > 0 ? input : -input;
	// ignore input within dead zone
	if (absValue > deadZone) {
		// compute fractional value between dead zone and max value
		retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);
		// make sure sign matches original value
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// clamp between -1.0f and 1.0f
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY) {
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// make into 2D vector
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// if length < deadZone, should be no input
	if (length < deadZone) {
		dir = Vector2::Zero;
	}
	else {
		// calculate fractional value between
		// dead zone and max value circle
		float f = (length - deadZone) / (maxValue - deadZone);
		// Clamp f between 0.0f and 1.0f
		f = Math::Clamp(f, 0.0f, 1.0f);
		// normalize the vector. and then scale it to the fractional value
		dir *= f / length;
	}

	return dir;
}