#pragma once

#include <memory>
#include <vector>

class ManagedObject
{
public:
	ManagedObject();
	virtual ~ManagedObject() = 0;

	bool ShouldClose();
};

class ObjectManager
{
private:
	static inline std::vector<std::unique_ptr<ManagedObject>> objects{};

public:
	void Register();
	void Deregister();
	ManagedObject* Add(std::unique_ptr<ManagedObject> object);
	ManagedObject* Get(ManagedObject* id);
	static void Clear();
};