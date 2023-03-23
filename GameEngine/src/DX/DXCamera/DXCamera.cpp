#include "DXCamera.h"

DXCAMERA::DXCAMERA(){}

DXCAMERA::DXCAMERA( const DXCAMERA* Other )
{
	*this = *Other;
}

DXCAMERA::~DXCAMERA(){}

void DXCAMERA::Init()
{
	m_Info.Position = XMFLOAT3( 0.0f, 0.0f, -2.0f );
	m_Info.Target = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Info.Up = XMFLOAT3( 0.0f, 1.0f, 0.0f );

	m_DefaultForward = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
	m_DefaultRight = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f );

	m_Yaw = 0.0f;
	m_Pitch = 0.0f;

	m_MoveLeftRight = 0.0f;
	m_MoveBackForward = 0.0f;
}

void DXCAMERA::Frame()
{
	m_CamPosition = XMLoadFloat3( &m_Info.Position );
	m_CamTarget = XMLoadFloat3( &m_Info.Target );
	m_CamUp = XMLoadFloat3( &m_Info.Up );

	m_RotationMatrix = XMMatrixRotationRollPitchYaw( m_Pitch, m_Yaw, 0 );
	m_CamTarget = XMVector3TransformCoord( m_DefaultForward, m_RotationMatrix );
	m_CamTarget = XMVector3Normalize( m_CamTarget );

	XMMATRIX RotationYawMatrix = XMMatrixRotationY( m_Yaw );

	m_Right = XMVector3TransformCoord( m_DefaultRight, RotationYawMatrix );
	m_CamUp = XMVector3TransformCoord( m_CamUp, RotationYawMatrix );
	m_Forward = XMVector3TransformCoord( m_DefaultForward, RotationYawMatrix );

	m_CamPosition += m_MoveLeftRight * m_Right + m_MoveBackForward * m_Forward;

	m_CamTarget += m_CamPosition;

	m_ViewMatrix = XMMatrixLookAtLH( m_CamPosition, m_CamTarget, m_CamUp );

	m_MoveBackForward = 0.0f;
	m_MoveLeftRight = 0.0f;

	XMStoreFloat3( &m_Info.Position, m_CamPosition );
	XMStoreFloat3( &m_Info.Target, m_CamTarget );
	XMStoreFloat3( &m_Info.Up, m_CamUp );
}

CAMERABUFFER DXCAMERA::GetCameraInfo() { return m_Info; }
XMMATRIX DXCAMERA::GetViewMatrix() { return m_ViewMatrix; }