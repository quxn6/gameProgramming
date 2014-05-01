#pragma once
#include "DDObject.h"
#include "DDModel.h"

struct CUSTOMVERTEX
{
	FLOAT	x, y, z;
	DWORD	color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct BoundingBoxAABB
{
	D3DXVECTOR3 maxPoint;
	D3DXVECTOR3 minPoint;
};

class CollisionBox :
	public DDModel
{
public:
	CollisionBox();
	CollisionBox( std::wstring filePath );
	virtual ~CollisionBox();

	void InitCollisionBox();

	CREATE_OBJECT_WSTRING( CollisionBox, filePath );
	
protected:
	virtual void RenderItSelf();
	virtual void UpdateItSelf( float dTime );
	BoundingBoxAABB GetBoundingBoxAABB();	
	/*void DrawBoundingBoxAABB(BoundingBoxAABB boundingBox);*/

	BoundingBoxAABB m_BoundingBoxAABB;
	//LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
};

