#pragma once
class Stage : public IGameObject
{
public:
	Stage();
	~Stage();
	//描画関数。
	void Render(RenderContext& rc);
	
	//モデルレンダ―。        
	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;
	Vector3 m_position;

};

