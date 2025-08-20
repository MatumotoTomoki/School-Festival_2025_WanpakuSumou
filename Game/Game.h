#pragma once
#include "sound/SoundSource.h"
class Player;
class GameCamera;
class Stage;
class Enemy;
class Item;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	void Update();
	void Render(RenderContext& rc);
	void OnEnemyDefeated();
	static Game* GetInstance();
	static Game* s_instance;
	//メンバー変数。
	Player* player = nullptr;			//プレイヤー。
	GameCamera* gameCamera = nullptr;			//ゲームカメラ。
	Stage* stage; //ステージ
	Enemy* enemy; //エネミー
	Item* item;
	FontRender fontRender; //文字
	int m_defeatCount = 0;
	float m_elapsedTime = 0.0f; // 経過時間（秒）
	FontRender timeFontRender; // 時間表示用のフォント
	SoundSource* gamebgm;
	bool m_isGameClear = false;
	float m_itemRespawnTimer = 0.0f;
private:
	
	Vector3 m_pos;
	
};

