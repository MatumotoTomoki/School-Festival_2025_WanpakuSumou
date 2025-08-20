#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	//�A�j���[�V�����O���b�v�����[�h����B
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	//���j�e�B�����̃��f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_characterController.Init(25.0f, 75.0f,m_position);
}

bool Player::Start()
{
	m_initialPosition = m_position;  // �����ʒu�Ƃ��Č��݈ʒu��ۑ�

	// ���̑��������R�[�h
	return true;
}

Player::~Player()
{

}

//�X�V�����B
void Player::Update()
{
	// �����`�F�b�N
	if (m_position.y < -200.0f) {
		Respawn();
	}
	//�ړ������B
	Move();

	//��]�����B
	Rotation();

	//�X�e�[�g�Ǘ��B
	ManageState();

	//�A�j���[�V�����̍Đ��B
	PlayAnimation();

	//�G�`������̍X�V�����B
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
	

	
	
		
		//xz�̈ړ����x��0.0f�ɂ���B
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;

		//���X�e�B�b�N�̓��͗ʂ��擾�B
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();

		//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
		Vector3 forward = g_camera3D->GetForward();
		Vector3 right = g_camera3D->GetRight();
		//y�����ɂ͈ړ������Ȃ��B
		forward.y = 0.0f;
		right.y = 0.0f;

		//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
		right *= stickL.x * 360.0f * m_speedMultiplier;
		forward *= stickL.y * 360.0f * m_speedMultiplier;

		//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
		m_moveSpeed += right + forward;
	


	//�n�ʂɕt���Ă�����B
	if (m_characterController.IsOnGround())
	{
		//�d�͂𖳂����B
		m_moveSpeed.y = 0.0f;
		//A�{�^���������ꂽ��B
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�W�����v������B
			m_moveSpeed.y = 240.0f;
		}
	}
	//�n�ʂɕt���Ă��Ȃ�������B
	else
	{
		//�d�͂𔭐�������B
		m_moveSpeed.y -= 5.5f;
	}

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//�G�`������ɉ�]��������B
		m_modelRender.SetRotation(m_rotation);
	}
}

//�X�e�[�g�Ǘ��B
void Player::ManageState()
{
	//�n�ʂɕt���Ă��Ȃ�������B
	if (m_characterController.IsOnGround() == false)
	{
		//�X�e�[�g��1(�W�����v��)�ɂ���B
		playerState = 1;
		//������ManageState�̏������I��点��B
		return;
	}

	//�n�ʂɕt���Ă�����B
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�X�e�[�g��2(����)�ɂ���B
		playerState = 2;
	}
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��0(�ҋ@)�ɂ���B
		playerState = 0;
	}
}

//�A�j���[�V�����̍Đ��B
void Player::PlayAnimation()
{
	//switch���B
	switch (playerState)
	{
		//�v���C���[�X�e�[�g��0(�ҋ@)��������B
	case 0:
		//�ҋ@�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//�v���C���[�X�e�[�g��1(�W�����v��)��������B
	case 1:
		//�W�����v�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
	case 2:
		//�����A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	}
		
}

void Player::Respawn()
{
	m_position = m_initialPosition + Vector3(0.0f, 10.0f, 0.0f); // ������������
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
		m_moveSpeed = blowDir * 100.0f;  // ������ё��x
		m_moveSpeed.y = 360.0f;          // ������ɔ�΂�
	}
}

void Player::AddSpeedBoost(float boost, float duration)
{
	m_speedMultiplier = boost;   // ��: 2.0f ��2�{��
	m_boostTimer = duration;     // ��: 5.0f �b�ԗL��
}

	



//�`�揈���B
void Player::Render(RenderContext& rc)
{
	//���j�e�B������`�悷��B
	m_modelRender.Draw(rc);
}
	
