#ifndef __DXENGINE_H__
#define __DXENGINE_H__

#include "DXCommon.h"

#include "DXD3D.h"
#include "DXCamera.h"
#include "DXIMGUI.h"
#include "DXTexture.h"
#include "OBShader.h"
#include "OBModel.h"

// Variables - Globals
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 1.0f;

class DXENGINE
{
	// Functions - Constructor and Destructor
	public :

		DXENGINE();
		DXENGINE( const DXENGINE* Other );
		~DXENGINE();

	// Functions - Basic Initialization and Run Frame & End
	public :

		bool Init( int Width, int Height, HWND hWnd );

		bool Frame( int FPS, int CPU, float Time, int mouseX, int mouseY );
		void Release();

	// Functions - Divide Init Functions
	private :

		bool InitDXD3D( int Width, int Height, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR );
		bool InitDXCAMERA();
		bool InitDXIMGUI( HWND hWnd );
		bool InitShader( OBSHADER*& Shader, SHADERTYPE Type, const char* FileDir );
		bool InitModel();
		void InitRenderState();
		void InitPointer();

		void InitFileDIR();

	// Functions - Render for each frames
	private :

		bool Render();

	// Variables - DXD3D Object
	private :

		DXD3D* m_DXD3D;
		DXCAMERA* m_DXCAMERA;
		DXIMGUI* m_DXIMGUI;

		OBSHADER* m_VertexShader;
		OBSHADER* m_PixelShader;
		OBMODEL* m_Model;

		RENDERSTATE* m_RenderState;

		const char* m_VertexShaderDir;
		const char* m_PixelShaderDir;
		const char* m_ShaderDir;
		const char* m_ImageFileDir1;
		const char* m_ImageFileDir2;
};

#endif __DXENGINE_H__