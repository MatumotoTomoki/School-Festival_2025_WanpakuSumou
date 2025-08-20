#pragma once
class Loading;
class Title : public IGameObject
{
public:
    Title();
    ~Title();
    //描画関数。
    void Render(RenderContext& rc);
    //更新処理。
    void Update();
    bool m_shwHowtoplay = false;
    //スプライトレンダ―。
    SpriteRender spriteRender;
    SpriteRender m_howToPlaySprite;
};

