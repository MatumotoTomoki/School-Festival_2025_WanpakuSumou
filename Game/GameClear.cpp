#include "stdafx.h"
#include "GameClear.h"
#include "Title.h";
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

bool GameClear::Start()
{
    m_spriteRender.Init("Assets/sprite/GameClar.dds", 1920.0f, 1080.0f);

    wchar_t text[64];
    swprintf_s(text, L"クリアタイム: %.2f秒", m_clearTime);
    m_fontRender.SetText(text);
    m_fontRender.SetPosition({ 0.0f, 0.0f, 0.0f });
    m_fontRender.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    g_soundEngine->ResistWaveFileBank(3, "Assets/sound/GameClear.wav");
    SoundSource* bgm = NewGO<SoundSource>(0);
    bgm->Init(3);      
    bgm->Play(false);   
    bgm->SetVolume(3.5f);  // 音量調整


    return true;
}

void GameClear::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA)) {
        NewGO<Title>(0);     // タイトルに戻る
        DeleteGO(this);      // このオブジェクトを削除
    }
}


void GameClear::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
    m_fontRender.Draw(rc);
}
