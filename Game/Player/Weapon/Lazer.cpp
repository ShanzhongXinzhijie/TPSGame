#include "stdafx.h"
#include "Lazer.h"
#include "NormalBullet.h"
#include "CPlayer.h"
#include "Game.h"

Lazer::Lazer(CPlayer* player, GameObj::CSkinModelRender* playerModel,
	unsigned int shotAnim, unsigned int reloadAnim)
	:Weapon(player, playerModel, getInfo(shotAnim, reloadAnim)) {

	m_game = FindGO<Game>(L"GameClass");
	m_sprite[0].Init(L"Resource/spriteData/lockon.dds");
	m_sprite[1].Init(L"Resource/spriteData/lockon2.dds");
}

Lazer::~Lazer() {
}

CVector3 Lazer::GetLockPos()const {
	CVector3 vec;
	if (player->GetLockOnIsPly()) {
		const CPlayer* P = m_game->getPlayer(player->GetLockOnNum());
		vec = P->getPosition();
		if (!P->isFlying()) { vec.y += 60.0f; }
	}
	else {
		vec = m_game->GetCitizenGene().GetCitizen(player->GetLockOnNum())->getPos();
		vec.y += 60.0f;
	}
	return vec;
}

Bullet* Lazer::createBullet(CPlayer * player, CVector3 pos, CVector3 dir) {
	if (player->GetLockOnNum() < 0) { return nullptr; }//�m�[���b�N

	CVector3 lockPos = GetLockPos();

	//�G�t�F�N�g
	CVector3 Dir, axis;
	CQuaternion bias, rot, rot2;
	bias.SetRotationDeg(CVector3::AxisX(), 90.0f); 

	Dir = lockPos - pos; axis = Dir; axis.y = 0.0f;
	if (axis.LengthSq() > FLT_EPSILON) {
		axis.Normalize();
		Dir.Normalize();
		float sign = 1.0f;
		if (CVector2(-1.0f, 0.0f).Cross({ 0.0f,Dir.y }) < 0.0f) { sign = -1.0f; }
		rot2.SetRotation(CVector3::AxisX(), sign*CVector3::AngleOf2NormalizeVector(axis, Dir));
	}
	Dir = lockPos - pos; Dir.y = 0.0f;
	if (Dir.LengthSq() > FLT_EPSILON) {
		Dir.Normalize();
		float sign = 1.0f;
		if (CVector2(0.0f, 1.0f).Cross({ Dir.x,Dir.z }) < 0.0f) { sign = -1.0f; }
		rot.SetRotation(CVector3::AxisZ(), sign*CVector3::AngleOf2NormalizeVector(CVector3::AxisZ(), Dir));
	}

	rot.Multiply(bias, rot);
	rot.Multiply(rot, rot2);

	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_lazer.efk", 1.0f, pos, rot, { 50.0f,(lockPos - pos).Length() / 10.0f,50.0f });
	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_end.efk"  , 50.0f, lockPos);
	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_start.efk", 1.0f, pos, CQuaternion::Identity(), {50.0f,450.0f,50.0f});

	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_lazerShot.wav");
	se->SetPos(pos);//���̈ʒu
	se->SetDistance(500.0f);//������������͈�
	se->Play(true);

	//�����ȋʂ��o��
	NormalBullet* B = new NormalBullet(player, lockPos, CVector3::AxisY()*0.1f, nullptr, 150);
	B->SetLifeTime(0.0f);

	//�v���C���[����
	if (player->GetLockOnIsPly()) {
		m_game->getPlayer(player->GetLockOnNum())->SetFlyTimer(0.0f);
	}

	return B;
}

WeaponInfo Lazer::getInfo(unsigned int shotAnim, unsigned int reloadAnim) {
	WeaponInfo info;
	info.modelPath = L"Resource/modelData/Lazer.cmo";
	info.spritePath = L"Resource/spriteData/Lazer.dds";
	info.maxBullet = 1;
	info.shotCool = 1.5f;
	info.shotAnimNum = shotAnim;
	info.reloadAnimNum = reloadAnim;
	return info;
}

namespace{
	//�_P�Ɛ�(AB)�̋���
	float Distance_DotAndLine(const CVector3& P, const CVector3& A, const CVector3& B)
	{
		CVector3 AB, AP;

		AB = B - A;
		AP = P - A;

		//AB�AAP���O�ς��ċ��߂�ꂽ�x�N�g���̒������A���s�l�ӌ`D�̖ʐςɂȂ�
		AB.Cross(AP); float D = AB.Length();

		//AB�Ԃ̋���
		float L = (A - B).Length();	//AB�̒���

		return D / L;
	}
}

void Lazer::WeaponUpdate(){

	if (!isActive) {
		player->SetLockOn(player->GetLockOnIsPly(), -1);
		if (m_eff) { delete m_eff; m_eff = nullptr; }
		if (m_se) { delete m_se; m_se = nullptr; }
		m_effCnt = 0.0f;
		m_charge = 0.0f;
		m_isOnEFF = false;
		return;
	}
		
	//���b�N�p������
	if (player->GetLockOnNum() >= 0 && m_isOnEFF) {
		CVector3 rayStart = player->getPosition(); if (!player->isFlying()) { rayStart.y += 60.0f; }
		CVector3 targetPos, targetPos_dot;
		if (player->GetLockOnIsPly()) {
			const CPlayer* P = m_game->getPlayer(player->GetLockOnNum());
			targetPos = P->getPosition();
			targetPos_dot = targetPos;
			if (!P->isFlying()) { targetPos.y += 60.0f; }
		}
		else {
			targetPos = m_game->GetCitizenGene().GetCitizen(player->GetLockOnNum())->getPos(); 
			targetPos_dot = targetPos;
			targetPos.y += 60.0f;
		}
		//�O�㔻��
		if (player->GetActionSender().getLookVec().Dot(targetPos_dot - player->getPosition()) <= 0.0f) {
			player->SetLockOn(player->GetLockOnIsPly(), -1);
		}
		else {
			//��Q������
			btCollisionWorld::AllHitsRayResultCallback callback(rayStart, targetPos);
			GetPhysicsWorld().RayTest(rayStart, targetPos, callback);
			if (callback.hasHit()) {
				int i;
				for (i = 0; i < callback.m_collisionObjects.size(); ++i) {
					const btCollisionObject* col = callback.m_collisionObjects[i];
					if (col->getUserIndex() != enCollisionAttr_Character
					 && col->getInternalType() != btCollisionObject::CO_GHOST_OBJECT
					) {
						player->SetLockOn(player->GetLockOnIsPly(), -1);
						break;
					}
				}
			}
		}
	}	
	
	//�`���[�W�G�t�F�N�g
	if (m_isOnEFF) {
		m_effCnt -= GetDeltaTimeSec();
		CVector3 pos = player->getPosition();		
		
		//�e������(����)�Əꏊ���w�肵�Ĕ���
		if (player->isFlying()) {
			pos += player->GetActionSender().getLookVec() * (100.0f + 50.0f);
		}
		else {
			pos += player->GetActionSender().getLookVec() * (50.0f + 50.0f);
			pos.y += 60.0f;
		}

		//���ʒu�X�V
		if (m_se) {
			m_se->SetPos(pos);
		}

		if (m_effCnt <= 0.0f) {
			if (m_eff) { delete m_eff; m_eff = nullptr;}
			m_eff = new SuicideObj::CEffekseer(L"Resource/effect/lazerCharge.efk", 10.0f, pos);
			m_effCnt = 1.6f;
		}
		m_eff->SetPos(pos);
		m_eff->SetScale(CVector3::One()*(m_charge+1.0f));
	}
	else {
		//player->SetLockOn(player->GetLockOnIsPly(), -1);
		if (m_eff) { delete m_eff; m_eff = nullptr; }
		if (m_se) { delete m_se; m_se = nullptr; }
		m_effCnt = 0.0f;
		m_charge = 0.0f;
	}
	m_isOnEFF = false;
}

void Lazer::PreShot() {
	if (bulletCount == 0) { return; }

	if (m_charge < FLT_EPSILON) {// player->GetLockOnNum() < 0 && 
		LockOn();
	}
	//if (player->GetLockOnNum() >= 0) {
		m_isOnEFF = true;
		m_charge += GetDeltaTimeSec();
		if (m_charge < 1.0f) {
			shotCool = 1.5f;
		}
		else {
			shotCool = 0.0f;
		}
		//SE
		if (!m_se) {
			m_se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_lazerCharge.wav");
			m_se->SetPos(player->getPosition());//���̈ʒu
			m_se->SetDistance(500.0f);//������������͈�
			m_se->SetIsAutoDelete(false);//�����ō폜���Ȃ�
			m_se->Play(true);
		}
	//}
}

void Lazer::LockOn() {
#ifndef SpritScreen
	if (player->playerNum != GetPhoton()->GetLocalPlayerNumber()) {
		return;
	}
#endif

	bool isFirst = true;
	float distance = -1.0f;
	const float CAM_FAR = 4000.0f;
	CVector3 rayStart = player->getPosition(); if (!player->isFlying()) { rayStart.y += 60.0f; }

	//�v���C���[���b�N�I��
	for (auto& C : m_game->getPlayers()) {
		if (player->playerNum == C.second->playerNum || C.second->GetIsDead()) { continue; }

		CVector3 dist = C.second->getPosition() - player->getPosition();
		if (player->GetActionSender().getLookVec().Dot(dist) > 0.0f && dist.LengthSq() > 230.0f*230.0f) {
			
			float d = Distance_DotAndLine(C.second->getPosition(), rayStart, rayStart + player->GetActionSender().getLookVec()*CAM_FAR);
			if (isFirst || d < distance) {

				CVector3 targetPos = C.second->getPosition(); if (!C.second->isFlying()) { targetPos.y += 60.0f; }
				//��Q������
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
				player->SetLockOn(true, C.second->playerNum);
			}
		}
	}
	//�s�����b�N�I��
	for (int i = 0; i < m_game->GetCitizenGene().GetCitizenNum(); i++) {
		const Citizen* C = m_game->GetCitizenGene().GetCitizen(i);

		if (C->GetIsDead()) { continue; }

		CVector3 dist = C->getPos() - player->getPosition();
		if (player->GetActionSender().getLookVec().Dot(dist) > 0.0f && dist.LengthSq() > 230.0f*230.0f) {
			
			float d = Distance_DotAndLine(C->getPos(), rayStart, rayStart + player->GetActionSender().getLookVec()*CAM_FAR);
			if (isFirst || d < distance) {

				CVector3 targetPos = C->getPos(); targetPos.y += 60.0f;
				//��Q������
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
				player->SetLockOn(false, i);
			}
		}
	}
}

void Lazer::PostRender(){

	if (player->GetLockOnNum() < 0 || m_charge < FLT_EPSILON) { return; }//�m�[���b�N

	CVector3 vec = GetMainCamera()->CalcScreenPosFromWorldPos(GetLockPos());
	if (vec.z > 0.0f && vec.z < 1.0f) {
#ifndef SpritScreen
		if (player->playerNum != GetPhoton()->GetLocalPlayerNumber()) {
			//�������_���Ă���
			if (player->GetLockOnIsPly() && player->GetLockOnNum() == GetPhoton()->GetLocalPlayerNumber()) {
				m_sprite[1].Draw(vec, { 1.0f,1.0f }, { 0.5f,0.5f }, CMath::RandomZeroToOne()*CMath::PI2, { 1.0f,0.0f,0.0f,1.0f });
			}
			return;
		}
#endif
		m_sprite[0].Draw(vec, { 0.75f,0.75f }, { 0.5f,0.5f }, 0.0f, {0.0f,1.0f,0.0f,1.0f});
	}
}