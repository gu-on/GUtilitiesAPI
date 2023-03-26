#include <managed_object.hpp>
#include <reaper_plugin_functions.h>

ManagedObject::ManagedObject(/* args */)
{
	ShowConsoleMsg("Created!");
}

ManagedObject::~ManagedObject()
{
	ShowConsoleMsg("Deleted!");
}

void ObjectManager::Register()
{
	plugin_register("timer", reinterpret_cast<void*>(&Clear));
}

ManagedObject* ObjectManager::Add(std::unique_ptr<ManagedObject> object)
{
	objects.push_back(std::move(object));
	return objects.back().get();
}

void ObjectManager::Deregister()
{
	plugin_register("-timer", reinterpret_cast<void*>(&Clear));
}

ManagedObject* ObjectManager::Get(ManagedObject* id)
{
	for (auto& object : objects)
	{
		auto* temp = object.get();
		if (temp == id)
		{
			return temp;
		}
	}
	return nullptr;
}

void ObjectManager::Clear()
{
	std::erase_if(objects, [](std::unique_ptr<ManagedObject> const& p) { return p->ShouldClose(); });
}
