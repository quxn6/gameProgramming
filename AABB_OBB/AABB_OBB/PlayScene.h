#pragma once
#include "DDScene.h"

class DDLight;
class DDCamera;
class CollisionBox;

class PlayScene :
	public DDScene
{
public:
	PlayScene();
	PlayScene( std::wstring sceneName );
	virtual ~PlayScene();
		
	//static PlayScene* Create( std::wstring sceneName );
	CREATE_OBJECT_WSTRING( PlayScene , sceneName);

	void Init();


private:
	virtual void UpdateItSelf( float dTime );
	
	DDLight*		m_pDirectonalLight = nullptr;
	DDCamera*		m_Camera = nullptr;
	CollisionBox*	m_CollisionBox = nullptr;	
	DDPoint			m_PrevMousePosition{ .0f, .0f };
};

