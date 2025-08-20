#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "Stage.h"
#include "Enemy.h"
#include "GameClear.h"
#include"Item.h"
#include "sound/SoundEngine.h"
Game::Game()
{
	//�v���C���[�̃I�u�W�F�N�g�����B
	player = NewGO<Player>(0, "player");

	//�Q�[���J�����̃I�u�W�F�N�g�����B
	gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//�X�e�[�W�����
	Stage*stage = NewGO<Stage>(0,"stage");

	//�X�e�[�W�̈ʒu�����������ɂ���
	//stage->m_position = { -100.0f,0.0f,-100.0f };

	//�G�l�~�[���o��
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->Rotation();
	enemy->SetPlayer(player);

	//�A�C�e����\��
	item = NewGO<Item>(0, "item");

	//����
	fontRender.SetText(L"�|������:");
	fontRender.SetPosition({ -600.0f,100.0f,0.0f });
	fontRender.SetColor(g_vec4White);

	//����
	timeFontRender.SetPosition({ -600.0f, 50.0f, 0.0f }); // ��ʂ̏㕔�ɕ\��
	timeFontRender.SetColor(g_vec4White);

	enemy->SetGame(this);
	s_instance = this;

	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/GameBgm.wav");
	gamebgm = NewGO<SoundSource>(0);
	gamebgm->Init(0);
	gamebgm->Play(true);
}
Game::~Game()
{
	//�v���C���[���폜����B
	DeleteGO(player);
	//�Q�[���J�������폜����B
	DeleteGO(gameCamera);
	//�w�i���폜����B
	DeleteGO(stage);
	//�G�l�~�[���폜����B
	DeleteGO(enemy);
	//�Q�[��BGM���폜����B
	DeleteGO(gamebgm);
	//�A�C�e�����폜����B
	DeleteGO(item);
}

void Game::Update()
{
	if (!m_isGameClear) {
		m_elapsedTime += g_gameTime->GetFrameDeltaTime();  // 1�t���[�����Ƃ̉��Z
	}

	// ���Ԃ̕\�����X�V�i��Ɂj
	wchar_t timeText[64];
	swprintf_s(timeText, L"����: %.2f�b", m_elapsedTime);
	timeFontRender.SetText(timeText);
	
	//  �A�C�e��������Ƃ��i�����`�F�b�N�j
	if (item) {
		Vector3 playerPos = player->GetPosition();
		Vector3 itemPos = item->GetPosition();
		if ((playerPos - itemPos).Length() < 50.0f) {
			player->AddSpeedBoost(2.0f, 5.0f);  // 5�b��2�{��
			DeleteGO(item);
			item = nullptr;

			// 5�b��Ƀ��X�|�[��
			m_itemRespawnTimer = 5.0f;
		}
	}
	//  �A�C�e���������Ƃ��i���X�|�[���҂��j
	else {
		if (m_itemRespawnTimer > 0.0f) {
			m_itemRespawnTimer -= g_gameTime->GetFrameDeltaTime();
			if (m_itemRespawnTimer <= 0.0f) {
				// �A�C�e�����Đ���
				item = NewGO<Item>(0, "item");
			}
		}
	}
}


void Game::OnEnemyDefeated()
{
	m_defeatCount++;

	// �������X�V
	wchar_t text[64];
	swprintf_s(text, L"�|������: %d", m_defeatCount);
	fontRender.SetText(text);

	if (m_defeatCount >= 10 && !m_isGameClear)
	{
		m_isGameClear = true;
		GameClear* clear = NewGO<GameClear>(0, "gameclear");
		clear->SetClearTime(m_elapsedTime);

		DeleteGO(this);  // Game�V�[�����폜
	}

	if (item) {
		Vector3 playerPos = player->GetPosition();  // �� GetPosition() �� Player �Ɏ����K�v
		Vector3 itemPos = item->GetPosition();      // �� Item �ɂ����l
		if ((playerPos - itemPos).Length() < 50.0f) {  // ���a50�ȓ��Ȃ�擾
			player->AddSpeedBoost(2.0f, 5.0f);  // 5�b�ԃX�s�[�h2�{
			DeleteGO(item);   // �A�C�e���폜
			item = nullptr;
		}
	}
	
}


void Game::Render(RenderContext& rc)
{
	fontRender.Draw(rc); //�|������
	timeFontRender.Draw(rc);   // ����
}

Game* Game::s_instance = nullptr;

Game* Game::GetInstance() {
	return s_instance;
}