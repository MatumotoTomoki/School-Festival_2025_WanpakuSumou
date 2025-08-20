#include "stdafx.h"
#include "Stage.h"

Stage::Stage()
{		
	//“–‚½‚è”»’è
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	modelRender.Init("Assets/modelData/strs1.tkm");
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());

}
Stage::~Stage()
{

}

void Stage::Render(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚ğ•`‰æ‚·‚éB

	modelRender.Draw(rc);
}