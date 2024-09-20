#pragma once
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_gamecontroller.h"
#include "SDL2/SDL_mouse.h"
#include "Math.h"

enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
};

// helper for keyboard input
class KeyboardState {
public:
	// friend so InputSystem can easily update it
	friend class InputSystem;
	// get just the boolean true/false value of key
	bool GetKeyValue(SDL_Scancode keyCode) const;
	// get a state based on current and previous frame
	ButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
	const Uint8* mCurrState;
	Uint8* mPrevState[SDL_NUM_SCANCODES];
};

// helper for mouse input
class MouseState {
public:
	friend class InputSystem;

	// for mouse position
	const Vector2& GetPosition() const { return mMousePos; }
	const Vector2& GetScrollWheel() const { return mScrollWheel; }
	bool IsRelative() const { return mIsRelative; }

	// for buttons
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;
private:
	// store current mouse position
	Vector2 mMousePos;
	// motion of scroll wheel
	Vector2 mScrollWheel;
	// store button date
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	// are we ion relative mouse mode
	bool mIsRelative;
};


// helper for controller input
class ControllerState {
public:
	friend class InputSystem;

	// for buttons
	bool GetButtonValue(SDL_GameControllerButton button) const;
	ButtonState GetButtonState(SDL_GameControllerButton button) const;

	const Vector2& GetLeftStick() const { return mLeftStick; }
	const Vector2& GetRightStick() const { return mRightStick; }
	float GetLeftTrigger() const { return mLeftTrigger; }
	float GetRightTrigger() const { return mRightTrigger; }

	bool GetIsConnected() const { return mIsConnected; }
private:
	// current/previous buttons
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	// left/right sticks
	Vector2 mLeftStick;
	Vector2 mRightStick;
	// left/right trigger
	float mLeftTrigger;
	float mRightTrigger;
	// is this controller connected
	bool mIsConnected;
};

struct InputState
{
	KeyboardState Keyboard;
	MouseState Mouse;
	ControllerState Controller;
};

class InputSystem {
public:
	bool Initialize();
	void Shutdown();

	// called right before SDL_PollEvents loop
	void PrepareForUpdate();
	// called after SDL_PollEvents loop
	void Update();
	// called to process an SDL event in input system
	void ProcessEvent(union SDL_Event& event);

	const InputState& GetState() const { return mState; }

	void SetRelativeMouseMode(bool value);
private:
	float Filter1D(int input);
	Vector2 Filter2D(int inputX, int inputY);
	InputState mState;
	SDL_GameController* mController;
};