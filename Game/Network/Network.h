#pragma once

#include "ActionSender.h"

enum NetworkEventCode {
	enActionSender,
};

class NetActionSenderCaster : public IQSGameObject
{
public:

	void PostUpdate()override {
		if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }

		ExitGames::Common::Hashtable _event; 
		_event.put(static_cast<nByte>(1), m_actionSender.getMovement().x);
		_event.put(static_cast<nByte>(2), m_actionSender.getMovement().y);
		_event.put(static_cast<nByte>(3), static_cast<nByte>(m_actionSender.isJump()));
		_event.put(static_cast<nByte>(4), static_cast<nByte>(m_actionSender.isDash()));
		_event.put(static_cast<nByte>(5), static_cast<nByte>(m_actionSender.isShot()));
		_event.put(static_cast<nByte>(6), m_actionSender.getLookVec().x);
		_event.put(static_cast<nByte>(7), m_actionSender.getLookVec().y);
		_event.put(static_cast<nByte>(8), m_actionSender.getLookVec().z);

		GetPhoton()->Send(enActionSender, _event, true);
	}

	//送信するアクションセンダーを設定
	void SetActionSender(const ActionSender& actionsender) {
		m_actionSender = actionsender;
	}

private:
	ActionSender m_actionSender;
};

class NetWorkManager : public IGameObject{
public:
	NetWorkManager() {
		GetEngine().InitPhoton(L"3d457b93-aeb7-4fb7-b958-808dd8b623a4", L"Alpha",
			[&](int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
			switch (eventCode)
			{
			case enActionSender:
			{
				if (playerNr == 4) {
					playerNr = GetPhoton()->GetLocalPlayerNumber();
				}
				playerNr--;

				if (playerNr > 2) { break; }//プレイヤーが多すぎる

				ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
				
				if (eventContent.getValue((nByte)8)) {
					m_actionSenderReceiver[playerNr] = ActionSender(
						{
							((ExitGames::Common::ValueObject<float>*)(eventContent.getValue((nByte)1)))->getDataCopy(),
							((ExitGames::Common::ValueObject<float>*)(eventContent.getValue((nByte)2)))->getDataCopy()
						},
						((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)3)))->getDataCopy(),
						((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)4)))->getDataCopy(),
						{
							((ExitGames::Common::ValueObject<float>*)(eventContent.getValue((nByte)6)))->getDataCopy(),
							((ExitGames::Common::ValueObject<float>*)(eventContent.getValue((nByte)7)))->getDataCopy(),
							((ExitGames::Common::ValueObject<float>*)(eventContent.getValue((nByte)8)))->getDataCopy()
						},
						((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)5)))->getDataCopy()
					);
				}
			}
				break;
			default:
				break;
			}
		}
		);
	}

	//受信用アクションセンダーを取得
	ActionSender& GetReceiveActionSender(int index) {
		return m_actionSenderReceiver[index];
	}

	void ConnectJoin(const wchar_t* playerName) {
		if (!GetPhoton()->GetConnected()) {
			GetPhoton()->ConnectServer(playerName);
		}
	}
	void Update()override {
		if (GetPhoton()->GetState() == PhotonNetworkLogic::CONNECTED) {
			GetPhoton()->JoinRoom(L"ああああ", 2);
		}
	}

private:
	ActionSender m_actionSenderReceiver[3];
};

