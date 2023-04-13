#pragma once

void GU_ConfigFileWrite(const char* filePath, const char* category, const char* key, const char* value);
const char* GU_ConfigFileRead(const char* filePath, const char* category, const char* key);