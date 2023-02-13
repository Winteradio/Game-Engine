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

	private :
		CAMERABUFFER m_Info;

		XMMATRIX m_ViewMatrix;
};


#endif