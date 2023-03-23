#include "api_creator.hpp"

#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>

#include <memory>
#include <vector>

class Item
{
public:
	Item()
	{
		ShowConsoleMsg("Created!\n");
	};
	~Item()
	{
		ShowConsoleMsg("Deleted!\n");
	};
};

std::vector<std::unique_ptr<Item>> list{};

void ClearList()
{
	static int counter{1};

	++counter;

	if (counter % 10 != 0)
		return;

	for (auto& item : list)
	{
		item.reset();
	}
}

extern "C"
{
	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t* rec)
	{
		if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0)
		{
			std::unique_ptr<Item> item = std::make_unique<Item>();
			list.push_back(std::move(item));
			rec->Register("timer", (void*)ClearList);
			APICreator::Register();
			return 1;
		}
		else
		{
			rec->Register("-timer", (void*)ClearList);
			return 0;
		}
	}
}
