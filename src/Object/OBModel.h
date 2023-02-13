#ifndef __OBMODEL_H__
#define __OBMODEL_H__

#include "DXCommon.h"
#include "DXTexture.h"
#include "DXBuffer.h"

typedef enum MODELTYPE
{
	IMAGE, MESSAGE, STORY, MAP
}MODELTYPE;

class OBMODEL
{
	public :
		OBMODEL();
		OBMODEL( const OBMODEL* Other );
		OBMODEL( const char* Name );
		OBMODEL( DXTEXTURE* Texture );
		~OBMODEL();

	public :
		bool Init( ID3D11Device* Device, const char* FileDir );
		bool InitVertexBuffer( ID3D11Device* Device );
		bool InitIndexBuffer( ID3D11Device* Device );
		bool InitTexture( ID3D11Device* Device );

		void Release();
		void InitPointer();
		void Render( ID3D11DeviceContext* DeviceContext );

		void FrameRotation();
		void SetPosition( XMFLOAT3 Position );
		void SetRotation( XMFLOAT3 Rotation );
		void SetScale( XMFLOAT3 Scale );
		XMMATRIX Frame();

		bool SetTexture( ID3D11Device* Device, const char* FileDir );

	public :

		MODELTYPE m_Type;

		const char* m_Name;
		const char* m_TextureFileDir;

		DXTEXTURE* m_Texture;

		UINT m_NumIndex;

		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;

		XMFLOAT3 m_Position;
		XMFLOAT3 m_Rotation;
		XMFLOAT3 m_Scale;
};

#endif // __OBMODEL_H__