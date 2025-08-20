#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	//アニメーショングリップをロードする。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	//ユニティちゃんのモデルを読み込む。
	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_characterController.Init(25.0f, 75.0f,m_position);
}

bool Player::Start()
{
	m_initialPosition = m_position;  // 初期位置として現在位置を保存

	// その他初期化コード
	return true;
}

Player::~Player()
{

}

//更新処理。
void Player::Update()
{
	// 落下チェック
	if (m_position.y < -200.0f) {
		Respawn();
	}
	//移動処理。
	Move();

	//回転処理。
	Rotation();

	//ステート管理。
	ManageState();

	//アニメーションの再生。
	PlayAnimation();

	//絵描きさんの更新処理。
	m_modelRender.Update();

	if (m_boostTimer > 0.0f)
	{
		m_boostTimer -= g_gameTime->GetFrameDeltaTime();
	}
	if (m_boostTimer <= 0.0f)
	{
		m_speedMultiplier = 1.0f;
	}
}

void Player::Move()
{
	

	
	
		
		//xzの移動速度を0.0fにする。
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;

		//左スティックの入力量を取得。
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();

		//カメラの前方向と右方向のベクトルを持ってくる。
		Vector3 forward = g_camera3D->GetForward();
		Vector3 right = g_camera3D->GetRight();
		//y方向には移動させない。
		forward.y = 0.0f;
		right.y = 0.0f;

		//左スティックの入力量と120.0fを乗算。
		right *= stickL.x * 360.0f * m_speedMultiplier;
		forward *= stickL.y * 360.0f * m_speedMultiplier;

		//移動速度にスティックの入力量を加算する。
		m_moveSpeed += right + forward;
	


	//地面に付いていたら。
	if (m_characterController.IsOnGround())
	{
		//重力を無くす。
		m_moveSpeed.y = 0.0f;
		//Aボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//ジャンプさせる。
			m_moveSpeed.y = 240.0f;
		}
	}
	//地面に付いていなかったら。
	else
	{
		//重力を発生させる。
		m_moveSpeed.y -= 5.5f;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rotation);
	}
}

//ステート管理。
void Player::ManageState()
{
	//地面に付いていなかったら。
	if (m_characterController.IsOnGround() == false)
	{
		//ステートを1(ジャンプ中)にする。
		playerState = 1;
		//ここでManageStateの処理を終わらせる。
		return;
	}

	//地面に付いていたら。
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//ステートを2(歩き)にする。
		playerState = 2;
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを0(待機)にする。
		playerState = 0;
	}
}

//アニメーションの再生。
void Player::PlayAnimation()
{
	//switch文。
	switch (playerState)
	{
		//プレイヤーステートが0(待機)だったら。
	case 0:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//プレイヤーステートが1(ジャンプ中)だったら。
	case 1:
		//ジャンプアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
	case 2:
		//歩きアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	}
		
}

void Player::Respawn()
{
	m_position = m_initialPosition + Vector3(0.0f, 10.0f, 0.0f); // 少し浮かせる
	m_moveSpeed = Vector3::Zero;
	m_characterController.SetPosition(m_position);
}


void Player::BlowOff(const Vector3& fromPosition)
{
	Vector3 blowDir = m_position - fromPosition;
	blowDir.y = 0.0f;
	if (blowDir.LengthSq() > 0.01f)
	{
		blowDir.Normalize();
		m_moveSpeed = blowDir * 100.0f;  // 吹っ飛び速度
		m_moveSpeed.y = 360.0f;          // 少し上に飛ばす
	}
}

void Player::AddSpeedBoost(float boost, float duration)
{
	m_speedMultiplier = boost;   // 例: 2.0f で2倍速
	m_boostTimer = duration;     // 例: 5.0f 秒間有効
}

	



//描画処理。
void Player::Render(RenderContext& rc)
{
	//ユニティちゃんを描画する。
	m_modelRender.Draw(rc);
}
	
