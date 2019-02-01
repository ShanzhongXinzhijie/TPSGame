#pragma once

class NetMasterClientCaster :
	public IQSGameObject
{
public:
	NetMasterClientCaster();
	~NetMasterClientCaster();

	void PostUpdate() override;

private:
	bool m_isChangeSetting = false;
};

