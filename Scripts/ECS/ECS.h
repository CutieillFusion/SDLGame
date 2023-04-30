#pragma once

#include "..\Globals.h"

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID GetComponentTypeID() 
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID GetComponentTypeID() noexcept 
{
	static ComponentID typeID = GetComponentTypeID();
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
	std::vector<Entity*> children;

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

	void SetChild(Entity* child);
	bool HasChildren();
	std::vector<Entity*> GetChildren();

	template <typename T> bool HasComponent() const
	{
		return componentBitSet[GetComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[GetComponentTypeID<T>()] = c;
		componentBitSet[GetComponentTypeID<T>()] = true;

		c->Initialize();

		return *c;
	}

	template <typename T> T& GetComponent() const
	{
		auto ptr(componentArray[GetComponentTypeID<T>()]);
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