#include "GameLayer.h"

GameLayer::GameLayer()
{
	m_Rendering = true;
}

GameLayer::GameLayer( const GameLayer& Other )
{
	m_PosX = Other.m_PosX;
	m_PosY = Other.m_PosY;
	m_Width = Other.m_Width;
	m_Height = Other.m_Height;
	m_Name = Other.m_Name;
	m_Rendering = Other.m_Rendering;

	m_Viewport = Other.m_Viewport;
}

GameLayer::~GameLayer()
{
	Destroy();
}

void GameLayer::Init( int PosX, int PosY, int Width, int  Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;

	CreateViewport();
	CreateProjectionMatrix();
	CreateOrthoMatrix();
}

void GameLayer::Render( ID3D11DeviceContext* DeviceContext )
{
	DeviceContext->RSSetViewports( 1, &m_Viewport );
}

void GameLayer::CreateViewport()
{
	m_Viewport.TopLeftX = (float)m_PosX;
	m_Viewport.TopLeftY = (float)m_PosY;
	m_Viewport.Width = (float)m_Width;
	m_Viewport.Height = (float)m_Height;

	m_Viewport.MinDepth = (float)SCREEN_NEAR;
	m_Viewport.MaxDepth = (float)SCREEN_DEPTH;
}

void GameLayer::CreateProjectionMatrix()
{
	float FieldOfView = XM_PI / 4.0f;
	float ScreenAspect = (float)m_Width / (float)m_Height;

	m_ProjectionMatrix = XMMatrixPerspectiveFovLH( FieldOfView, ScreenAspect, SCREEN_NEAR, SCREEN_DEPTH );
}

void GameLayer::CreateOrthoMatrix()
{
	float FieldOfView = XM_PI / 4.0f;
	float ScreenAspect = (float)m_Width / (float)m_Height;

	m_OrthoMatrix = XMMatrixOrthographicLH( FieldOfView, ScreenAspect, SCREEN_NEAR, SCREEN_DEPTH );
}

void GameLayer::Destroy()
{

}

D3D11_VIEWPORT& GameLayer::GetViewport() { return m_Viewport; }
XMMATRIX& GameLayer::GetProjectionMatrix() { return m_ProjectionMatrix; }
XMMATRIX& GameLayer::GetOrthoMatrix() { return m_OrthoMatrix; }