#include "stdafx.h"
#include "Lazer.h"
#include "NormalBullet.h"
#include "CPlayer.h"
#include "Game.h"

Lazer::Lazer(CPlayer* player, GameObj::CSkinModelRender* playerModel,
	unsigned int shotAnim, unsigned int reloadAnim)
	:Weapon(player, playerModel, getInfo(shotAnim, reloadAnim)) {

	m_game = FindGO<Game>(L"GameClass");
	m_sprite.Init(L"Resource/spriteData/lockon.dds");
}

Lazer::~Lazer() {
}

Bullet* Lazer::createBullet(CPlayer * player, CVector3 pos, CVector3 dir) {
	return new NormalBullet(player, pos, dir*5000.0f, L"Resource/modelData/RifleBullet.cmo", 250);
}

WeaponInfo Lazer::getInfo(unsigned int shotAnim, unsigned int reloadAnim) {
	WeaponInfo info;
	info.modelPath = L"Resource/modelData/Lazer.cmo";
	info.spritePath = L"Resource/spriteData/Lazer.dds";
	info.maxBullet = 1;
	info.shotCool = 1.5f;
	info.zoomScale = 5.0f;
	info.shotAnimNum = shotAnim;
	info.reloadAnimNum = reloadAnim;
	return info;
}

namespace{
	//点Pと線(AB)の距離
	float Distance_DotAndLine(const CVector3& P, const CVector3& A, const CVector3& B)
	{
		CVector3 AB, AP;

		AB = B - A;
		AP = P - A;

		//AB、APを外積して求められたベクトルの長さが、平行四辺形Dの面積になる
		AB.Cross(AP); float D = AB.Length();

		//AB間の距離
		float L = (A - B).Length();	//ABの長さ

		return D / L;
	}
}

void Lazer::WeaponUpdate(){

	m_num = -1;
	if (!isActive) { return; }

	bool isFirst = true;	
	float distance = -1.0f;
	const float CAM_FAR = 4000.0f;
	CVector3 rayStart = player->getPosition(); if (!player->isFlying()) { rayStart.y += 60.0f; }

	//プレイヤーロックオン
	for (auto& C : m_game->getPlayers()) {		
		if (player->playerNum == C.second->playerNum) { continue; }
		if (player->GetActionSender().getLookVec().Dot(C.second->getPosition() - player->getPosition()) > 0.0f) {
			float d = Distance_DotAndLine(C.second->getPosition(), rayStart, rayStart + player->GetActionSender().getLookVec()*CAM_FAR);
			if (isFirst || d < distance) {

				CVector3 targetPos = C.second->getPosition(); if (!C.second->isFlying()) { targetPos.y += 60.0f; }
				//障害物判定
				btCollisionWorld::AllHitsRayResultCallback callback(rayStart, targetPos);
				GetPhysicsWorld().RayTest(rayStart, targetPos, callback);
				if (callback.hasHit()){
					int i;
					for (i = 0; i < callback.m_collisionObjects.size(); ++i) {
						const btCollisionObject* col = callback.m_collisionObjects[i];
						if (col->getUserIndex() != enCollisionAttr_Character
						 && col->getInternalType() != btCollisionObject::CO_GHOST_OBJECT
						) {
							break;
						}
					}
					if (i != callback.m_collisionObjects.size()) { continue; }
				}

				isFirst = false;
				distance = d;
				m_isPly = true; m_num = C.second->playerNum; m_lockPos = targetPos;
			}
		}
	}
	//市民ロックオン
	for (int i = 0; i < m_game->GetCitizenGene().GetCitizenNum(); i++) {
		const Citizen* C = m_game->GetCitizenGene().GetCitizen(i);
		if (player->GetActionSender().getLookVec().Dot(C->getPos() - player->getPosition()) > 0.0f) {
			float d = Distance_DotAndLine(C->getPos(), rayStart, rayStart + player->GetActionSender().getLookVec()*CAM_FAR);
			if (isFirst || d < distance) {

				CVector3 targetPos = C->getPos(); targetPos.y += 60.0f;
				//障害物判定
				btCollisionWorld::AllHitsRayResultCallback callback(rayStart, targetPos);
				GetPhysicsWorld().RayTest(rayStart, targetPos, callback);
				if (callback.hasHit()) {
					int i;
					for (i = 0; i < callback.m_collisionObjects.size(); ++i) {
						const btCollisionObject* col = callback.m_collisionObjects[i];
						if (col->getUserIndex() != enCollisionAttr_Character
							&& col->getInternalType() != btCollisionObject::CO_GHOST_OBJECT
							) {
							break;
						}
					}
					if (i != callback.m_collisionObjects.size()) { continue; }
				}

				isFirst = false;
				distance = d;
				m_isPly = false; m_num = i; m_lockPos = targetPos;
			}
		}
	}
	
}

void Lazer::PostRender(){
	if (m_num < 0) { return; }//ノーロック

	CVector3 vec = GetMainCamera()->CalcScreenPosFromWorldPos(m_lockPos);
	if (vec.z > 0.0f) {
		m_sprite.Draw(vec, { 0.75f,0.75f }, { 0.5f,0.5f }, 0.0f, {0.0f,1.0f,0.0f,1.0f});
	}
}
//チャージ
//発射
//スタン
//クールダウンで発射制限