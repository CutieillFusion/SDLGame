#include "ECS.h"
#include "Globals.h"

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

void Entity::SetActiveStatus(bool activeStatus)
{
	if (active == activeStatus) 
	{
		return;
	}

	active = activeStatus;
	if (HasChildren())
	{
		for (auto child : children)
		{
			child->SetActiveStatus(activeStatus);
		}
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

bool Entity::Destroyable()
{
	return destroy;
}

void Entity::Destroy()
{
	destroy = true;
}

void Entity::SetParent(Entity* parent)
{
	this->parent = parent;
	parent->SetChild(this);
}

bool Entity::HasParent()
{
	return (parent != nullptr);
}

Entity* Entity::GetParent()
{
	return parent;
}

void Entity::SetChild(Entity* child)
{
	children.emplace_back(child);
}

bool Entity::HasChildren()
{
	return (children.size() > 0);
}

std::vector<Entity*> Entity::GetChildren()
{
	return children;
}

void Manager::Update()
{
	for (auto& [key, entity] : entities)
	{
		if (entity->isActive()) 
		{
			entity->Update();
		}
	}
}

void Manager::Render()
{
	for (int i = 0; i < 4; i++)
	{
		for (auto& uid : renderLayers[(RenderLayer)i])
		{
			if(entities[uid]->isActive()) 
			{
				entities[uid]->Render();
			}
		}
	}
}

void Manager::Refresh()
{
	for (std::map<UID, std::unique_ptr<Entity>>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if(it->second->Destroyable()) 
		{
			auto uid = it->second->GetUID();
			entities.erase(it);
			for (int i = 0; i < 4; i++)
			{
				for (std::vector<UID>::iterator _it = renderLayers[(RenderLayer)i].begin();  _it < renderLayers[(RenderLayer)i].end(); _it++)
				{
					if (uid == *_it) 
					{
						renderLayers[(RenderLayer)i].erase(_it);
						return;
					}
				}
			}

		}
	}
}

std::vector<UID> Manager::FindEntitiesWithTag(Tag tag)
{
	std::vector<UID> entitiesWithTag;

	for (auto& [key, entity] : entities)
	{
		if (entity->HasTag(tag))
		{
			entitiesWithTag.emplace_back(entity->GetUID());
		}
	}

	return entitiesWithTag;
}

Entity& Manager::AddEntity(RenderLayer renderLayer)
{
	Entity* e = new Entity(nextUID);
	std::unique_ptr<Entity> uPtr{ e };
	entities[nextUID] = std::move(uPtr);
	renderLayers[renderLayer].emplace_back(nextUID);
	nextUID++;
	return *e;
}
