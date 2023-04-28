#include "Entity.h"

Entity::Entity() {}

Entity::Entity( std::string Name ) : m_Name( Name ) 
{}

Entity::Entity( std::string Name, MyUUID ID ) : m_Name( Name ), m_ID( ID ) 
{}

Entity::~Entity() {}

Entity::Data& Entity::GetData() { return m_Data; }

void Entity::Destroy()
{
	m_Data.clear();
}
void Entity::SetID( MyUUID ID ) { m_ID = ID; }
void Entity::SetName( std::string Name ) { m_Name = Name; }

std::string& Entity::GetID() { return m_ID.GetID(); }
std::string& Entity::GetName() { return m_Name; }