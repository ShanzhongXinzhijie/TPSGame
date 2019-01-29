#pragma once
class Fade : public IGameObject{
public:
	Fade();
	~Fade();

	void PostRender() override;

	void fadeOut();
	void fadeIn(const std::function<void()>& function);

private:
	float alpha = 1.0f;
	enum State {
		In,
		Out,
		Idle
	};
	State state = Out;
	static constexpr float fadePerSec = 1.0f;

	CSprite sprite;
	std::function<void()> func;
};

