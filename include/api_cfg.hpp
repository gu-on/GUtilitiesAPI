// write file
void GU_ConfigFileWrite(const char* filePath, const char* category, const char* key, const char* value);
// read file
const char* GU_ConfigFileRead(const char* filePath, const char* category, const char* key);
// example text
void GU_PrintMessage();