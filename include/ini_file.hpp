#include <filesystem>
#include <mini/ini.h>
#include <string>

class INIFile final
{
private:
	static constexpr bool PASS = true;
	static constexpr bool FAIL = false;

	std::string GetFormattedFileName(std::string fileName);
	const std::string FilePath{};

public:
	INIFile() = delete;
	explicit INIFile(const std::string& filePath);

	bool Write(const std::string& category, const std::string& key, const std::string& value);
	bool Read(const std::string& category, const std::string& key, std::string& value);
};
