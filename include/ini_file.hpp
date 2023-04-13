#include <mini/ini.h>
#include <string>

class INIFile final
{
private:
	const std::string FilePath{};

public:
	INIFile() = delete;
	explicit INIFile(const std::string& filePath);

	void Write(const std::string& category, const std::string& key, const std::string& value);
	std::string Read(const std::string& category, const std::string& key);
};
