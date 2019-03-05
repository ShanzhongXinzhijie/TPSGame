#include "stdafx.h"
#include "NetPlayerReceiver.h"
#include "Citizen.h"
#include "Weapon.h"

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
			//ボタン入力のビットフィールドを取得
			nByte buttons = ((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 5))))->getDataCopy();
			//アクションセンダーをセット
			m_status[playerNr].m_actionSender = ActionSender(
				{
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 0))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 1))))->getDataCopy() / 100.0f - 1.0f
				},
				(buttons & 0b1) != 0,
				(buttons & 0b10) != 0,
				{
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 2))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 3))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 4))))->getDataCopy() / 100.0f - 1.0f
				},
				(buttons & 0b100) != 0,
				(buttons & 0b1000) != 0,
				(buttons & 0b10000) != 0,
				(buttons & 0b100000) != 0
			);
			//飛行中のフラグ
			m_status[playerNr].m_isFly = (buttons & 0b10000) != 0;
		}

		//フライ情報
		if (eventContent.getValue((nByte)enFlyTimer)) {
			m_status[playerNr].m_isUpd8FlyTimer = true;
			m_status[playerNr].m_flyTimer = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enFlyTimer)))->getDataCopy();
		}

		//弾数
		if (eventContent.getValue((nByte)enBulletCnt)) {
			m_status[playerNr].m_isUpd8BulletCnt = true;
			for (int i = 0; i < CPlayer::weaponNum; i++)
			{
				m_status[playerNr].m_bulletCnt[i] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enBulletCnt + i))))->getDataCopy();
			}
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
		//ベロシティ
		if (eventContent.getValue((nByte)enVelocity)) {
			m_status[playerNr].m_isUpd8Velocity = true;
			m_status[playerNr].m_velocity.Set(
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enVelocity + 0))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enVelocity + 1))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enVelocity + 2))))->getDataCopy()
			);
		}
	}
	break;

	case enKenzoku:
	{
		//眷属化
		int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)1)))->getDataCopy();
		int i2 = 2;
		for (int i = 0; i < num; i++) {
			int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;//ID
			int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;//時間
			//座標
			CVector3 pos;
			pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;
			pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;
			pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;
			
			auto C = m_citizensStatus.find(id);
			if (C == m_citizensStatus.end()) {
				//新規作成
				m_citizensStatus.emplace(id, CitizensStatus() = { time, playerNr, pos });
			}
			else {
				//時間が新しい or 時間が同じでプレイヤー番号が大きい
				if (C->second.timeCnt < time || C->second.timeCnt == time && C->second.plyNum < playerNr) {
					//上書き
					C->second = { time, playerNr, pos };
				}
			}
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
		//市民に情報渡す
		UpdateCitizen();

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
		//市民に情報渡す
		UpdateCitizen();
	}
}

//プレイヤーに情報渡す
void NetPlayerReceiver::UpdatePlayer(int playerNr) {
	if (m_pCPlayer[playerNr]) {
		
		if (playerNr != GetPhoton()->GetLocalPlayerNumber()) {//自分は除く
			//アクション
			m_pCPlayer[playerNr]->sendAction(m_status[playerNr].m_actionSender);
			//フライ情報
			if (m_status[playerNr].m_isUpd8FlyTimer) {
				m_pCPlayer[playerNr]->SetFlyTimer(m_status[playerNr].m_flyTimer);
				m_status[playerNr].m_isUpd8FlyTimer = false;
			}
			//飛行フラグ
			if (m_status[playerNr].m_isFly) {
				if (!m_pCPlayer[playerNr]->isFlying()) { m_pCPlayer[playerNr]->fly(); }
			}
			else {
				if ( m_pCPlayer[playerNr]->isFlying()) { m_pCPlayer[playerNr]->flyStop(); }
			}
			//弾数
			if (m_status[playerNr].m_isUpd8BulletCnt) {
				Weapon** w = m_pCPlayer[playerNr]->GetWeapons();
				for (int i = 0; i < CPlayer::weaponNum; i++)
				{
					w[i]->setBulletCount(m_status[playerNr].m_bulletCnt[i]);
				}
				m_status[playerNr].m_isUpd8BulletCnt = false;
			}
			//座標
			if (m_status[playerNr].m_isUpdatePos) {
				m_pCPlayer[playerNr]->SetPosition(m_status[playerNr].m_pos);
				m_status[playerNr].m_isUpdatePos = false;
			}
			//ベロシティ
			if (m_status[playerNr].m_isUpd8Velocity) {
				m_pCPlayer[playerNr]->SetVelocity(m_status[playerNr].m_velocity);
				m_status[playerNr].m_isUpd8Velocity = false;
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

//市民に情報渡す
void NetPlayerReceiver::UpdateCitizen() {
	if (m_citizenGene) {
		for (auto& cs : m_citizensStatus) {
			auto citizen = m_citizenGene->GetCitizen(cs.first);
			if (!citizen) { continue; }
			//時間が新しい or 時間が同じでプレイヤー番号が大きい
			if (citizen->GetLastKenzokuingCnt() < cs.second.timeCnt || citizen->GetLastKenzokuingCnt() == cs.second.timeCnt && cs.second.plyNum > citizen->GetLastKenzokuingPly()) {
				//更新(市民の情報二種・座標)・眷属化
				if (m_pCPlayer[cs.second.plyNum]) {
					citizen->SetLastKenzokuingCnt(cs.second.timeCnt);
					citizen->SetLastKenzokuingPly(cs.second.plyNum);
					citizen->setPos(cs.second.pos);
					citizen->ChangeToKenzoku(m_pCPlayer[cs.second.plyNum]);
				}
			}
		}
		m_citizensStatus.clear();
	}
}