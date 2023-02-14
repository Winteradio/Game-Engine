#include "OBShader.h"

OBSHADER::OBSHADER()
{
	InitPointer();
};

OBSHADER::OBSHADER( const OBSHADER& Other )
{
	this->m_FileDir = Other.m_FileDir;
}

OBSHADER::OBSHADER( const char* FileDir )
{
	m_FileDir = FileDir;
}

OBSHADER::~OBSHADER(){}

bool OBSHADER::Init( ID3D11Device* Device, SHADERTYPE Type, const char* FileDir )
{
	m_Type = Type;
	m_FileDir = FileDir;

	if ( !InitShader( Device ) ) { return false; }

	switch( Type )
	{
	case Vertex :
		if ( !InitLayout( Device ) ) { return false; }
		if ( !InitMatrixBuffer( Device ) ) { return false; }
		if ( !InitCameraBuffer( Device ) ) { return false; }
		break;

	case Pixel :
		if ( !InitLightBuffer( Device ) ) { return false; }
		break;
	}

	return true;
}

bool OBSHADER::InitShader( ID3D11Device* Device )
{
	const char* ShaderType;
	const char* ShaderVersion;

	ID3D10Blob* ErrorMessage = nullptr;

	HRESULT hr;

	switch( m_Type )
	{
		case Vertex :
		{
			ShaderType = "VS";
			ShaderVersion = "vs_4_0";

			hr = D3DX11CompileFromFile( m_FileDir, 0, 0, ShaderType, ShaderVersion, 0, 0, 0, &m_ShaderBuffer, &ErrorMessage, 0 );
			if ( FAILED( hr ) )
			{
				LOG_ERROR(" Failed - Compile Vertex Shader File ");
				ShaderErrorMessage( ErrorMessage );
				return false;
			}
			else
			{
				LOG_INFO(" Successed - Compile Vertex Shader File ");
			}

			hr = Device->CreateVertexShader( m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), NULL, &m_VertexShader );
			if ( FAILED( hr ) )
			{
				LOG_ERROR(" Failed - Create Vertex Shader ");
				return false;
			}
			else
			{
				LOG_INFO(" Successed - Create Vertex Shader ");
			}

			break;
		}

		case Pixel :
		{
			ShaderType = "PS";
			ShaderVersion = "ps_4_0";

			hr = D3DX11CompileFromFile( m_FileDir, 0, 0, ShaderType, ShaderVersion, 0, 0, 0, &m_ShaderBuffer, &ErrorMessage, 0 );
			if ( FAILED( hr ) )
			{
				LOG_ERROR(" Failed - Compile Pixel Shader File ");
				ShaderErrorMessage( ErrorMessage );
				return false;
			}
			else
			{
				LOG_INFO(" Successed - Compile Pixel Shader File ");
			}

			hr = Device->CreatePixelShader( m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), NULL, &m_PixelShader );
			if ( FAILED( hr ) )
			{
				LOG_ERROR(" Failed - Create Pixel Shader ");
				return false;
			}
			else
			{
				LOG_INFO(" Successed - Create Pixel Shader ");
			}
			break;
		}

		default :
			return false;
	}

	return true;
}

bool OBSHADER::InitLayout( ID3D11Device* Device  )
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC Layout[] =
	{
	    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE( Layout );

	hr = Device->CreateInputLayout( Layout, numElements, m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), &m_Layout );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Layout ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Layout ");
	}

	return true;
}

bool OBSHADER::InitMatrixBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	D3D11_BUFFER_DESC matrixBufferDesc;

	ZeroMemory( &matrixBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	matrixBufferDesc.ByteWidth = sizeof( MATRIXBUFFER );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = 0;
	matrixBufferDesc.MiscFlags = 0;

	hr = Device->CreateBuffer( &matrixBufferDesc, NULL, &m_ShaderMatrixBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Shader Matrix Constant Buffer ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Shader Matrix Constant Buffer ");
	}

	return true;
}

bool OBSHADER::InitCameraBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	D3D11_BUFFER_DESC cameraBufferDesc;

	ZeroMemory( &cameraBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	cameraBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cameraBufferDesc.ByteWidth = sizeof( CAMERABUFFER );
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = 0;
	cameraBufferDesc.MiscFlags = 0;

	hr = Device->CreateBuffer( &cameraBufferDesc, NULL, &m_ShaderCameraBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Shader Camera Constant Buffer ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Shader Camera Constant Buffer ");
	}

	return true;
}

bool OBSHADER::InitLightBuffer( ID3D11Device* Device )
{
	HRESULT hr;

	D3D11_BUFFER_DESC lightBufferDesc;

	ZeroMemory( &lightBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	lightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	lightBufferDesc.ByteWidth = sizeof( LIGHTBUFFER );
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = 0;
	lightBufferDesc.MiscFlags = 0;

	hr = Device->CreateBuffer( &lightBufferDesc, NULL, &m_ShaderLightBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Shader Light Constant Buffer ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Shader Light Constant Buffer ");
	}

	return true;

}

void OBSHADER::InitPointer()
{
	m_Type = NullShader;
	m_ShaderBuffer = nullptr;
	m_VertexShader = nullptr;
	m_PixelShader = nullptr;
	m_Layout = nullptr;
}

void OBSHADER::Release()
{
	LOG_INFO(" Release - OBSHADER ");
	if ( m_ShaderBuffer != nullptr )
	{
		m_ShaderBuffer->Release();
	}

	if ( m_VertexShader != nullptr )
	{
		m_VertexShader->Release();
	}

	if ( m_PixelShader != nullptr )
	{
		m_PixelShader->Release();
	}

	if ( m_Layout != nullptr )
	{
		m_Layout->Release();
	}
	InitPointer();
}


void OBSHADER::Frame( XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, CAMERABUFFER Camera )
{
	// XMMatrixTranspose : 전치 행렬 만들어주기 필수 !!!
	m_ShaderMatrix.World = XMMatrixTranspose( worldMatrix ) ;
	m_ShaderMatrix.View = XMMatrixTranspose( viewMatrix );
	m_ShaderMatrix.Projection = XMMatrixTranspose( projectionMatrix );
	m_ShaderMatrix.LightPosition1 = XMFLOAT4( 0.0f, 0.0f, 10.0f, 0.0f );
	m_ShaderMatrix.LightPosition2 = XMFLOAT4( 10.0f, 0.0f, 0.0f, 0.0f );

	m_ShaderCamera.Position = Camera.Position;
}

void OBSHADER::Frame()
{
	m_ShaderLight.DiffuseColor = XMFLOAT4( 0.5f, 0.5f, 5.0f, 1.0f );
	m_ShaderLight.LightDirection = XMFLOAT3( 1.0f, 1.0f, -5.0f );
	m_ShaderLight.AmbientColor = XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f );
	m_ShaderLight.SpecularPower = 5.0f;
}

void OBSHADER::Render( ID3D11DeviceContext* DeviceContext )
{
	switch( m_Type )
	{
	case Vertex :
		DeviceContext->UpdateSubresource( m_ShaderMatrixBuffer, 0, NULL, &m_ShaderMatrix, 0, 0 );
		DeviceContext->UpdateSubresource( m_ShaderCameraBuffer, 0, NULL, &m_ShaderCamera, 0, 0 );
		DeviceContext->VSSetConstantBuffers( 0, 1, &m_ShaderCameraBuffer );
		DeviceContext->VSSetConstantBuffers( 0, 1, &m_ShaderMatrixBuffer );
		DeviceContext->VSSetShader( m_VertexShader, 0, 0 );
		DeviceContext->IASetInputLayout( m_Layout );
		break;

	case Pixel :
		DeviceContext->UpdateSubresource( m_ShaderLightBuffer, 0, NULL, &m_ShaderLight, 0, 0 );
		DeviceContext->PSSetConstantBuffers( 0, 1, &m_ShaderLightBuffer );
		DeviceContext->PSSetShader( m_PixelShader, 0, 0 );
		break;
	}

	DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void OBSHADER::ShaderErrorMessage( ID3D10Blob* ErrorMessage )
{
	LOG_ERROR("%s \n",(char*)( ErrorMessage->GetBufferPointer() ) ) ;


	ErrorMessage->Release();
	ErrorMessage = nullptr;
}