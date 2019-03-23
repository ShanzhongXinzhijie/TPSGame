#include "stdafx.h"
#include "NetPlayerReceiver.h"
#include "Citizen.h"
#include "Weapon.h"
#include "../Ginger/Hades.h"

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

	case enNormal:
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
				false,//(buttons & 0b100000) != 0,
				false//(buttons & 0b1000000) != 0
			);
			//飛行中のフラグ
			m_status[playerNr].m_isUpd8Fly = true;
			m_status[playerNr].m_isFly = (buttons & 0b10000) != 0;			
		}

		//ロックオン
		if (eventContent.getValue((nByte)enLockOn)) {
			m_status[playerNr].m_isUpd8Lock = true;
			m_status[playerNr].m_lock = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enLockOn)))->getDataCopy();
		}

		//装備武器
		if (eventContent.getValue((nByte)enActiveWepon)) {
			m_status[playerNr].m_isUpd8ActiveWeapon = true;
			m_status[playerNr].m_activeWeapon = ((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)enActiveWepon)))->getDataCopy();
		}

		//フライ情報
		if (eventContent.getValue((nByte)enFlyTimer)) {
			m_status[playerNr].m_isUpd8FlyTimer = true;
			m_status[playerNr].m_flyTimer = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enFlyTimer)))->getDataCopy();
		}

		//弾数
		if (eventContent.getValue((nByte)enBulletCnt)) {
			m_status[playerNr].m_isUpd8BulletCnt = true;
			for (int i = 0; i < CPlayer::WEAPON_NUM; i++)
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

		//市民位置
		int offset = 0;
		if (eventContent.getValue((nByte)enZombiePosAvg)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enZombiePosAvg)))->getDataCopy();
			for (int i = 0; i < num; i++) {
				//id
				int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//座標
				CVector3 pos;
				pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				m_citizenPosListAvg.emplace_back(std::make_pair(id,pos));
			}
		}
		if (eventContent.getValue((nByte)enZombiePosSync)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enZombiePosSync)))->getDataCopy();
			for (int i = 0; i < num; i++) {
				//id
				int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//時間
				int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//座標
				CVector3 pos;
				pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				m_citizenPosListSync.emplace_back(std::make_tuple(id, pos, playerNr, time));
			}
		}
		//市民Mover
		if (eventContent.getValue((nByte)enZombieMover)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enZombieMover)))->getDataCopy();
			for (int i = 0; i < num; i++) {
				//id
				int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//時間
				int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//座標
				CVector3 pos;
				pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//MoverVec
				CVector3 MoverVec;
				MoverVec.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				MoverVec.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				MoverVec.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				m_citizenMoverSyncList.emplace_back(std::make_tuple(id, pos, playerNr, time, MoverVec));
			}
		}
	}
	break;

	case enReliable:
	{
		//眷属化
		if (eventContent.getValue((nByte)enKenzoku)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enKenzoku)))->getDataCopy();
			int i2 = enKenzoku + 1;
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

		//神の力獲得
		if (eventContent.getValue((nByte)enGetGodpower)) {
			int gingerN = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enGetGodpower+0))))->getDataCopy();
			int plyN = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enGetGodpower+1))))->getDataCopy();
			GetGodPower(gingerN, plyN);
		}
		//神の力使用
		if (eventContent.getValue((nByte)enUseGodPower)) {
			GodPowerType type = (GodPowerType)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)enUseGodPower)))->getDataCopy();
			UseGodPower(type, playerNr);
		}

		//ヲシリスの召喚
		if (eventContent.getValue((nByte)enSummonWosiris)) {
			//召喚
			if (m_gingerGene) {
				int rot = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 0))))->getDataCopy();
				new Wosiris(m_pCPlayer[playerNr], CMath::DegToRad((float)rot), m_gingerGene);
			}
			//生贄
			if (m_citizenGene) {
				int citizen[3];
				citizen[0] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 1))))->getDataCopy();
				citizen[1] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 2))))->getDataCopy();
				citizen[2] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 3))))->getDataCopy();
				for (int i = 0; i < 3; i++) {
					if (citizen[i] >= 0) {
						m_citizenGene->GetCitizen(citizen[i])->Death();
					}
				}
			}
		}
		//ヲシリスのコントロール奪取
		if (eventContent.getValue((nByte)enGetControlWosiris)) {
			if (m_gingerGene && m_gingerGene->GetWosiris()) {
				Wosiris* pWosiris = m_gingerGene->GetWosiris();
				int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enGetControlWosiris)))->getDataCopy();
				//時間が新しい or 時間が同じでプレイヤー番号が大きい
				if (pWosiris->GetLastControlTime() < time || pWosiris->GetLastControlTime() == time && playerNr > pWosiris->GetLastControlPly()) {
					//更新
					if (m_pCPlayer[playerNr]) {
						pWosiris->SetLastControl(playerNr, time);
						pWosiris->ChangeControl(m_pCPlayer[playerNr]);
					}
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

	case enDestroyGinger:
	{
		//神社破壊
		int offset = 1;
		int num = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)offset)))->getDataCopy(); offset++;
		for (int i = 0; i < num; i++) {
			int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)offset)))->getDataCopy(); offset++;
			int id = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)offset)))->getDataCopy(); offset++;
			AddGodPowerLottery(id, time, playerNr);
		}
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

		UpdateGodPower();

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

		UpdateGodPower();
	}
}

//プレイヤーに情報渡す
void NetPlayerReceiver::UpdatePlayer(int playerNr) {
	if (m_pCPlayer[playerNr] && m_pCPlayer[playerNr]->GetIsInit()) {
		
		if (playerNr != GetPhoton()->GetLocalPlayerNumber()) {//自分は除く
			//アクション
			m_pCPlayer[playerNr]->sendAction(m_status[playerNr].m_actionSender);
			
			//ロックオン
			if (m_status[playerNr].m_isUpd8Lock) {
				bool isPly = m_pCPlayer[playerNr]->GetLockOnIsPly();
				int lock = m_status[playerNr].m_lock;
				
				if (lock == 0) {
					//ノーロック
					lock = -1;
				}
				else {
					if (lock < 0) { 
						//プレイヤー
						isPly = true;
						lock *= -1;
					}
					else {
						//市民
						isPly = false;
					}
					lock--;
				}

				m_pCPlayer[playerNr]->SetLockOn(isPly, lock);
				m_status[playerNr].m_isUpd8Lock = false;
			}	

			//装備武器
			if (m_status[playerNr].m_isUpd8ActiveWeapon) {
				m_pCPlayer[playerNr]->changeWeapon(m_status[playerNr].m_activeWeapon);
				m_status[playerNr].m_isUpd8ActiveWeapon = false;
			}
			//フライ情報
			if (m_status[playerNr].m_isUpd8FlyTimer) {
				m_pCPlayer[playerNr]->SetFlyTimer(m_status[playerNr].m_flyTimer);
				m_status[playerNr].m_isUpd8FlyTimer = false;
			}
			//飛行フラグ
			if (m_status[playerNr].m_isUpd8Fly) {
				if (m_status[playerNr].m_isFly) {
					if (!m_pCPlayer[playerNr]->isFlying()) { m_pCPlayer[playerNr]->fly(); }
				}
				else {
					if (m_pCPlayer[playerNr]->isFlying()) { m_pCPlayer[playerNr]->flyStop(); }
				}
				m_status[playerNr].m_isUpd8Fly = false;
			}
			//弾数
			if (m_status[playerNr].m_isUpd8BulletCnt) {
				Weapon** w = m_pCPlayer[playerNr]->GetWeapons();
				for (int i = 0; i < CPlayer::WEAPON_NUM; i++)
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
		//眷属化
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

		//位置平均
		for (auto& cs : m_citizenPosListAvg) {
			auto citizen = m_citizenGene->GetCitizen(cs.first);
			if (!citizen) { continue; }
			//更新
			//citizen->SetIsSend(false);
			citizen->setPos((citizen->getPos() + cs.second)*0.5f);
		}
		m_citizenPosListAvg.clear();

		//位置同期
		for (auto& cs : m_citizenPosListSync) {
			auto citizen = m_citizenGene->GetCitizen(std::get<0>(cs));
			if (!citizen) { continue; }
			//時間が新しい or 時間が同じでプレイヤー番号が大きいか等しい
			if (citizen->GetTargetCnt() < std::get<3>(cs) || citizen->GetTargetCnt() == std::get<3>(cs) && std::get<2>(cs) >= citizen->GetTargetPly()) {
				//更新
				citizen->SetIsSend(false);
				citizen->setPos(std::get<1>(cs));
				citizen->SetTargetPly(std::get<2>(cs));
				citizen->SetTargetCnt(std::get<3>(cs));
			}
		}
		m_citizenPosListSync.clear();

		//Mover同期
		for (auto& cs : m_citizenMoverSyncList) {
			auto citizen = m_citizenGene->GetCitizen(std::get<0>(cs));
			if (!citizen) { continue; }
			//時間が新しい or 時間が同じでプレイヤー番号が大きいか等しい
			if (citizen->GetSyncMoverCnt() < std::get<3>(cs) || citizen->GetSyncMoverCnt() == std::get<3>(cs) && std::get<2>(cs) >= citizen->GetSyncMoverPly()) {
				//更新
				citizen->SetIsSendMover(false);
				citizen->setPos(std::get<1>(cs));
				citizen->SetSyncMoverPly(std::get<2>(cs));
				citizen->SetSyncMoverCnt(std::get<3>(cs));
				citizen->SetMoverNetVec(std::get<4>(cs));
			}
		}
		m_citizenMoverSyncList.clear();		
	}
}

void NetPlayerReceiver::UpdateGodPower() {
	//神の力あげる
	for (auto& T : m_godPowerLotteryTimer) {
		if (T.second > 0) {
			T.second--;
			if (T.second <= 0) {
				//獲得
				GetGodPower(T.first, m_godPowerLotteryList[T.first].second);

				if (m_pCaster[GetPhoton()->GetLocalPlayerNumber()]) {
					//みんなに伝える
					m_pCaster[GetPhoton()->GetLocalPlayerNumber()]->SendGetGodPower(T.first, m_godPowerLotteryList[T.first].second);
				}
				m_godPowerLotteryTimer.erase(T.first);
			}
		}
	}
}
void NetPlayerReceiver::AddGodPowerLottery(int id, int time, int plyNum) {

	if (!GetPhoton()->GetIsMasterClient()) { return; }

	if (m_godPowerLotteryList.count(id) == 0) {
		//新規
		m_godPowerLotteryList.emplace(id, std::make_pair(time, plyNum));
		
		if (m_godPowerLotteryTimer.count(id) == 0) {
			//タイマー開始
			m_godPowerLotteryTimer.emplace(id, 30);
		}
	}
	else {
		//時間が古いと優先
		if (m_godPowerLotteryList[id].first > time) {
			m_godPowerLotteryList.insert_or_assign(id, std::make_pair(time, plyNum));
		}
	}
}

void NetPlayerReceiver::GetGodPower(int jinjyaNum,int plyNum) {
	if (!m_gingerGene) { return; }
	
	//獲得
	if (m_pCPlayer[plyNum]) {
		m_pCPlayer[plyNum]->SetGodPower(m_gingerGene->GetGinger(jinjyaNum)->GetPowerType());
	}
	//神社破壊
	m_gingerGene->GetGinger(jinjyaNum)->Destory();
}
void NetPlayerReceiver::UseGodPower(GodPowerType type, int plyNum) {
	if (!m_gingerGene || !m_pCPlayer[plyNum]) { return; }
	
	switch (type) {
	case enHades:
		new Hades(m_pCPlayer[plyNum], m_gingerGene);
		break;
	default:
		break;
	}
}