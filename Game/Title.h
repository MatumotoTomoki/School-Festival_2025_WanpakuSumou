#pragma once
class Loading;
class Title : public IGameObject
{
public:
    Title();
    ~Title();
    //�`��֐��B
    void Render(RenderContext& rc);
    //�X�V�����B
    void Update();
    bool m_shwHowtoplay = false;
    //�X�v���C�g�����_�\�B
    SpriteRender spriteRender;
    SpriteRender m_howToPlaySprite;
};

