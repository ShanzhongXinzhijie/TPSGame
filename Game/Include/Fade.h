#pragma once
/// <summary>
/// �t�F�[�h�C���E�t�F�[�h�A�E�g���s���N���X
/// </summary>
class Fade : public IGameObject{
public:
	Fade();
	~Fade();

	void PostRender() override;

	void fadeOut();

	/// <summary>
	/// �t�F�[�h�C��
	/// </summary>
	/// <remarks>�Ó]����������Ɏ��s�������������֐��I�u�W�F�N�g�Ƃ��ēn�����Ƃ��ł��܂�</remarks>
	/// <param name="function">�Ó]��������s���鏈��</param>
	void fadeIn(const std::function<void()>& function);

	/// <summary>
	/// �t�F�[�h�C���E�t�F�[�h�A�E�g���łȂ����true
	/// </summary>
	/// <returns></returns>
	bool isIdle() {
		return state == Idle;
	};

private:
	float alpha = 1.0f;//�����x
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

