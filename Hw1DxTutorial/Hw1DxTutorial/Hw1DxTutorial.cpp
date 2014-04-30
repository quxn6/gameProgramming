#include "stdafx.h"

LPDIRECT3D9		g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9	g_pVB = NULL;
LPDIRECT3DTEXTURE9	g_pTexture01 = NULL;
LPDIRECT3DTEXTURE9	g_pTexture02 = NULL;

LPD3DXMESH		g_pMesh = NULL;
D3DMATERIAL9*	g_pMeshMaterials = NULL;
LPDIRECT3DTEXTURE9* g_pMeshTexture = NULL;
DWORD			g_dwNumMaterials = 0L;

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;             
	//D3DCOLOR color;
	FLOAT tu, tv;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		return E_FAIL;
	}

	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	g_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	return S_OK;
}

VOID SetupNormalVector()
{
	if (!(g_pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		//가지고 있지 않다면 메쉬를 복제하고 D3DFVF_NORMAL을 추가한다.
		ID3DXMesh* pTempMesh = 0;
		g_pMesh->CloneMeshFVF(
			D3DXMESH_MANAGED,
			g_pMesh->GetFVF() | D3DFVF_NORMAL,  //이곳에 추가
			g_pD3DDevice,
			&pTempMesh);

		// 법선을 계산한다.
		D3DXComputeNormals(pTempMesh, 0);

		g_pMesh->Release(); // 기존메쉬를 제거한다
		g_pMesh = pTempMesh; // 기존메쉬를 법선이 계산된 메쉬로 지정한다.
	}
}

HRESULT InitGeometry()
{
	if ( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, L"banana.bmp", &g_pTexture01 ) )  )
	{
		if ( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, L"..\\banana.bmp", &g_pTexture01 ) ) )
		{
			MessageBox( NULL, L"no banana", L"Textures.exe", MB_OK );
			return E_FAIL;
		}
	}
	if ( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, L"koala.bmp", &g_pTexture02 ) ) )
	{
		if ( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, L"..\\koala.bmp", &g_pTexture02 ) ) )
		{
			MessageBox( NULL, L"no koala", L"Textures.exe", MB_OK );
			return E_FAIL;
		}
	}



	if (FAILED(g_pD3DDevice->CreateVertexBuffer(
		50 * 2 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	CUSTOMVERTEX* pVertices;

	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI*i) / (50 - 1);
		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		pVertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		//pVertices[2 * i + 0].color = 0xffffffff;
		pVertices[2 * i + 0].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 0].tv = 1.0f;


		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
		pVertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		//pVertices[2 * i + 0].color = 0xff808080;
		pVertices[2 * i + 1].tu = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 1].tv = 0.0f;
	}

	g_pVB->Unlock();


	// draw tiger

	LPD3DXBUFFER pD3DXMtrlBuffer;

	if (FAILED(D3DXLoadMeshFromX(L"bong.x", D3DXMESH_SYSTEMMEM, g_pD3DDevice, NULL, &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, &g_pMesh)))
	{
		if (FAILED(D3DXLoadMeshFromX(L"..\\bong.x", D3DXMESH_SYSTEMMEM, g_pD3DDevice, NULL, &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, &g_pMesh)))
		{
			MessageBox(NULL, L"No tiger.x", L"Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
	if (g_pMeshMaterials == NULL)
	{
		return E_OUTOFMEMORY;
	}

	g_pMeshTexture = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
	if (g_pMeshTexture == NULL)
	{
		return E_OUTOFMEMORY;
	}

	for (DWORD i = 0; i < g_dwNumMaterials; i++)
	{
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		g_pMeshTexture[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			const CHAR* strPrefix = "..\\";
			CHAR strTexture[MAX_PATH];
			strcpy_s(strTexture, MAX_PATH, strPrefix);
			strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);

			if (FAILED(D3DXCreateTextureFromFileA(g_pD3DDevice, strTexture, &g_pMeshTexture[i])))
			{
				MessageBox(NULL, L"no texture map", L"Meshes.exe", MB_OK);
			}
		}
	}

	pD3DXMtrlBuffer->Release();

	SetupNormalVector();

	return S_OK;

}


VOID Cleanup()
{
	if ( g_pTexture01 != NULL )
	{
		g_pTexture01->Release();
	}

	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
	}

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
	}

	if (g_pVB != NULL)
	{
		g_pVB->Release();
	}

	if ( g_pMeshTexture )
	{
		for ( DWORD i = 0; i < g_dwNumMaterials; ++i )
		{
			if ( g_pMeshTexture[i] )
			{
				g_pMeshTexture[i]->Release();
			}
		}
		delete[] g_pMeshTexture;
	}

	if ( g_pMeshMaterials != NULL )
	{
		delete[] g_pMeshMaterials;
	}

	if ( g_pMesh != NULL )
	{
		g_pMesh->Release();
	}

}

VOID SetupCylinderMatrix()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationX(&matWorld, timeGetTime() / 1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

}

// VOID SetupTigerMatrix( float x, float y, float z )
// {
// 	D3DXMATRIXA16 tigerWorld, tmp1, tmp2;
// 	D3DXMatrixIdentity( &tigerWorld );
// 	D3DXMatrixTranslation( &tmp1, x, y, z );
// 	D3DXMatrixRotationY( &tmp2, timeGetTime() / 1000.0f );
// 	D3DXMatrixMultiply( &tigerWorld, &tmp1, &tmp2 );
// 	g_pD3DDevice->SetTransform( D3DTS_WORLD, &tigerWorld );
// 	g_pD3DDevice->MultiplyTransform( D3DTS_WORLD, &tmp2 );
// }

D3DXMATRIXA16 tigerWorld;
D3DXMATRIXA16 bonggoWorld;

VOID SetupTigerMatrix( float x, float y, float z )
{
//	D3DXMATRIXA16 tigerWorld, tmp1, tmp2;
	
	D3DXQUATERNION	quaternionRotation;
	
	D3DXVECTOR3 position( 5.0f, .0f, .0f );
	D3DXVECTOR3 rotation( .0f, 90.0f, .0f );
	D3DXVECTOR3 scale( 1.0f, 1.0f, 1.0f );

	D3DXMatrixIdentity( &tigerWorld );	
	
	D3DXQuaternionRotationYawPitchRoll( &quaternionRotation, D3DXToRadian( rotation.y ), D3DXToRadian( rotation.x), D3DXToRadian( rotation.z) );

		
//	D3DXMatrixTranslation( &tmp1, x, y, z );
// 	D3DXMatrixRotationY( &tmp2, timeGetTime() / 1000.0f );
// 	D3DXMatrixMultiply( &tigerWorld, &tmp1, &tmp2 );


	D3DXMatrixTransformation( &tigerWorld, NULL, NULL, &scale, NULL, &quaternionRotation, &position );

	//D3DXMatrixAffineTransformation( &tigerWorld, 1.0f, NULL, &quaternionRotation, &transf );

	g_pD3DDevice->SetTransform( D3DTS_WORLD, &tigerWorld );
//	g_pD3DDevice->MultiplyTransform( D3DTS_WORLD, &tmp2 );
}

VOID SetupBonggoMatrix( float x, float y, float z )
{
	D3DXQUATERNION	quaternionRotation;

	D3DXVECTOR3 position( .0f, 5.0f, .0f );
	D3DXVECTOR3 rotation( .0f, 90.0f, .0f );
	D3DXVECTOR3 scale( 1.0f, 1.0f, 1.0f );

	D3DXMatrixIdentity( &bonggoWorld );

	D3DXQuaternionRotationYawPitchRoll( &quaternionRotation, D3DXToRadian( rotation.y ), D3DXToRadian( rotation.x ), D3DXToRadian( rotation.z ) );
	
	D3DXMatrixTransformation( &bonggoWorld, NULL, NULL, &scale, NULL, &quaternionRotation, &position );

	g_pD3DDevice->SetTransform( D3DTS_WORLD, &bonggoWorld );
}

VOID SetupBonggoMatrix2( float x, float y, float z )
{
	D3DXQUATERNION	quaternionRotation;

	D3DXMATRIXA16 bonggoWorld2;
	D3DXMatrixIdentity( &bonggoWorld2 );
	D3DXMatrixMultiply( &bonggoWorld2, &bonggoWorld, &tigerWorld );
	g_pD3DDevice->SetTransform( D3DTS_WORLD, &bonggoWorld2 );
}

VOID SetupBonggoMatrix3( float x, float y, float z )
{
	D3DXQUATERNION	quaternionRotation;

	D3DXMATRIXA16 bonggoWorld3;
	D3DXMatrixIdentity( &bonggoWorld3 );
	D3DXMatrixMultiply( &bonggoWorld3, &tigerWorld, &bonggoWorld );
	g_pD3DDevice->SetTransform( D3DTS_WORLD, &bonggoWorld3 );
}

VOID SetupMatrices()
{
	D3DXVECTOR3 vEyePt(0.0f, 20.0f, 0.0f);	
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec(0.0f, .0f, 1.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4 , 1.0f, 1.0f, 1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

}


VOID SetupMaterials()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

	g_pD3DDevice->SetMaterial(&mtrl);


}

VOID SetupLights01()
{

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	float a = ( rand() % 10 );
	float b = a/10;

	light.Diffuse.r = b;
	light.Diffuse.g = b;
	light.Diffuse.b = b;
	vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
		1.0f,
		sinf(timeGetTime() / 350.f));

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	g_pD3DDevice->SetLight(0, &light);
	g_pD3DDevice->LightEnable(0, TRUE);

	g_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );

}

VOID SetupLights02()
{
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = .0f;
	light.Diffuse.b = .0f;
	vecDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;
	g_pD3DDevice->SetLight(0, &light);
	g_pD3DDevice->LightEnable(0, TRUE);

}

VOID SetupTexture01()
{
	g_pD3DDevice->SetTexture( 0, g_pTexture01 );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );	
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
}

VOID SetupTexture02()
{
	g_pD3DDevice->SetTexture( 0, g_pTexture02 );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
}


VOID Render()
{
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100,100, 100), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		if (1 == (timeGetTime() / 1000) % 2)
		{
			SetupLights01();
		}
		else
		{
			SetupLights02();			
		}



// 		SetupCylinderMatrix();
// 		SetupMaterials();
// 
// 		if ( 1 == ( timeGetTime() / 2000 ) % 2 )
// 		{
// 			SetupTexture01();
// 		}
// 		else
// 		{
// 			SetupTexture02();
// 		}
// 		g_pD3DDevice->SetStreamSource( 0, g_pVB, 0, sizeof( CUSTOMVERTEX ) );
// 		g_pD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
// 		g_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2 );


		SetupTigerMatrix( 3.0f, 0.0f, 0.0f );
		for ( DWORD i = 0; i < g_dwNumMaterials; ++i )
		{
			g_pD3DDevice->SetMaterial( &g_pMeshMaterials[i] );
			g_pD3DDevice->SetTexture( 0, g_pMeshTexture[i] );

			g_pMesh->DrawSubset( i );
		}

		SetupBonggoMatrix( -3.0f, 0.0f, 0.0f );
		for ( DWORD i = 0; i < g_dwNumMaterials; ++i )
		{
			g_pD3DDevice->SetMaterial( &g_pMeshMaterials[i] );
			g_pD3DDevice->SetTexture( 0, g_pMeshTexture[i] );

			g_pMesh->DrawSubset( i );
		}
		
		SetupBonggoMatrix2( -3.0f, 0.0f, 0.0f );
		for ( DWORD i = 0; i < g_dwNumMaterials; ++i )
		{
			g_pD3DDevice->SetMaterial( &g_pMeshMaterials[i] );
			g_pD3DDevice->SetTexture( 0, g_pMeshTexture[i] );

			g_pMesh->DrawSubset( i );
		}
		SetupBonggoMatrix3( -3.0f, 0.0f, 0.0f );
		for ( DWORD i = 0; i < g_dwNumMaterials; ++i )
		{
			g_pD3DDevice->SetMaterial( &g_pMeshMaterials[i] );
			g_pD3DDevice->SetTexture( 0, g_pMeshTexture[i] );

			g_pMesh->DrawSubset( i );
		}

		SetupMatrices();

		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D tutorial", NULL
	};
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(L"D3D tutorial", L"D3D hw1_dxtutorial",
		WS_OVERLAPPEDWINDOW, 100, 100, 800, 800,
		NULL, NULL, wc.hInstance, NULL);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitGeometry()) ) 
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));

			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					Render();
				}
			}

		}

	}

	UnregisterClass(L"D3D tutorial", wc.hInstance);
	return 0;
}