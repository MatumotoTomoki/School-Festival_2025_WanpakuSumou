#pragma once
//プレイヤークラス。
class Enemy;
class GameOver;
class Player :public IGameObject
{
public:
	Player();
	~Player();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//移動処理。
	void Move();
	//回転処理。
	void Rotation();
	//ステート管理。
	void ManageState();
	//アニメーションの再生。
	void PlayAnimation();
	//位置を読み取る
	Vector3 GetPosition() const { return m_position; };
	//吹っ飛ばし
	void BlowOff(const Vector3& fromPosition);
	bool Start();
	void Respawn();
	void AddSpeedBoost(float boost, float duration);
	float m_speedMultiplier = 1.0f;     // 通常は 1.0
	float m_boostTimer = 0.0f;          // 残り時間（秒）
	//メンバー変数。
	ModelRender m_modelRender;//モデルレンダー。
	Vector3 m_position;//座標
	enum EnAnimationClip {//アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];//アニメーションクリップ。
	CharacterController m_characterController;//キャラクターコントローラー。
	Vector3 m_moveSpeed;//移動速度。
	Quaternion m_rotation;  //クォータニオン。
	int playerState = 0;//プレイヤーのステート（状態）を表す変数。
	Vector3 m_initialPosition;  // 初期位置

};

