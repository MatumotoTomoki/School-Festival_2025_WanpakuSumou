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
	//�ʒu�����ɉ�����
	m_position.z += 500.0f;
	m_position.y -= 40.0f;
	m_initialPosition = m_position;
	//���f���Ɉʒu�𔽉f
	m_modelRender.SetPosition(m_position);
	m_characterController.Init(30.0f, 30.0f, m_position);
	//�����蔻�����������B
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/poyonn.wav");
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/tokutenn.wav");
}

Enemy::~Enemy()
{

}

void Enemy::Rotation()
{
	//�G�l�~�[��180�ǉ�]������
	m_rotation.SetRotationDegY(180.0f);

	//��]���G�`������ɋ�����B
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
		// Game�ɒʒm
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

		// �d�͏����i�n�ʂɐڒn���Ă邩����j
		if (m_characterController.IsOnGround()) {
			m_moveSpeed.y = 0.0f;
		}
		else {
			m_moveSpeed.y += -9.8f / 60.0f;
		}

		// ������ђ��̈ړ����������s���i�ʏ�ǐՂ̓X�L�b�v�j
		m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);
		m_modelRender.SetPosition(m_position);
		return;
	}
	if (m_player != nullptr)
	{
		//�G����v���C���[�ɂ̕����x�N�g�����v�Z
		Vector3 toPlayrer = m_player->GetPosition() - m_position;
		//Y���͖�������XZ���ʂ����ŕ������o��
		toPlayrer.y = 0.0f;
		//���K��(����1�ɂ���)
		toPlayrer.Normalize();
		//�ړ��X�s�[�h
		Vector3 moveDir = toPlayrer * 240.0f;
		//�ړ�����
		m_moveSpeed.x = moveDir.x;
		m_moveSpeed.z = moveDir.z;
	}
	//�n�ʂɕt���Ă�����B
	if (m_characterController.IsOnGround())
	{
		//�d�͂𖳂���
		m_moveSpeed.y = 0.0f;
	}
	else
	{
		//�d�͂𔭐�������
		m_moveSpeed.y = -9.8f / 60.0f; //�t���[�����Ƃ̏d�́B
	}
	//�L�����R���ňړ��v�Z(�����蔻����܂�)
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);

	//���f���̉�]���v���C���[�ɕ����Ɍ�����
	if (m_player != nullptr)
	{
		Vector3 dir = m_player->GetPosition() - m_position;
		dir.y = 0.0f;
		if (dir.LengthSq() > 0.01f)
		{
			dir.Normalize();
			//z���������ʂƂ��Ċp�x���擾
			float angle = atan2f(dir.x, dir.z);
			m_rotation.SetRotation(Vector3(0.0f, 1.0f, 0.0f), angle);
			m_modelRender.SetRotation(m_rotation);
		}
	}
	// �v���C���[�Ƃ̋����𑪂��ďՓ˂����琁����΂�
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
			// �G�l�~�[���v���C���[�̕����Ƌt�ɐ�����΂�
			BlowBack(m_player->GetPosition());
		}
	}
	// ��������iY���W�����ȉ��Ȃ烊�X�|�[���j
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
		m_moveSpeed = blowDir * 500.0f;  // �v���C���[���痣���悤�Ɉړ�
		m_moveSpeed.z = 480.0f; // ������������

		m_isBlowBack = true;
		m_blowBackTimer = 180; // 180�t���[���̊� ������у��[�h
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(7.5f);
	}
}

void Enemy::Respawn()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(2);  // WaveFileBank�ԍ�
	se->Play(false);  // false �� ��񂾂��Đ�
	se->SetVolume(4.0f);  // ���ʒ���
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
	//���f����`�悷��B

	m_modelRender.Draw(rc);
}
