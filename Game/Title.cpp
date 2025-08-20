#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include"Loading.h"
Title::Title()
{
    //�摜��ǂݍ��ށB
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
            //�ŏ���A�{�^�� �^�C�g��
            m_shwHowtoplay = true;
        }
        else
        {
            //���ڂ�A�{�^���@�V�ѕ�
            NewGO<Loading>(0);
            DeleteGO(this);
        }
    }
}

void Title::Render(RenderContext& rc)
{
    if (!m_shwHowtoplay)
    {
        spriteRender.Draw(rc); //�^�C�g�����
   }
    else
    {
        m_howToPlaySprite.Draw(rc); //�V�ѕ����
    }
}
