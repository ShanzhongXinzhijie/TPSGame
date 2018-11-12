#pragma once
class ActionSender {
public:
	ActionSender(CVector2 movement = {0, 0}, bool jump = false, bool dash = false);
	~ActionSender();

	CVector2 getMovement() const{
		return movement;
	}

	bool isJump() const{
		return jump;
	}

	bool isDash() const {
		return dash;
	}

private:
	CVector2 movement;
	bool jump;
	bool dash;
};

