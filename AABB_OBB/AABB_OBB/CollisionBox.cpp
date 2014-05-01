#include "stdafx.h"
#include "CollisionBox.h"
#include <assert.h>
#include <directxcollision.h>
#include <DirectXMath.h>


CollisionBox::CollisionBox()
{
}

CollisionBox::CollisionBox( std::wstring filePath ) : DDModel(filePath)
{

}


CollisionBox::~CollisionBox()
{
}

void CollisionBox::RenderItSelf()
{
	DDModel::RenderItSelf();	

	m_BoundingBoxAABB = GetBoundingBoxAABB();
// 	DirectX::XMFLOAT3 center = DirectX::XMFLOAT3(( m_BoundingBoxAABB.maxPoint + m_BoundingBoxAABB.minPoint ) / 2);
// 	DirectX::XMFLOAT3 extends = DirectX::XMFLOAT3( ( m_BoundingBoxAABB.maxPoint - m_BoundingBoxAABB.minPoint ) / 2 );
// 	DirectX::BoundingBox bb( &center, &extends );

// 	CUSTOMVERTEX Vertices[] =
// 	{
// 		{ m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.minPoint.z },
// 		{ m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.minPoint.z },
// 		{ m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.maxPoint.z },
// 		{ m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.maxPoint.z },
// 		{ m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.minPoint.z },
// 		{ m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.minPoint.z },
// 		{ m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.minPoint.z },
// 		{ m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.maxPoint.z },
// 		{ m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.maxPoint.z },
// 		{ m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.minPoint.z },
// 	};

	ID3DXLine *box;
	D3DXCreateLine( DDRenderer::GetInstance()->GetDevice(), &box );
	box->SetWidth( 1.5f );
	box->SetAntialias( true );

	D3DXVECTOR3 bb[10];
	// ±×¸®±â
 	D3DXMATRIX projMat, viewMat;
	DDRenderer::GetInstance()->GetDevice()->GetTransform( D3DTS_PROJECTION, &projMat );
	DDRenderer::GetInstance()->GetDevice()->GetTransform( D3DTS_VIEW, &viewMat );
	viewMat *= projMat;
	//DDRenderer::GetInstance()->GetDevice()->GetTransform( D3DTS_WORLD, &viewMat );

	bb[0] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.minPoint.z );
	bb[1] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.minPoint.z );
	bb[2] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.maxPoint.z );
	bb[3] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.maxPoint.z );
	bb[4] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.minPoint.z );
	bb[5] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.minPoint.z );
	bb[6] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.minPoint.z );
	bb[7] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.maxPoint.z );
	bb[8] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.maxPoint.z );
	bb[9] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.minPoint.z );
		
	box->Begin();
	box->DrawTransform( bb, 10, &viewMat, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
		
	bb[0] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.maxPoint.z );
	bb[1] = D3DXVECTOR3( m_BoundingBoxAABB.maxPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.maxPoint.z );
	box->DrawTransform( bb, 2, &viewMat, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );

	bb[0] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.maxPoint.z );
	bb[1] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.maxPoint.z );
	box->DrawTransform( bb, 2, &viewMat, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );

	bb[0] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.maxPoint.y, m_BoundingBoxAABB.minPoint.z );
	bb[1] = D3DXVECTOR3( m_BoundingBoxAABB.minPoint.x, m_BoundingBoxAABB.minPoint.y, m_BoundingBoxAABB.minPoint.z );
	box->DrawTransform( bb, 2, &viewMat, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );

	box->End();
	box->Release();

}

void CollisionBox::InitCollisionBox()
{
	
}

void CollisionBox::UpdateItSelf( float dTime )
{
}

BoundingBoxAABB CollisionBox::GetBoundingBoxAABB()
{
	unsigned int	VertexBufferSize = m_pMesh->GetNumVertices();
	BoundingBoxAABB boundingBox;

	if ( 0 == VertexBufferSize )
	{
		boundingBox.maxPoint = { .0f, .0f, .0f };
		boundingBox.minPoint = { .0f, .0f, .0f };
		return boundingBox;
	}

	D3DXVECTOR3* vertices;
	if ( SUCCEEDED( m_pMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&vertices ) ) )
	{
		boundingBox.maxPoint = vertices[0];
		boundingBox.minPoint = vertices[0];

		for ( unsigned int i = 1; i < VertexBufferSize; ++i )
		{
			if ( boundingBox.minPoint.x < vertices[i].x )
				boundingBox.minPoint.x = vertices[i].x;
			if ( boundingBox.maxPoint.x > vertices[i].x )
				boundingBox.maxPoint.x = vertices[i].x;

			if ( boundingBox.minPoint.y < vertices[i].y )
				boundingBox.minPoint.y = vertices[i].y;
			if ( boundingBox.maxPoint.y > vertices[i].y )
				boundingBox.maxPoint.y = vertices[i].y;

			if ( boundingBox.minPoint.z < vertices[i].z )
				boundingBox.minPoint.z = vertices[i].z;
			if ( boundingBox.maxPoint.z > vertices[i].z )
				boundingBox.maxPoint.z = vertices[i].z;
		}
		m_pMesh->UnlockVertexBuffer();
	}

	return boundingBox;
}

// void CollisionBox::DrawBoundingBoxAABB(  )
// {
// 
// }
