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

void Entity::SetActiveStatus(bool activeStatus)
{
	active = activeStatus;
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

void Manager::Update()
{
	for (std::map<UID, std::unique_ptr<Entity>>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if (it->second->isActive()) 
		{
			it->second->Update();
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

	for (std::map<UID, std::unique_ptr<Entity>>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if (it->second->HasTag(tag))
		{
			entitiesWithTag.emplace_back(it->second->GetUID());
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
