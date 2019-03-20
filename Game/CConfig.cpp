#include "stdafx.h"
#include "CConfig.h"
#include <fstream>
#include <direct.h>

CConfig::CConfig()
{
}
CConfig::~CConfig()
{
}

void CConfig::Load() {
	std::ifstream ifs("Save\\Config.dat", std::ios::in | std::ios::binary);
	if (!ifs) { return; }
	ifs.read((char*)&m_data, sizeof(m_data));
}
void CConfig::Save() {
	_mkdir("Save");
	std::fstream fs("Save\\Config.dat", std::ios::out | std::ios::binary);
	if (!fs) { return; }
	fs.write((const char*)&m_data, sizeof(m_data));
}