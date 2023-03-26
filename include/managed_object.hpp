#pragma once

#include <memory>
#include <vector>

class ManagedObject
{
private:
	/* data */
public:
	bool ShouldClose()
	{
		return true;
	}
	ManagedObject(/* args */);
	virtual ~ManagedObject() = 0;
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