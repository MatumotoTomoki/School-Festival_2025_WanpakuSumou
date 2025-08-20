#include "stdafx.h"
#include "Loading.h"
#include "Game.h"
#include <cmath> // sin関数用

bool Loading::Start()
{
    m_spriteRender.Init("Assets/sprite/loading.dds", 1920.0f, 1080.0f);

    // "Loading"本体
    m_loadingText.SetText(L"Loading");
    m_loadingText.SetPosition({ -120.0f, -200.0f, 0.0f });
    m_loadingText.SetScale(1.5f);
    m_loadingText.SetColor({ 1,1,1,1 });

    // 「.」「.」「.」
    m_dot1.SetText(L".");
    m_dot2.SetText(L".");
    m_dot3.SetText(L".");

    m_dot1.SetScale(1.5f);
    m_dot2.SetScale(1.5f);
    m_dot3.SetScale(1.5f);

    m_dot1.SetColor({ 1,1,1,1 });
    m_dot2.SetColor({ 1,1,1,1 });
    m_dot3.SetColor({ 1,1,1,1 });

    return true;
}

void Loading::Update()
{
    m_timer += 0.1f;
    m_frame++;

    float baseY = -200.0f;
    float amplitude = 10.0f;

    // sin波でそれぞれ少しずつタイミングをずらす
    m_dot1.SetPosition({ 100.0f, baseY + sinf(m_timer) * amplitude, 0.0f });
    m_dot2.SetPosition({ 130.0f, baseY + sinf(m_timer + 0.5f) * amplitude, 0.0f });
    m_dot3.SetPosition({ 160.0f, baseY + sinf(m_timer + 1.0f) * amplitude, 0.0f });

    // 任意でゲームに遷移（3秒後とか）
    if (m_frame > 600) {
        NewGO<Game>(0);
        DeleteGO(this);
    }
}

void Loading::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
    m_loadingText.Draw(rc);
    m_dot1.Draw(rc);
    m_dot2.Draw(rc);
    m_dot3.Draw(rc);
}
