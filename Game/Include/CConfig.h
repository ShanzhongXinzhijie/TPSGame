#pragma once
class CConfig
{
public:
	CConfig();
	~CConfig();

	void Load();
	void Save();

	struct ConfigData {
		bool lowGraphics = false;
	};

	ConfigData& GetConfigData() { return m_data; };

private:
	ConfigData m_data;
};

