#include "stdafx.h"
#include "GodPowerStarter.h"
#include "Game.h"
#include "Wosiris.h"
#include "Hades.h"

GodPowerStarter::GodPowerStarter()
{
}

GodPowerStarter::~GodPowerStarter()
{
}

void GodPowerStarter::Init(MainPlayer* player) {
	m_pPlayer = player;
}

bool GodPowerStarter::Start(){
	m_pGame = FindGO<Game>(L"GameClass"); if (!m_pGame) { return false; }
	m_pGinGene = &m_pGame->GetGingerGene();

	m_ring.Init(L"Resource\\modelData\\ring.cmo", enFbxUpAxisY);
	m_ring.SetIsShadowCaster(false);
	m_ring.SetEnable(false);
	m_ring.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
		mat->SetAlbedoScale({1.0f,0.0f,1.0f,1.0f});		
		mat->SetLightingEnable(false);
		mat->SetIsMotionBlur(false);
	});

	m_fontDefo.UseSystemFont();	
	m_bButton.Init(L"Resource/spriteData/BButton.dds");

	return true;
}

void GodPowerStarter::SetPowertype(GodPowerType type) {
	m_type = type;

	switch (m_type) {
	case enWosiris:
		wcscpy_s(m_text, L"GIANT DRAGON\n�@�|THE GOD OF WOSIRIS\nATTACK    DEFENCE\n X000         X000");
		break;
	case enHades:
		wcscpy_s(m_text, L"�n�f�X�̂��Ԃ�\n��������������B");
		break;
	case enNone:
	default:
		break;
	}
}

void GodPowerStarter::Update(){

	if (!m_pPlayer) { return; }//��������

	//�����O�\���̎���
	if(m_ringTime > 0){
		m_ringTime--;
		if (m_ringTime <= 0) { m_ring.SetEnable(false); }
	}
	if (m_mesageTime > 0) {
		m_mesageTime--;
	}

	if (Pad(m_pPlayer->GetUsePadNum()).GetDown(enButtonB)) {
		IGameObject* pGO = nullptr;

		switch (m_type) {
		case enHades:
		{
			//�g�p
			pGO = new Hades(m_pPlayer, m_pGinGene);

#ifndef SpritScreen
			//�g�p��ʐM
			m_pPlayer->GetNetCaster()->SendUseGodPower(enHades);
#endif
		}
		break;
		case enWosiris:
		{
			//���є͈͕\��
			const float sacrificeRange = 300.0f;
			m_ring.SetEnable(true);
			m_ring.SetScale({ sacrificeRange, 20.0f, sacrificeRange });
			m_ringTime = 15;

			//����
			int cnt = 0;
			Citizen* C_sacrifice[3] = {};
			CPlayer* P_sacrifice[3] = {};
			for (int i = 0; i < m_pGame->GetCitizenGene().GetCitizenNum(); i++) {
				Citizen* C = m_pGame->GetCitizenGene().GetCitizen(i);
				if (m_pPlayer->team == C->GetOwnerTeam() && !C->GetIsDead() && (C->getPos() - m_pPlayer->getPosition()).LengthSq() < sacrificeRange*sacrificeRange) {
					C_sacrifice[cnt] = C;
					cnt++;
					if (cnt == 3) { break; }
				}
			}
			if (cnt < 3) {
				for (auto& P : m_pGame->getPlayers()) {
					if (m_pPlayer != P.second && m_pPlayer->team == P.second->team && !P.second->GetIsDead() && (P.second->getPosition() - m_pPlayer->getPosition()).LengthSq() < sacrificeRange*sacrificeRange) {
						P_sacrifice[cnt] = P.second;
						cnt++;
						if (cnt == 3) { break; }
					}
				}
			}
			if (cnt == 3) {
				//���т������
				for (auto& S : C_sacrifice) { if (S) { S->Death(); } }
				for (auto& S : P_sacrifice) { if (S) { S->Death(); } }

				//�p�x���߂�
				int rot = 0;
				CVector3 Dir;
				Dir = m_pPlayer->getPosition(); Dir.y = 0.0f;
				if (Dir.LengthSq() > FLT_EPSILON) {
					Dir.Normalize();
					float sign = 1.0f;
					if (CVector2(0.0f, 1.0f).Cross({ Dir.x,Dir.z }) < 0.0f) { sign = -1.0f; }
					rot = (int)CMath::RadToDeg(sign * -CVector3::AngleOf2NormalizeVector(CVector3::AxisZ(), Dir));
				}
				
				//����
				pGO = new Wosiris(m_pPlayer, CMath::DegToRad((float)rot), m_pGinGene);
				
#ifndef SpritScreen
				//������ʐM
				int citi[3];
				for (int i = 0; i < 3;i++) {
					if (!C_sacrifice[i]) {
						citi[i] = -1;
					}
					else {
						citi[i] = C_sacrifice[i]->GetUniqueID();
					}
				}
				m_pPlayer->GetNetCaster()->SendSummonWosiris(rot, citi[0], citi[1], citi[2]);
#endif

			}else{
				m_mesageTime = 60;
			}
		}
			break;
		case enNone:
		default:
			break;
		}

		if (pGO) {
			//m_pGinGene->Register(pGO);
			m_type = enNone;
		}
	}

	m_ring.SetPos(m_pPlayer->getPosition());
}

void GodPowerStarter::PostRender(){	

	if (m_type == enNone) { return; }

	m_bButton.Draw({ 0.88f,0.65f}, { 0.6f, 0.6f }, { 0.5f,0.5f });
	m_font.Draw(m_text, { 0.88f + 0.003f*0.5f,0.65f + 0.05f + 0.003f*0.5f }, { 0.0f,0.0f,0.0f,1.0f }, CVector2::One()*0.5f, { 0.5f,0.0f });
	m_font.Draw(m_text, { 0.88f,0.65f + 0.05f }, { 1.0f,1.0f,1.0f,1.0f }, CVector2::One()*0.5f, { 0.5f,0.0f });
	
	if(m_mesageTime > 0){
		m_fontDefo.Draw(L"3�̂̐��т��K�v", { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, CVector2::One(), { 0.5f,0.5f },0.0f,DirectX::SpriteEffects_None, 0.0f);
	}
}