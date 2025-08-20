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
	//ƒ‚ƒfƒ‹‚ğ•`‰æ‚·‚éB

	modelRender.Draw(rc);
}