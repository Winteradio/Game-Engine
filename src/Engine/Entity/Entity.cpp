#include "Entity.h"

Entity::Entity() {}
Entity::Entity( std::string Name ) : m_Name( Name ) {}
Entity::Entity( MyUUID ID ) : m_ID( ID ) {}
Entity::Entity( MyUUID ID, std::string Name ) : m_ID( ID ), m_Name( Name ) {}
Entity::~Entity() {}

void Entity::SetID( MyUUID ID ) { m_ID = ID; }
void Entity::SetName( std::string Name ) { m_Name = Name; }

MyUUID& Entity::GetID() { return m_ID; }
std::string& Entity::GetName() { return m_Name; }
Entity::Data& Entity::GetData() { return m_Data; }

void Entity::Destroy() { m_Data.clear(); }