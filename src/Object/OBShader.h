#ifndef __OBSHADER_H__
#define __OBSHADER_H__

#include "DXCommon.h"
#include "DXBuffer.h"

typedef enum SHADERTYPE
{
	Vertex, Pixel, NullShader
}SHADERTYPE;

class OBSHADER
{
	public :
		OBSHADER();
		OBSHADER( const OBSHADER& Other );
		OBSHADER( const char* FileDIR );
		~OBSHADER();

	public :

		bool Init( ID3D11Device* Device, SHADERTYPE Type, const char* FileDir );
		bool InitShader( ID3D11Device* Device );
		bool InitLayout( ID3D11Device* Device );
		bool InitMatrixBuffer( ID3D11Device* Device );
		bool InitLightBuffer( ID3D11Device* Device );
		bool InitCameraBuffer( ID3D11Device* Device );
		void InitPointer();
		void Release();

		void Frame( XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, CAMERABUFFER Camera );
		void Frame();
		void Render( ID3D11DeviceContext* DeviceCOntext );
		void ShaderErrorMessage( ID3D10Blob* ErrorMessage );

		SHADERTYPE m_Type;

		MATRIXBUFFER m_ShaderMatrix;
		LIGHTBUFFER m_ShaderLight;
		CAMERABUFFER m_ShaderCamera;

		const char* m_FileDir;

		ID3D10Blob* m_ShaderBuffer;
		ID3D11Buffer* m_ShaderMatrixBuffer;
		ID3D11Buffer* m_ShaderCameraBuffer;
		ID3D11Buffer* m_ShaderLightBuffer;

		ID3D11VertexShader* m_VertexShader;
		ID3D11PixelShader* m_PixelShader;

		ID3D11InputLayout* m_Layout;
};

#endif // __OBSHADER_H__