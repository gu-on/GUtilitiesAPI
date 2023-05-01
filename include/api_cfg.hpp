// write file
bool GU_ConfigFileWrite(const char* filePath, const char* category, const char* key, const char* value);
// read file
bool GU_ConfigFileRead(const char* filePath, const char* category, const char* key, char* valueOut, int valueOut_sz);
// Test function that prints \"hello\"
void GU_PrintMessage();