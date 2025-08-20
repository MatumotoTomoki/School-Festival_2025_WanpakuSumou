#pragma once
class Loading : public IGameObject
{
public:
    Loading() {}
    ~Loading() {}
    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

private:
    SpriteRender m_spriteRender;
    FontRender m_loadingText;    // "Loading" –{‘Ì
    FontRender m_dot1;
    FontRender m_dot2;
    FontRender m_dot3;

    float m_timer = 0.0f;
    int m_frame = 0;
};

