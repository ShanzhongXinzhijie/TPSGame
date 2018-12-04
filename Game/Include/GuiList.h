#pragma once
class GuiList {
public:
	GuiList(const wchar_t* backImagePath);
	~GuiList();
	
	void Draw();

	CVector2 position = { 0.5f, 0.5f };
	CVector2 scale = CVector2::One();
	CVector2 pivot = {0.5f, 0.5f};
	CVector4 color = CVector4::White();

	float side = 0.0f;

	std::vector<const wchar_t*> values;
private:
	CSprite back;
	CFont font;
};

