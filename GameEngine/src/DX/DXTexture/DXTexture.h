#ifndef __DXTEXTURE_H__
#define __DXTEXTURE_H__

#include "DXCommon.h"

class DXTEXTURE
{
	public :
		DXTEXTURE();
		DXTEXTURE( const DXTEXTURE* Other );
		~DXTEXTURE();

	public :
		bool Init( ID3D11Device* Device, const char* FileDir1, const char* FileDir2 );
		bool InitTextureFile( ID3D11Device* Device );
		bool InitSamplerState( ID3D11Device* Device );

		void Release();
		void InitPointer();

		void Render( ID3D11DeviceContext* DeviceConText );

	private :
		ID3D11ShaderResourceView** m_ShaderResourceView;
		ID3D11SamplerState* m_SamplerState;

		const char* m_FileDir1;
		const char* m_FileDir2;
};

#endif