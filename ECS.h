#pragma once

#include <map>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID() 
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept 
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity = nullptr;

	virtual void Initialize() { };
	virtual void Update() { };
	virtual void Render() { };

	virtual ~Component() { }
};

typedef std::uint32_t UID;
typedef std::string Tag;

class Entity
{
private:
	UID uid;
	std::vector<Tag> tags;
	bool active = true;
	bool destroy = false;
	std::vector<std::unique_ptr<Component>> components;
	Entity* parent = nullptr;

	ComponentArray componentArray{};
	ComponentBitSet componentBitSet;
public:
	Entity(UID uid);
	void Update();
	void Render();
	bool isActive() const { return active; }
	void SetActiveStatus(bool activeStatus);
	int GetUID() const { return uid; }
	
	//Tags Logic
	void ClearTags();
	void AddTag(Tag tag);
	bool HasTag(Tag tag);
	void RemoveTag(Tag tag);//This has not been tested nor will be 

	//Destroy Logic
	bool Destroyable();
	void Destroy();

	void SetParent(Entity* parent);
	bool HasParent();
	Entity* GetParent();

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->Initialize();

		return *c;
	}

	template <typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

enum RenderLayer
{
	LAYER_BACKGROUND,
	LAYER_FOREGROUND,
	LAYER_CHARACTER,
	LAYER_UI
};

class Manager
{
private:
	UID nextUID = 0;

public:
	std::map<UID, std::unique_ptr<Entity>> entities;
	std::map<RenderLayer, std::vector<UID>> renderLayers;

	void Update();
	void Render();
	void Refresh();

	std::vector<UID> FindEntitiesWithTag(Tag tag);

	Entity& AddEntity(RenderLayer renderLayer);
};