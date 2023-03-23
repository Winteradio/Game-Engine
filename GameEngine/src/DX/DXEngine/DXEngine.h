#ifndef __DXENGINE_H__
#define __DXENGINE_H__

#include "DXCommon.h"

#include "DXD3D.h"
#include "DXCamera.h"
#include "DXIMGUI.h"
#include "DXInput.h"
#include "DXTexture.h"
#include "DXWindow.h"
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

		bool Init( int PosX, int PosY, int Width, int Height, HWND hWnd );

		bool Frame( int FPS, int CPU, float Time, int mouseX, int mouseY );
		void Release();

	// Functions - Divide Init Functions
	private :

		bool InitDXWINDOW( int Width, int Height );
		bool InitDXD3D( int Width, int Height, HWND hWnd, float SCREEN_DEPTH, float SCREEN_NEAR );
		bool InitDXCAMERA();
		bool InitDXIMGUI( HWND hWnd );
		bool InitDXINPUT( HWND hWnd, int PosX, int PosY, int Width, int Height );
		bool InitShader( OBSHADER*& Shader, SHADERTYPE Type, const char* FileDir );
		bool InitModel();
		void InitRenderState();
		void InitPointer();

		void InitFileDIR();

	// Functions - Render for each frames
	private :

		bool Render();
		bool RenderEngine();
		bool RenderInGame();
		bool RenderGUI();

	// Variables - DXD3D Object
	private :

		DXD3D* m_DXD3D;
		DXCAMERA* m_DXEngineCamera;
		DXCAMERA* m_DXInGameCamera;
		DXIMGUI* m_DXIMGUI;
		DXINPUT* m_DXINPUT;

		OBSHADER* m_VertexShader;
		OBSHADER* m_PixelShader;
		OBMODEL* m_Model;

		DXRECTWINDOW* m_DXEngineWindow;
		DXRECTWINDOW* m_DXInGameWindow;
		DXRECTWINDOW* m_DXUserWindow;
		DXRECTWINDOW* m_DXInfoWindow;

		RENDERSTATE* m_RenderState;
		RENDERSTATE* m_RenderStateInGame;
		RENDERSTATE* m_RenderStateEngine;

		const char* m_VertexShaderDir;
		const char* m_PixelShaderDir;
		const char* m_ShaderDir;
		const char* m_ImageFileDir1;
		const char* m_ImageFileDir2;

		int Count = 1;
		int FrameCount = 40;
};

#endif __DXENGINE_H__