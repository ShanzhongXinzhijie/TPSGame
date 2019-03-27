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
	/// シングルトンのGameを生成する関数
	/// </summary>
	/// <remarks>すでにGameが作られているときは何もせずnullptrを返します。</remarks>
	/// <param name="fade">Fadeオブジェクトのポインタ</param>
	/// <param name="timeLimit">制限時間</param>
	/// <param name="citizenCnt">市民の数</param>
	/// <param name="seed">ランダムのシード値</param>
	/// <param name="startTime_ms">Gameを作成した時間</param>
	/// <returns>作られたGameインスタンスのポインタ,またはnullptr</returns>
	static Game* createGame(Fade* fade, float timeLimit, int citizenCnt, int seed, int startTime_ms) {
		if (static_game == nullptr) {
			static_game = new Game(fade, timeLimit, citizenCnt, seed, startTime_ms);
			static_game->SetName(L"GameClass");
			return static_game;
		}
		return nullptr;
	}

	/// <summary>
	/// Gameオブジェクトを取得
	/// </summary>
	/// <returns>Gameのポインタ</returns>
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
	/// プレイヤーを生成する
	/// </summary>
	/// <param name="isMe">自機ならtrue</param>
	/// <param name="playerNum">プレイヤー番号(重複禁止)</param>
	void createPlayer(bool isMe,int playerNum);

	/// <summary>
	/// プレイヤーを削除します
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	void removePlayer(int playerNum);

	/// <summary>
	/// プレイヤーのポインタを取得します。
	/// </summary>
	/// <param name="playerNum">プレイヤー番号</param>
	/// <returns>プレイヤーのポインタ</returns>
	CPlayer* getPlayer(int playerNum) { 
		return playerGene.getPlayer(playerNum); 
	}

	/// <summary>
	/// プレイヤーのポインタが入ったmapが返されます。
	/// </summary>
	/// <returns>プレイヤー番号をキーとするプレイヤーポインタのマップ</returns>
	const std::unordered_map<int, CPlayer*>& getPlayers() const {
		return playerGene.getPlayers();
	}

	/// <summary>
	/// 自機を取得します
	/// </summary>
	/// <returns></returns>
	const MainPlayer* getMainPlayer() const{
		return playerGene.getMainPlayer();
	}

	/// <summary>
	/// 画面分割時、2Pプレイヤーを取得します
	/// </summary>
	/// <returns>2Pプレイヤー、居ない場合はnullptr</returns>
	const MainPlayer* getMainPlayer2() const {
		return playerGene.getMainPlayer2();
	}

	/// <summary>
	/// 市民ジェネレーターを取得
	/// </summary>
	/// <returns></returns>
	const CitizenGene& GetCitizenGene()const {
		return citizenGene;
	}
	CitizenGene& GetCitizenGene(){
		return citizenGene;
	}

	/// <summary>
	/// 神社ジェネレーターを取得
	/// </summary>
	/// <returns></returns>
	GingerGene& GetGingerGene(){
		return gingerGene;
	}

private:
	static Game* static_game;//シングルトンGameインスタンス

	Ground* ground = nullptr;//舞台
	Level level;//市民などの配置を決めるレベル

	float m_waitGameStartTimer_sec = 0.0f;//ゲーム開始前の待機時間
	float timer = 160.0f;//制限時間
	static constexpr float fadeInDelay = -2.0f;//ゲーム終了後フェードイン前の待機時間
	bool gameIsEnd = false;//ゲームが終了したかどうか

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

