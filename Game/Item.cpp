#include "stdafx.h"
#include "Item.h"
Item::Item()
{
	modelRender.Init("Assets/modelData/Item.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
}

Item::~Item()
{

}

void Item::Render(RenderContext& rc)
{
	//���f����`�悷��B

	modelRender.Draw(rc);
}