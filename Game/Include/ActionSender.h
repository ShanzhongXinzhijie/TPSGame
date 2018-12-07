#pragma once
class ActionSender {
public:
	ActionSender(CVector2 movement = {0, 0},
				 bool jump = false,
				 bool dash = false,
				 CVector3 lookVector = {0,0,0},
				 bool shot = false);
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

	bool isShot() const {
		return shot;
	}

	CVector3 getLookVec() const {
		return lookVec;
	}

private:
	CVector2 movement;
	bool jump;
	bool dash;
	CVector3 lookVec;
	bool shot;
};

