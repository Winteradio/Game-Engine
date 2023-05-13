#include "Entity.h"

Entity::Entity() {}
Entity::~Entity() {}

MyUUID& Entity::GetID() { return GetComponent<IDComponent>()->ID; }
std::string& Entity::GetName() { return GetComponent<TagComponent>()->Tag; }
Entity::Data& Entity::GetData() { return m_Data; }

void Entity::Destroy()
{
	m_Data.clear();
}