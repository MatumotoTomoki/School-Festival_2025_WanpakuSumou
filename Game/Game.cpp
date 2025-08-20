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
	//プレイヤーのオブジェクトを作る。
	player = NewGO<Player>(0, "player");

	//ゲームカメラのオブジェクトを作る。
	gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//ステージを作る
	Stage*stage = NewGO<Stage>(0,"stage");

	//ステージの位置を少ししたにする
	//stage->m_position = { -100.0f,0.0f,-100.0f };

	//エネミーを出す
	enemy = NewGO<Enemy>(0, "enemy");
	enemy->Rotation();
	enemy->SetPlayer(player);

	//アイテムを表示
	item = NewGO<Item>(0, "item");

	//文字
	fontRender.SetText(L"倒した数:");
	fontRender.SetPosition({ -600.0f,100.0f,0.0f });
	fontRender.SetColor(g_vec4White);

	//時間
	timeFontRender.SetPosition({ -600.0f, 50.0f, 0.0f }); // 画面の上部に表示
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
	//プレイヤーを削除する。
	DeleteGO(player);
	//ゲームカメラを削除する。
	DeleteGO(gameCamera);
	//背景を削除する。
	DeleteGO(stage);
	//エネミーを削除する。
	DeleteGO(enemy);
	//ゲームBGMを削除する。
	DeleteGO(gamebgm);
	//アイテムを削除する。
	DeleteGO(item);
}

void Game::Update()
{
	if (!m_isGameClear) {
		m_elapsedTime += g_gameTime->GetFrameDeltaTime();  // 1フレームごとの加算
	}

	// 時間の表示を更新（常に）
	wchar_t timeText[64];
	swprintf_s(timeText, L"時間: %.2f秒", m_elapsedTime);
	timeFontRender.SetText(timeText);
	
	//  アイテムがあるとき（距離チェック）
	if (item) {
		Vector3 playerPos = player->GetPosition();
		Vector3 itemPos = item->GetPosition();
		if ((playerPos - itemPos).Length() < 50.0f) {
			player->AddSpeedBoost(2.0f, 5.0f);  // 5秒間2倍速
			DeleteGO(item);
			item = nullptr;

			// 5秒後にリスポーン
			m_itemRespawnTimer = 5.0f;
		}
	}
	//  アイテムが無いとき（リスポーン待ち）
	else {
		if (m_itemRespawnTimer > 0.0f) {
			m_itemRespawnTimer -= g_gameTime->GetFrameDeltaTime();
			if (m_itemRespawnTimer <= 0.0f) {
				// アイテムを再生成
				item = NewGO<Item>(0, "item");
			}
		}
	}
}


void Game::OnEnemyDefeated()
{
	m_defeatCount++;

	// 文字を更新
	wchar_t text[64];
	swprintf_s(text, L"倒した数: %d", m_defeatCount);
	fontRender.SetText(text);

	if (m_defeatCount >= 10 && !m_isGameClear)
	{
		m_isGameClear = true;
		GameClear* clear = NewGO<GameClear>(0, "gameclear");
		clear->SetClearTime(m_elapsedTime);

		DeleteGO(this);  // Gameシーンを削除
	}

	if (item) {
		Vector3 playerPos = player->GetPosition();  // ← GetPosition() を Player に実装必要
		Vector3 itemPos = item->GetPosition();      // ← Item にも同様
		if ((playerPos - itemPos).Length() < 50.0f) {  // 半径50以内なら取得
			player->AddSpeedBoost(2.0f, 5.0f);  // 5秒間スピード2倍
			DeleteGO(item);   // アイテム削除
			item = nullptr;
		}
	}
	
}


void Game::Render(RenderContext& rc)
{
	fontRender.Draw(rc); //倒した数
	timeFontRender.Draw(rc);   // 時間
}

Game* Game::s_instance = nullptr;

Game* Game::GetInstance() {
	return s_instance;
}