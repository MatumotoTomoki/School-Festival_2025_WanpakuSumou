#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Enemy::Enemy()
{
	m_modelRender.Init("Assets/modelData/Barry.tkm");
	m_modelRender.Update();
	//位置を後ろに下げる
	m_position.z += 500.0f;
	m_position.y -= 40.0f;
	m_initialPosition = m_position;
	//モデルに位置を反映
	m_modelRender.SetPosition(m_position);
	m_characterController.Init(30.0f, 30.0f, m_position);
	//当たり判定を可視化する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/poyonn.wav");
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/tokutenn.wav");
}

Enemy::~Enemy()
{

}

void Enemy::Rotation()
{
	//エネミーを180ど回転させる
	m_rotation.SetRotationDegY(180.0f);

	//回転を絵描きさんに教える。
	m_modelRender.SetRotation(m_rotation);
	
}

void Enemy::Update()
{

	Move();

	m_modelRender.Update();

	if (m_isBlowBack) {
		m_blowBackTimer--;
		if (m_blowBackTimer <= 0) {
			m_isBlowBack = false;
		}
	}

	if (m_position.y < -600.0f) {
		// Gameに通知
		Game::GetInstance()->OnEnemyDefeated();
		Respawn();
	}

	if (m_position.y < -600.0f) {
		if (m_game != nullptr) {
			m_game->OnEnemyDefeated();
			
		}
		Respawn();
	}


}

void Enemy::Move()
{
	if (m_isBlowBack) {
		m_blowBackTimer--;
		if (m_blowBackTimer <= 0) {
			m_isBlowBack = false;
		}

		// 重力処理（地面に接地してるか判定）
		if (m_characterController.IsOnGround()) {
			m_moveSpeed.y = 0.0f;
		}
		else {
			m_moveSpeed.y += -9.8f / 60.0f;
		}

		// 吹き飛び中の移動処理だけ行う（通常追跡はスキップ）
		m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);
		m_modelRender.SetPosition(m_position);
		return;
	}
	if (m_player != nullptr)
	{
		//敵からプレイヤーにの方向ベクトルを計算
		Vector3 toPlayrer = m_player->GetPosition() - m_position;
		//Y軸は無視してXZ平面だけで方向を出す
		toPlayrer.y = 0.0f;
		//正規化(長さ1にする)
		toPlayrer.Normalize();
		//移動スピード
		Vector3 moveDir = toPlayrer * 240.0f;
		//移動方向
		m_moveSpeed.x = moveDir.x;
		m_moveSpeed.z = moveDir.z;
	}
	//地面に付いていたら。
	if (m_characterController.IsOnGround())
	{
		//重力を無くす
		m_moveSpeed.y = 0.0f;
	}
	else
	{
		//重力を発生させる
		m_moveSpeed.y = -9.8f / 60.0f; //フレームごとの重力。
	}
	//キャラコンで移動計算(当たり判定を含む)
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);

	//モデルの回転をプレイヤーに方向に向ける
	if (m_player != nullptr)
	{
		Vector3 dir = m_player->GetPosition() - m_position;
		dir.y = 0.0f;
		if (dir.LengthSq() > 0.01f)
		{
			dir.Normalize();
			//z方向が正面として角度を取得
			float angle = atan2f(dir.x, dir.z);
			m_rotation.SetRotation(Vector3(0.0f, 1.0f, 0.0f), angle);
			m_modelRender.SetRotation(m_rotation);
		}
	}
	// プレイヤーとの距離を測って衝突したら吹っ飛ばす
	if (m_player != nullptr)
	{
		Vector3 toPlayer = m_player->GetPosition() - m_position;
		if (toPlayer.Length() < 50.0f)
		{
			m_player->BlowOff(m_position);
		}
	}

	if (m_player != nullptr)
	{
		Vector3 toPlayer = m_player->GetPosition() - m_position;
		if (toPlayer.Length() < 50.0f)
		{
			// エネミーをプレイヤーの方向と逆に吹き飛ばす
			BlowBack(m_player->GetPosition());
		}
	}
	// 落下判定（Y座標が一定以下ならリスポーン）
	if (m_position.y < -200.0f) {
		Respawn();
		return;
	}


}

void Enemy::BlowBack(const Vector3& fromPosition)
{
	Vector3 blowDir = m_position - fromPosition;
	blowDir.z = 0.0f;

	if (blowDir.LengthSq() > 0.01f)
	{
		blowDir.Normalize();
		m_moveSpeed = blowDir * 500.0f;  // プレイヤーから離れるように移動
		m_moveSpeed.z = 480.0f; // 少し浮かせる

		m_isBlowBack = true;
		m_blowBackTimer = 180; // 180フレームの間 吹っ飛びモード
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(7.5f);
	}
}

void Enemy::Respawn()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(2);  // WaveFileBank番号
	se->Play(false);  // false ＝ 一回だけ再生
	se->SetVolume(4.0f);  // 音量調整
	m_position = m_initialPosition;
	m_moveSpeed = Vector3::Zero;
	m_rotation = Quaternion::Identity;

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_characterController.SetPosition(m_position);

	m_isBlowBack = false;
	m_blowBackTimer = 0;
}







void Enemy::Render(RenderContext& rc)
{
	//モデルを描画する。

	m_modelRender.Draw(rc);
}
