#include "ECS.h"

Entity::Entity(UID uid): uid(uid) 
{ 

}

void Entity::Update()
{
	for (auto& c : components)
	{
		c->Update();
	}
}

void Entity::Render()
{
	for (auto& c : components)
	{
		c->Render();
	}
}

void Entity::ClearTags()
{
	tags.clear();
}

void Entity::AddTag(Tag tag)
{
	tags.emplace_back(tag);
}

bool Entity::HasTag(Tag tag)
{
	for (auto& _tag : tags) 
	{ 
		if (_tag == tag) 
		{ 
			return true; 
		} 
	} 
	return false;
}

void Entity::RemoveTag(Tag tag)
{
	tags.erase(std::remove_if(tags.begin(), tags.end(), [&](const Tag _tag)-> bool { return _tag == tag; }), tags.end());
}

void Manager::Update()
{
	for (auto& e : entities)
	{
		e->Update();
	}
}

void Manager::Render()
{
	for (int i = 0; i < 4; i++)
	{
		for (auto& uid : renderLayers[(RenderLayer)i])
		{
			entities[uid]->Render();
		}
	}
}

void Manager::Refresh()
{
	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<Entity>& mEntity)
		{
			return !mEntity->isActive();
		}),
		std::end(entities));
}

std::vector<int> Manager::FindEntitiesWithTag(Tag tag)
{
	std::vector<int> entitiesWithTag;

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->HasTag(tag))
		{
			entitiesWithTag.emplace_back(i);
		}
	}

	return entitiesWithTag;
}

Entity& Manager::AddEntity(RenderLayer renderLayer)
{
	Entity* e = new Entity(nextUID);
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));
	renderLayers[renderLayer].emplace_back(nextUID);
	nextUID++;
	return *e;
}
