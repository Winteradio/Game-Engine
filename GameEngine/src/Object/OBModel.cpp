#include "OBModel.h"

OBMODEL::OBMODEL()
{
	InitPointer();
	m_Position = { 0.0f, 0.0f, 0.0f };
	m_Rotation = { 0.0f, 0.0f, 0.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };
}

OBMODEL::OBMODEL( const OBMODEL* Other )
{
	*this = *Other;
}

OBMODEL::OBMODEL( const char* Name )
{
	m_Name = Name;
}

OBMODEL::OBMODEL( DXTEXTURE* Texture )
{
	m_Texture = Texture;
}

OBMODEL::~OBMODEL() {}

bool OBMODEL::Init( ID3D11Device* Device, const char* FileDir1, const char* FileDir2 )
{
	if ( !InitVertexBuffer( Device ) ) { return false; }
	if ( !InitIndexBuffer( Device ) ) { return false; }
	if ( !InitTexture( Device, FileDir1, FileDir2 ) ) { return false; }

	return true;
}

bool OBMODEL::InitVertexBuffer( ID3D11Device* Device )
{
	// VERTEX
	VERTEX VertexData[] =
	{
	  // Front Face
	    VERTEX(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
	    VERTEX(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
	    VERTEX( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
	    VERTEX( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

	    // Back Face
	    VERTEX(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
	    VERTEX( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
	    VERTEX( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
	    VERTEX(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

	    // Top Face
	    VERTEX(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
	    VERTEX(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
	    VERTEX( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
	    VERTEX( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

	    // Bottom Face
	    VERTEX(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
	    VERTEX( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
	    VERTEX( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
	    VERTEX(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

	    // Left Face
	    VERTEX(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
	    VERTEX(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
	    VERTEX(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
	    VERTEX(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

	    // Right Face
	    VERTEX( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
	    VERTEX( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
	    VERTEX( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
	    VERTEX( 1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};


	HRESULT hr;

	// Describe Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory( &vertexBufferDesc, sizeof( vertexBufferDesc ) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( VERTEX ) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Describe Vertex Buffer Data
	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory( &vertexBufferData, sizeof( vertexBufferData ) );

	vertexBufferData.pSysMem = VertexData;

	hr = Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &m_VertexBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Vertex Buffer ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create Vertex Buffer ");
	}

	return true;
}

bool OBMODEL::InitIndexBuffer( ID3D11Device* Device )
{
	// INDEX
	m_NumIndex = 36;
	UINT IndexData[] =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	HRESULT hr;

	// Describe the Index buffer
	D3D11_BUFFER_DESC indexBufferDesc;

	ZeroMemory( &indexBufferDesc, sizeof( indexBufferDesc ) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( UINT ) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;

	indexBufferData.pSysMem = IndexData;

	hr = Device->CreateBuffer( &indexBufferDesc, &indexBufferData, &m_IndexBuffer );
	if ( FAILED( hr ) )
	{
		LOG_ERROR(" Failed - Create Index Buffer ");
	}
	else
	{
		LOG_INFO(" Successed - Create Index Buffer ");
	}

	return true;
}

bool OBMODEL::InitTexture( ID3D11Device* Device, const char* FileDir1, const char* FileDir2 )
{
	m_Texture = new DXTEXTURE;

	if ( !m_Texture )
	{
		LOG_ERROR(" Failed - Create DXTEXTURE ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Create DXTEXTURE ");
	}

	if ( !m_Texture->Init( Device, FileDir1, FileDir2 ) )
	{
		LOG_ERROR(" Failed - Init DXTEXTURE ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DXTEXTURE ");
	}

	return true;
}

void OBMODEL::Release()
{
	LOG_INFO(" Release - OBMODEL ");
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	InitPointer();
}

void OBMODEL::InitPointer()
{
	m_Name = nullptr;

	m_Texture = nullptr;
	m_VertexBuffer = nullptr;
	m_IndexBuffer = nullptr;
}

void OBMODEL::Render( ID3D11DeviceContext* DeviceContext )
{
	// Set the Vertex Buffer;
	UINT stride = sizeof( VERTEX );
	UINT offset = 0;
	DeviceContext->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	DeviceContext->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	m_Texture->Render( DeviceContext );

	DeviceContext->DrawIndexed( m_NumIndex, 0, 0 );
}

bool OBMODEL::SetTexture( ID3D11Device* Device, const char* FileDir1, const char* FileDir2 )
{
	m_Texture->Release();

	if ( !InitTexture( Device, FileDir1, FileDir2 ) )
	{
		LOG_ERROR(" Failed - Init Texture ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Texture ");
	}

	return true;
}

void OBMODEL::FrameRotation()
{
	m_Rotation.y += 0.0001f;
	if ( m_Rotation.y >= XM_2PI )
	{
		m_Rotation.y = 0.0f;
	}
}

XMMATRIX OBMODEL::Frame()
{
	XMMATRIX Rotation, Translation, Scaling, World;

	World = XMMatrixIdentity();

	XMVECTOR RotX, RotY, RotZ;
	RotX = XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f );
	RotY = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	RotZ = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );

	Rotation = XMMatrixRotationAxis( RotY, m_Rotation.y );

	Translation = XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );

	Scaling = XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z );

	World = Rotation * Translation * Scaling;

	return World;
}

void OBMODEL::SetPosition( XMFLOAT3 Position )
{
	m_Position = Position;
}

void OBMODEL::SetRotation( XMFLOAT3 Rotation )
{
	m_Rotation = Rotation;
}

void OBMODEL::SetScale( XMFLOAT3 Scale )
{
	m_Scale = Scale;
}