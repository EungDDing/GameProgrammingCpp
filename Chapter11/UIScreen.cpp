#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"


UIScreen::UIScreen(Game* game)
	:mGame(game)
	, mTitle(nullptr)
	, mBackground(nullptr)
	, mTitlePos(0.0f, 300.0f)
	, mNextButtonPos(0.0f, 200.0f)
	, mBGPos(0.0f, 250.0f)
	, mState(EActive)
{
	// add to UI stack
	mGame->PushUI(this);
	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
	mButtonOn = mGame->GetRenderer()->GetTexture("Assets/ButtonYellow.png");
	mButtonOff = mGame->GetRenderer()->GetTexture("Assets/ButtonBlue.png");
}

UIScreen::~UIScreen()
{
	if (mTitle) {
		mTitle->Unload();
		delete mTitle;
	}

	for (auto b : mButtons) {
		delete b;
	}
	mButtons.clear();
}

void UIScreen::Update(float deltaTime) {

}

void UIScreen::Draw(Shader* shader) {
	// draw background (if exists)
	if (mBackground) {
		DrawTexture(shader, mBackground, mBGPos);
	}
	// draw title (if exists)
	if (mTitle) {
		DrawTexture(shader, mTitle, mTitlePos);
	}
	// draw buttons
	for (auto b : mButtons) {
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());
		DrawTexture(shader, b->GetNameTex(), b->GetPosition());
	}
}

void UIScreen::ProcessInput(const uint8_t* keys) {
	if (!mButtons.empty()) {
		// get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// convert to (0, 0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

		// highlight any buttons
		for (auto b : mButtons) {
			if (b->ContainsPoint(mousePos)) {
				b->SetHighlighted(true);
			}
			else {
				b->SetHighlighted(false);
			}
		}
	}
}

void UIScreen::HandleKeyPress(int key) {
	switch (key) {
	case SDL_BUTTON_LEFT:
		if (!mButtons.empty()) {
			for (auto b : mButtons) {
				if (b->GetHighlighted()) {
					b->OnClick();
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

void UIScreen::Close() {
	mState = EClosing;
}

void UIScreen::SetTitle(const std::string& text, const Vector3& color, int pointSize) {
	// clear out previous title texture if it exists
	if (mTitle) {
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}
	mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::AddButton(const std::string& name, std::function<void()> onClick) {
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims);
	mButtons.emplace_back(b);

	// update position of next button
	// move down by height of button plus padding
	mNextButtonPos.y -= mButtonOff->GetHeight() + 20.0f;
}
void UIScreen::DrawTexture(class Shader* shader, class Texture* texture, const Vector2& offset, float scale) {
	// scale the quad by the width/height of texture
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale,
		static_cast<float>(texture->GetHeight()) * scale,
		1.0f);
	// translate to position on screen
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));
	// set world transform
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// set current texture
	texture->SetActive();
	// draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool relative) {
	if (relative) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// make an initial call to get relative to clear out
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else {
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

Button::Button(const std::string& name, Font* font,
	std::function<void()> onClick,
	const Vector2& pos, const Vector2& dims)
	:mOnClick(onClick)
	, mNameTex(nullptr)
	, mFont(font)
	, mPosition(pos)
	, mDimensions(dims)
	, mHighlighted(false)
{
	SetName(name);
}

Button::~Button()
{
	if (mNameTex) {
		mNameTex->Unload();
		delete mNameTex;
	}
}

void Button::SetName(const std::string& name) {
	mName = name;

	if (mNameTex) {
		mNameTex->Unload();
		delete mNameTex;
		mNameTex = nullptr;
	}
	mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2& pt) const {
	bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
		pt.x >(mPosition.x + mDimensions.x / 2.0f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
		pt.y >(mPosition.y + mDimensions.y / 2.0f);
	return !no;
}

void Button::OnClick() {
	// call attach handler, if it exists
	if (mOnClick) {
		mOnClick();
	}
}