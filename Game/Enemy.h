#pragma once
//�G�l�~�[�N���X
class Player;
class Game;

class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//��]�����B
	void Rotation();
	//�ړ������B
	void Move();
	//�v���C���[�Ǐ]
	void SetPlayer(Player* player) { m_player = player; }
	void BlowBack(const Vector3& fromPosition);
	void Respawn();
	void SetGame(Game* game) { m_game = game; }
	Game* m_game = nullptr;

	//�����o�[�ϐ��B
	ModelRender m_modelRender;//���f�������_�[�B
	Vector3 m_position;//���W
	CharacterController m_characterController;//�L�����N�^�[�R���g���[���[�B
	Quaternion m_rotation;  //�N�H�[�^�j�I���B
	Vector3 m_moveSpeed;//�ړ����x�B
	Player* m_player = nullptr;//�v���C���[�Ɍ������ē���
	bool m_isBlowBack = false;      // ������ђ����ǂ���
	int m_blowBackTimer = 0;        // ������ю��ԁi�t���[���P�ʁj
	Vector3 m_initialPosition; // �����ʒu

	
};

