#ifndef __DXCAMERA_H__
#define __DXCAMERA_H__

#include "DXCommon.h"
#include "DXBuffer.h"

#define PI 3.14159265358979323846

class DXCAMERA
{
	public :
		DXCAMERA();
		DXCAMERA( const DXCAMERA* Other );
		~DXCAMERA();

	// Function - Render
	public :
		void Init();
		void Frame();
		CAMERABUFFER GetCameraInfo();
		XMMATRIX GetViewMatrix();

	public :
		float m_Yaw;
		float m_Pitch;

		float m_MoveLeftRight;
		float m_MoveBackForward;

	private :
		CAMERABUFFER m_Info;

		XMVECTOR m_CamPosition;
		XMVECTOR m_CamTarget;
		XMVECTOR m_CamUp;

		XMVECTOR m_DefaultForward;
		XMVECTOR m_DefaultRight;

		XMVECTOR m_Forward;
		XMVECTOR m_Right;

		XMMATRIX m_ViewMatrix;
		XMMATRIX m_RotationMatrix;
};


#endif