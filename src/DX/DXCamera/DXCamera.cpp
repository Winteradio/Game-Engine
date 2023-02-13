#include "DXCamera.h"

DXCAMERA::DXCAMERA(){}

DXCAMERA::DXCAMERA( const DXCAMERA* Other )
{
	*this = *Other;
}

DXCAMERA::~DXCAMERA(){}

void DXCAMERA::Init()
{
	m_Info.Position = XMFLOAT3( 0.0f, 3.0f, -10.0f );
	m_Info.Target = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Info.Up = XMFLOAT3( 0.0f, 1.0f, 0.0f );
}

void DXCAMERA::Frame()
{
	XMVECTOR Position, Target, Up;
	Position = XMLoadFloat3( &m_Info.Position );
	Target = XMLoadFloat3( &m_Info.Target );
	Up = XMLoadFloat3( &m_Info.Up );
	m_ViewMatrix = XMMatrixLookAtLH( Position, Target, Up );
}

CAMERABUFFER DXCAMERA::GetCameraInfo() { return m_Info; }
XMMATRIX DXCAMERA::GetViewMatrix() { return m_ViewMatrix; }