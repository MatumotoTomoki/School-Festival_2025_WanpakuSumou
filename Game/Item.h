#pragma once
class Item : public IGameObject
{
public:
	Item();
	~Item();
	void Render(RenderContext& rc);
	Vector3 GetPosition() const { return m_position; };

	//���f�������_�\�B        
	ModelRender modelRender;
	PhysicsStaticObject physicsStaticObject;
	Vector3 m_position;
};

