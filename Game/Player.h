#pragma once
//�v���C���[�N���X�B
class Enemy;
class GameOver;
class Player :public IGameObject
{
public:
	Player();
	~Player();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//�ړ������B
	void Move();
	//��]�����B
	void Rotation();
	//�X�e�[�g�Ǘ��B
	void ManageState();
	//�A�j���[�V�����̍Đ��B
	void PlayAnimation();
	//�ʒu��ǂݎ��
	Vector3 GetPosition() const { return m_position; };
	//������΂�
	void BlowOff(const Vector3& fromPosition);
	bool Start();
	void Respawn();
	void AddSpeedBoost(float boost, float duration);
	float m_speedMultiplier = 1.0f;     // �ʏ�� 1.0
	float m_boostTimer = 0.0f;          // �c�莞�ԁi�b�j
	//�����o�[�ϐ��B
	ModelRender m_modelRender;//���f�������_�[�B
	Vector3 m_position;//���W
	enum EnAnimationClip {//�A�j���[�V�����B
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];//�A�j���[�V�����N���b�v�B
	CharacterController m_characterController;//�L�����N�^�[�R���g���[���[�B
	Vector3 m_moveSpeed;//�ړ����x�B
	Quaternion m_rotation;  //�N�H�[�^�j�I���B
	int playerState = 0;//�v���C���[�̃X�e�[�g�i��ԁj��\���ϐ��B
	Vector3 m_initialPosition;  // �����ʒu

};

