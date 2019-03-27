#pragma once
/// <summary>
/// フェードイン・フェードアウトを行うクラス
/// </summary>
class Fade : public IGameObject{
public:
	Fade();
	~Fade();

	void PostRender() override;

	void fadeOut();

	/// <summary>
	/// フェードイン
	/// </summary>
	/// <remarks>暗転しきった後に実行したい処理を関数オブジェクトとして渡すことができます</remarks>
	/// <param name="function">暗転した後実行する処理</param>
	void fadeIn(const std::function<void()>& function);

	/// <summary>
	/// フェードイン・フェードアウト中でなければtrue
	/// </summary>
	/// <returns></returns>
	bool isIdle() {
		return state == Idle;
	};

private:
	float alpha = 1.0f;//透明度
	enum State {
		In,
		Out,
		Idle
	};
	State state = Out;
	static constexpr float fadePerSec = 2.0f;

	CSprite sprite;
	std::function<void()> func;
};

