#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <winternl.h>
#include <errhandlingapi.h>
#include "externals/minhook/MinHook.h"
#include "utils/xorstr.hpp"

namespace globals {
	std::string dumpPath = xorstr_("D:\\a-debuggers\\Etheral\\Mapper\\Log\\");
	std::wstring dumpPathW = xorstr_(L"D:\\a-debuggers\\Etheral\\Mapper\\Log\\");
	bool AllocateConsole = true; /* If a console in the program is already allocated or program keeps crashing on injection try switching to False. */
}