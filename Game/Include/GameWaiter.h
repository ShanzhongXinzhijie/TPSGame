#pragma once
class GameWaiter
{
public:
	GameWaiter();
	~GameWaiter();

	static bool GetIsWait() { return m_isWait; }
	static void SetIsWait(bool isWait) { m_isWait = isWait; }

private:
	static bool m_isWait;
};

