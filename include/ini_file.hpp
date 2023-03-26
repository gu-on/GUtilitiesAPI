#include <managed_object.hpp>
#include <mini/ini.h>
#include <string>

class INIFile final : public ManagedObject
{
private:
	const std::string FilePath{};
	std::string readCache{};

public:
	INIFile() = delete;
	explicit INIFile(const std::string& filePath);

	void Write(const std::string& category, const std::string& key, const std::string& value);
	const char* Read(const std::string& category, const std::string& key);
};
