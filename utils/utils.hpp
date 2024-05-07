#include "../imports.h"
#include "../globals.h"

bool fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}