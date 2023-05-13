#include "FileHandler.h"

FileHandler::FileHandler()
{

}

FileHandler::~FileHandler()
{

}

void FileHandler::Init()
{
    Log::Info(" Init - File Handler ");
}

void FileHandler::Destroy()
{
    Log::Info(" Destroy - File Handler ");
}

FileHandler& FileHandler::GetHandle() { return m_FileHandler; }

FileHandler FileHandler::m_FileHandler;
