#pragma once
class GameClear : public IGameObject
{
public:
    GameClear() {}
    ~GameClear() {}

    void SetClearTime(float time) { m_clearTime = time; }

    bool Start() override;
    void Render(RenderContext& rc) override;
    void Update();
private:
    float m_clearTime = 0.0f;
    SpriteRender m_spriteRender;
    FontRender m_fontRender;
};
