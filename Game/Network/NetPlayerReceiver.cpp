#include "stdafx.h"
#include "NetPlayerReceiver.h"


NetPlayerReceiver::NetPlayerReceiver()
{
}
NetPlayerReceiver::~NetPlayerReceiver()
{
}

void NetPlayerReceiver::EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
	//プレイヤー関係のイベントをキューに保存
	if (eventCode > enPlayerEventStart && eventCode < enPlayerEventEnd) {
		ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
		//フレーム数
		if (eventContent.getValue((nByte)enFrameCount)) {
			int frmcnt = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enFrameCount)))->getDataCopy();
			m_eventContentQueue[playerNr].push(std::make_tuple((NetworkEventCode)eventCode, frmcnt, eventContent));
		}
	}
}
	
void NetPlayerReceiver::RunEvent(int playerNr, bool frameSkip){

	bool isFrameSkipOn = false;

	while(!m_eventContentQueue[playerNr].empty()){
	
	//いっことりだし
	int frmcnt = std::get<1>(m_eventContentQueue[playerNr].front());

	//フレームスキップ
	if (frameSkip) {
		const int FRAMESKIP_LAG = 6; // フレーム以上のラグはスキップ
		if (isFrameSkipOn || m_status[playerNr].m_cnt + FRAMESKIP_LAG <= frmcnt) {
			m_status[playerNr].m_cnt = frmcnt;
			isFrameSkipOn = true;
		}
	}
	
	//まだ実行しない
	if (!m_status[playerNr].m_isNoReceive && frmcnt > m_status[playerNr].m_cnt) { return; }	

	//初回イベント
	if (m_status[playerNr].m_isNoReceive) {
		m_status[playerNr].m_isNoReceive = false;
		m_status[playerNr].m_cnt = frmcnt;
	}

	NetworkEventCode eventCode = std::get<0>(m_eventContentQueue[playerNr].front());
	ExitGames::Common::Hashtable eventContent = std::get<2>(m_eventContentQueue[playerNr].front());
	m_eventContentQueue[playerNr].pop();

	switch (eventCode){

	case enPlayerStatus:
	{
		//パッド入力
		if (eventContent.getValue((nByte)enActionSender)) {
			m_status[playerNr].m_actionSender = ActionSender(
				{
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 0))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 1))))->getDataCopy() / 100.0f - 1.0f
				},
				((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 2))))->getDataCopy(),
				((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 3))))->getDataCopy(),
				{
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 5))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 6))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 7))))->getDataCopy() / 100.0f - 1.0f
				},
				((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 4))))->getDataCopy(),
				((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 8))))->getDataCopy()
			);
		}

		//座標
		if (eventContent.getValue((nByte)enPosition)) {
			m_status[playerNr].m_isUpdatePos = true;
			m_status[playerNr].m_pos.Set(
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enPosition + 0))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enPosition + 1))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enPosition + 2))))->getDataCopy()
			);
		}
	}
	break;

	case enKillPlayer:
	{
		//殺された
		m_status[GetPhoton()->GetLocalPlayerNumber()].m_isDead = true;
		m_status[GetPhoton()->GetLocalPlayerNumber()].m_isUpdateDead = true;
	}
	break;

	case enPlayerDeadOrAlive:
	{
		//生き死に
		m_status[playerNr].m_isDead = ((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)1)))->getDataCopy();
		m_status[playerNr].m_isUpdateDead = true;
	}
	break;

	default:
		break;
	}

	}
}

void NetPlayerReceiver::PreUpdate() {
	for (int i = 0; i < NET_MAX_PLAYER + 1; i++) {
		//イベント実行
		RunEvent(i);

		//プレイヤーに情報渡す
		UpdatePlayer(i);

		//カウンター進める
		if (!m_status[i].m_isNoReceive) { m_status[i].m_cnt++; }
	}
}

void NetPlayerReceiver::PostLoopUpdate() {
	for (int i = 0; i < NET_MAX_PLAYER + 1; i++) {
		//イベント実行
		RunEvent(i,true);

		//プレイヤーに情報渡す
		UpdatePlayer(i);
	}
}

//プレイヤーに情報渡す
void NetPlayerReceiver::UpdatePlayer(int playerNr) {
	if (m_pCPlayer[playerNr]) {
		
		if (playerNr != GetPhoton()->GetLocalPlayerNumber()) {//自分は除く
			//アクション
			m_pCPlayer[playerNr]->sendAction(m_status[playerNr].m_actionSender);

			//座標
			if (m_status[playerNr].m_isUpdatePos) {
				m_pCPlayer[playerNr]->SetPosition(m_status[playerNr].m_pos);
				m_status[playerNr].m_isUpdatePos = false;
			}
		}

		//生き死に
		if (m_status[playerNr].m_isUpdateDead) {
			if (m_status[playerNr].m_isDead) {
				m_pCPlayer[playerNr]->Death();
			}
			else {
				m_pCPlayer[playerNr]->Revive();
			}
			//再送を防ぐべくキャスタいじる
			if (playerNr != GetPhoton()->GetLocalPlayerNumber()) {
				m_pCaster[playerNr]->SetIsDead(m_status[playerNr].m_isDead);
			}
			m_status[playerNr].m_isUpdateDead = false;
		}

	}
}