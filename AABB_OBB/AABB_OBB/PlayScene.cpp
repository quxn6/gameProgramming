#include "stdafx.h"
#include "PlayScene.h"
#include "DDInputSystem.h"
#include "DDApplication.h"
#include "DDLight.h"
#include "DDUI.h"
#include "CollisionBox.h"
#include "..\DDFramework\DDCamera.h"
#include "directxcollision.h"

PlayScene::PlayScene()
{
	m_SceneName = L"DefaultPlayScene";
}

PlayScene::PlayScene( std::wstring sceneName )
{
	m_SceneName = sceneName;
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{	
	m_PrevMousePosition = DDInputSystem::GetInstance()->GetMousePosition();
	m_pDirectonalLight = DDLight::Create();
	AddChild( m_pDirectonalLight );	
	m_CollisionBox = CollisionBox::Create( L"debris.x" );
	m_CollisionBox->InitCollisionBox();
	m_CollisionBox->SetPosition( .0f, .0f, .0f );
	//m_CollisionBox->SetScale( 2.0f, 2.0f, 2.0f );
	AddChild( m_CollisionBox );

	m_Camera = DDCamera::Create();
	m_Camera->SetPosition( .0f, 5.0f, -5.0f );
	m_Camera->SetRotation( 50.0f, .0f, .0f );
	AddChild( m_Camera );
}

void PlayScene::UpdateItSelf( float dTime )
{
	UNREFERENCED_PARAMETER( dTime );

	if ( KEY_DOWN == GetKeyState( VK_SPACE ) )
	{

	}
	if ( KEY_PRESSED == GetKeyState( 0x57 ) ) //w
	{
		m_CollisionBox->IncreasePositionZ( dTime );
	}
	if ( KEY_PRESSED == GetKeyState( 0x53 ) ) //s
	{
		m_CollisionBox->IncreasePositionZ( -dTime );
	}
	if ( KEY_PRESSED == GetKeyState( 0x41 ) ) //a
	{
		m_CollisionBox->IncreasePositionX( -dTime );
	}
	if ( KEY_PRESSED == GetKeyState( 0x44 ) ) //d
	{
		m_CollisionBox->IncreasePositionX( dTime );
	}

	DDPoint currentMousePos = DDInputSystem::GetInstance()->GetMousePosition();
	m_Camera->IncreasePosition( D3DXVECTOR3(
		currentMousePos.GetX() - m_PrevMousePosition.GetX(), 
		0.0f,		
		-currentMousePos.GetY() + m_PrevMousePosition.GetY() )/ 100.0f
		);
	m_PrevMousePosition = currentMousePos;

}
