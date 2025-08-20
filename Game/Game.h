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
	//�����o�[�ϐ��B
	Player* player = nullptr;			//�v���C���[�B
	GameCamera* gameCamera = nullptr;			//�Q�[���J�����B
	Stage* stage; //�X�e�[�W
	Enemy* enemy; //�G�l�~�[
	Item* item;
	FontRender fontRender; //����
	int m_defeatCount = 0;
	float m_elapsedTime = 0.0f; // �o�ߎ��ԁi�b�j
	FontRender timeFontRender; // ���ԕ\���p�̃t�H���g
	SoundSource* gamebgm;
	bool m_isGameClear = false;
	float m_itemRespawnTimer = 0.0f;
private:
	
	Vector3 m_pos;
	
};

