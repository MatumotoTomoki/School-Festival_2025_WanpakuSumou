#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include"Loading.h"
Title::Title()
{
    //画像を読み込む。
    spriteRender.Init("Assets/sprite/taitoru.dds", 1920.0f, 1080.0f);
    m_howToPlaySprite.Init("Assets/sprite/asobikata.dds", 1920.0f, 1080.0f);
}



Title::~Title()
{

}

void Title::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        if (!m_shwHowtoplay)
        {
            //最初のAボタン タイトル
            m_shwHowtoplay = true;
        }
        else
        {
            //二回目のAボタン　遊び方
            NewGO<Loading>(0);
            DeleteGO(this);
        }
    }
}

void Title::Render(RenderContext& rc)
{
    if (!m_shwHowtoplay)
    {
        spriteRender.Draw(rc); //タイトル画面
   }
    else
    {
        m_howToPlaySprite.Draw(rc); //遊び方画面
    }
}
