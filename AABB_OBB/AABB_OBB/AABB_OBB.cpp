// AABB_OBB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DDApplication.h"
#include "..\DDFramework\DDSceneDirector.h"
#include "..\DDFramework\DDScene.h"
#include "PlayScene.h"



int _tmain(int argc, _TCHAR* argv[])
{
	DDApplication::GetInstance()->Init( L"AABB_OBB", 750, 500 );
	
	PlayScene* scene = PlayScene::Create( L"scene" );
	scene->Init();
	DDSceneDirector::GetInstance()->ChangeScene(scene);

	DDApplication::GetInstance()->Run();

	DDApplication::ReleaseInstance();

	return 0;
}

