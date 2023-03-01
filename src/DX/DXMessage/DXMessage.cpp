#include "DXMessage.h"

DXMESSAGE::DXMESSAGE()
{
	InitPointer();
}

DXMESSAGE::DXMESSAGE( const DXMESSAGE& Other )
{
	*this = Other;
}

DXMESSAGE::~DXMESSAGE()
{

}

bool DXMESSAGE::Init()
{

	return true;
}

void DXMESSAGE::Release()
{

}

void DXMESSAGE::InitPointer()
{

}