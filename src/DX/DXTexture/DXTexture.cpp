#include "DXTexture.h"

DXTEXTURE::DXTEXTURE() {}

DXTEXTURE::DXTEXTURE( const DXTEXTURE* Other )
{
	*this = *Other;
}

DXTEXTURE::~DXTEXTURE() {}

bool DXTEXTURE::Init( ID3D11Device* Device, const char* FileDir1, const char* FileDir2 )
{
	m_ShaderResourceView = new ID3D11ShaderResourceView*[2];
	m_FileDir1 = FileDir1;
	m_FileDir2 = FileDir2;
	if ( !InitTextureFile( Device ) ) { return false; }
	if ( !InitSamplerState( Device ) ) { return false; }

	return true;
}

bool DXTEXTURE::InitTextureFile( ID3D11Device* Device )
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile( Device, m_FileDir1, NULL, NULL, &m_ShaderResourceView[0], NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Compile DDS Texture File %s ", m_FileDir1 );
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Compile DDS Texture File %s ", m_FileDir1 );
	}

	hr = D3DX11CreateShaderResourceViewFromFile( Device, m_FileDir2, NULL, NULL, &m_ShaderResourceView[1], NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Compile DDS Texture File %s ", m_FileDir2 );
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Compile DDS Texture File %s ", m_FileDir2 );
	}

	return true;
}

bool DXTEXTURE::InitSamplerState( ID3D11Device* Device )
{
	HRESULT hr;

	D3D11_SAMPLER_DESC samplerStateDesc;

	ZeroMemory( &samplerStateDesc, sizeof( samplerStateDesc ) );

	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerStateDesc.MinLOD = 0;
	samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = Device->CreateSamplerState( &samplerStateDesc, &m_SamplerState );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Sampler State");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Sampler State ");
	}

	return true;
}

void DXTEXTURE::Release()
{
	m_ShaderResourceView[0]->Release();
	m_ShaderResourceView[1]->Release();
	m_SamplerState->Release();

	InitPointer();
}

void DXTEXTURE::InitPointer()
{
	m_ShaderResourceView = nullptr;
	m_SamplerState = nullptr;
}

void DXTEXTURE::Render( ID3D11DeviceContext* DeviceContext )
{
	DeviceContext->PSSetShaderResources( 0, 2, m_ShaderResourceView );
	DeviceContext->PSSetSamplers( 0, 1, &m_SamplerState );
}