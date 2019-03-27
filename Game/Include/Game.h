#pragma once
#include "MainPlayer.h"
#include "Ground.h"
#include "DemolisherWeapon/level/Level.h"
#include <unordered_set>
#include <unordered_map>
#include "Citizen.h"
#include "SqSpawner.h"
#include "Team.h"
#include "PlayerGene.h"
#include "CitizenGene.h"
#include "../Ginger/GingerGene.h"
#include "DemolisherWeapon/Graphic/Light/Lights.h"

#include "../Network/NetPlayerManager.h"
#include "../Network/NetGameEventCaster.h"
#include "../Network/NetWork.h"

class Fade;

class Game : public IGameObject{
public:
	/// <summary>
	/// �V���O���g����Game�𐶐�����֐�
	/// </summary>
	/// <remarks>���ł�Game������Ă���Ƃ��͉�������nullptr��Ԃ��܂��B</remarks>
	/// <param name="fade">Fade�I�u�W�F�N�g�̃|�C���^</param>
	/// <param name="timeLimit">��������</param>
	/// <param name="citizenCnt">�s���̐�</param>
	/// <param name="seed">�����_���̃V�[�h�l</param>
	/// <param name="startTime_ms">Game���쐬��������</param>
	/// <returns>���ꂽGame�C���X�^���X�̃|�C���^,�܂���nullptr</returns>
	static Game* createGame(Fade* fade, float timeLimit, int citizenCnt, int seed, int startTime_ms) {
		if (static_game == nullptr) {
			static_game = new Game(fade, timeLimit, citizenCnt, seed, startTime_ms);
			static_game->SetName(L"GameClass");
			return static_game;
		}
		return nullptr;
	}

	/// <summary>
	/// Game�I�u�W�F�N�g���擾
	/// </summary>
	/// <returns>Game�̃|�C���^</returns>
	static const Game* getGame() {
		return static_game;
	}

private:
	Game(Fade* fade, float timeLimit, int citizenCnt, int seed, int startTime_ms);
public:
	~Game();

	void Update() override;
	void PostRender() override;

	/// <summary>
	/// �v���C���[�𐶐�����
	/// </summary>
	/// <param name="isMe">���@�Ȃ�true</param>
	/// <param name="playerNum">�v���C���[�ԍ�(�d���֎~)</param>
	void createPlayer(bool isMe,int playerNum);

	/// <summary>
	/// �v���C���[���폜���܂�
	/// </summary>
	/// <param name="playerNum">�v���C���[�ԍ�</param>
	void removePlayer(int playerNum);

	/// <summary>
	/// �v���C���[�̃|�C���^���擾���܂��B
	/// </summary>
	/// <param name="playerNum">�v���C���[�ԍ�</param>
	/// <returns>�v���C���[�̃|�C���^</returns>
	CPlayer* getPlayer(int playerNum) { 
		return playerGene.getPlayer(playerNum); 
	}

	/// <summary>
	/// �v���C���[�̃|�C���^��������map���Ԃ���܂��B
	/// </summary>
	/// <returns>�v���C���[�ԍ����L�[�Ƃ���v���C���[�|�C���^�̃}�b�v</returns>
	const std::unordered_map<int, CPlayer*>& getPlayers() const {
		return playerGene.getPlayers();
	}

	/// <summary>
	/// ���@���擾���܂�
	/// </summary>
	/// <returns></returns>
	const MainPlayer* getMainPlayer() const{
		return playerGene.getMainPlayer();
	}

	/// <summary>
	/// ��ʕ������A2P�v���C���[���擾���܂�
	/// </summary>
	/// <returns>2P�v���C���[�A���Ȃ��ꍇ��nullptr</returns>
	const MainPlayer* getMainPlayer2() const {
		return playerGene.getMainPlayer2();
	}

	/// <summary>
	/// �s���W�F�l���[�^�[���擾
	/// </summary>
	/// <returns></returns>
	const CitizenGene& GetCitizenGene()const {
		return citizenGene;
	}
	CitizenGene& GetCitizenGene(){
		return citizenGene;
	}

	/// <summary>
	/// �_�ЃW�F�l���[�^�[���擾
	/// </summary>
	/// <returns></returns>
	GingerGene& GetGingerGene(){
		return gingerGene;
	}

private:
	static Game* static_game;//�V���O���g��Game�C���X�^���X

	Ground* ground = nullptr;//����
	Level level;//�s���Ȃǂ̔z�u�����߂郌�x��

	float m_waitGameStartTimer_sec = 0.0f;//�Q�[���J�n�O�̑ҋ@����
	float timer = 160.0f;//��������
	static constexpr float fadeInDelay = -2.0f;//�Q�[���I����t�F�[�h�C���O�̑ҋ@����
	bool gameIsEnd = false;//�Q�[�����I���������ǂ���

	PlayerGene playerGene;
	CitizenGene citizenGene;
	GingerGene gingerGene;

#ifdef SpritScreen
	GameObj::PerspectiveCamera karicamera;
#else
	NetPlayerManager m_netPlayerManager;
	NetGameEventCaster m_netEventCaster;
	NetWorkManager* m_netWork = nullptr;
#endif

	SuicideObj::CBGM* bgm;
	CFont font;
	Fade* fade;
};

