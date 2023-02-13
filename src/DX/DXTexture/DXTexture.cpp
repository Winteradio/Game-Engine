#include "DXTexture.h"

DXTEXTURE::DXTEXTURE() {}

DXTEXTURE::DXTEXTURE( const DXTEXTURE* Other )
{
	*this = *Other;
}

DXTEXTURE::DXTEXTURE( const char* FileDir )
{
	m_FileDir = FileDir;
}

DXTEXTURE::~DXTEXTURE() {}

bool DXTEXTURE::Init( ID3D11Device* Device, const char* FileDir )
{
	m_FileDir = FileDir;
	if ( !InitTextureFile( Device ) ) { return false; }
	if ( !InitSamplerState( Device ) ) { return false; }

	return true;
}

bool DXTEXTURE::InitTextureFile( ID3D11Device* Device )
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile( Device, m_FileDir, NULL, NULL, &m_ShaderResourceView, NULL );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Compile DDS Texture File %s ", m_FileDir );
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Compile DDS Texture File %s ", m_FileDir );
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
	m_ShaderResourceView->Release();
	m_SamplerState->Release();

	InitPointer();
}

void DXTEXTURE::InitPointer()
{
	m_ShaderResourceView = nullptr;
	m_SamplerState = nullptr;

	m_FileDir = nullptr;
}

void DXTEXTURE::Render( ID3D11DeviceContext* DeviceContext )
{
	DeviceContext->PSSetShaderResources( 0, 1, &m_ShaderResourceView );
	DeviceContext->PSSetSamplers( 0, 1, &m_SamplerState );
}