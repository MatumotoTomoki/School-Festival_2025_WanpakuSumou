#pragma once
//エネミークラス
class Player;
class Game;

class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//回転処理。
	void Rotation();
	//移動処理。
	void Move();
	//プレイヤー追従
	void SetPlayer(Player* player) { m_player = player; }
	void BlowBack(const Vector3& fromPosition);
	void Respawn();
	void SetGame(Game* game) { m_game = game; }
	Game* m_game = nullptr;

	//メンバー変数。
	ModelRender m_modelRender;//モデルレンダー。
	Vector3 m_position;//座標
	CharacterController m_characterController;//キャラクターコントローラー。
	Quaternion m_rotation;  //クォータニオン。
	Vector3 m_moveSpeed;//移動速度。
	Player* m_player = nullptr;//プレイヤーに向かって動く
	bool m_isBlowBack = false;      // 吹き飛び中かどうか
	int m_blowBackTimer = 0;        // 吹き飛び時間（フレーム単位）
	Vector3 m_initialPosition; // 初期位置

	
};

