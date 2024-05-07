#include "../imports.h"
#include "../globals.h"

typedef BOOL(WINAPI* tGetThreadContext)(HANDLE hThread,LPCONTEXT lpContext);
tGetThreadContext pGetThreadContext = nullptr;
bool WINAPI hookedGetThreadContext(HANDLE hThread, LPCONTEXT lpContext) {
    BOOL result = (*pGetThreadContext)(hThread, lpContext);
    if (lpContext) {
        lpContext->ContextFlags &= ~0x7F;
        lpContext->Dr0 = 0;
        lpContext->Dr1 = 0;
        lpContext->Dr2 = 0;
        lpContext->Dr3 = 0;
        lpContext->Dr6 = 0;
        lpContext->Dr7 = 0;
    }
    return pGetThreadContext(hThread, lpContext);
}

/* WriteProcessMemory */
typedef BOOL(WINAPI* tWriteProcessMemory)(HANDLE  hProcess, LPVOID  lpBaseAddress, LPCVOID lpBuffer, SIZE_T  nSize, SIZE_T* lpNumberOfBytesWritten);
tWriteProcessMemory pWriteProcessMemory = nullptr; // original function pointer after hook
bool WINAPI hookedWriteProcessMemory(HANDLE  hProcess, LPVOID  lpBaseAddress, LPCVOID lpBuffer, SIZE_T  nSize, SIZE_T* lpNumberOfBytesWritten) {

    for (int i = 1; i <= 1000; ++i) {
        std::string path = globals::dumpPath + xorstr_("writeprocessmemory_buffer.bin.") + std::to_string(i);
        if (!fileExists(path)) {
            std::ofstream outputFile(path, std::ios::binary | std::ios::app);
            if (outputFile.is_open()) {
                outputFile.write(reinterpret_cast<const char*>(lpBuffer), nSize);
                outputFile.close();
            }
            std::cout << xorstr_(" [WriteProcessMemory-Dumped] ") << path << std::endl;
            break;
        }
    }
    return pWriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

/* ReadProcessMemory */
typedef BOOL(WINAPI* tReadProcessMemory)(HANDLE  hProcess, LPCVOID lpBaseAddress, LPVOID  lpBuffer, SIZE_T  nSize, SIZE_T* lpNumberOfBytesRead);
tReadProcessMemory pReadProcessMemory = nullptr;
bool WINAPI hookedReadProcessMemory(HANDLE  hProcess, LPCVOID lpBaseAddress, LPVOID  lpBuffer, SIZE_T  nSize, SIZE_T* lpNumberOfBytesRead) {
    LPVOID tlpBuffer = lpBuffer;
    SIZE_T tnSize = nSize;
    SIZE_T* tlpNumberOfBytesRead = lpNumberOfBytesRead;
    LPCVOID tlpBaseAddress = lpBaseAddress;
    HANDLE thProcess = hProcess;
    ReadProcessMemory(thProcess, tlpBaseAddress, tlpBuffer, tnSize, tlpNumberOfBytesRead);

    for (int i = 1; i <= 1000; ++i) {
        std::string path = globals::dumpPath + xorstr_("readprocessmemory_buffer.bin.") + std::to_string(i);
        if (!fileExists(path)) {
            std::ofstream outputFile(path, std::ios::binary | std::ios::app);
            if (outputFile.is_open()) {
                outputFile.write(reinterpret_cast<const char*>(tlpBuffer), tnSize);
                outputFile.close();
            }
            std::cout << xorstr_(" [ReadProcessMemory-Dumped] ") << path << std::endl;
            break;
        }
    }
    return pReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

typedef BOOL(WINAPI* tDeleteFileW)(LPCWSTR lpFileName);
tDeleteFileW pDeleteFileW;
bool WINAPI hookedDeleteFileW(LPCWSTR lpFileName) {
    for (int i = 1; i <= 1000; ++i) {
        std::wstring srcPath = globals::dumpPathW + xorstr_(L"DeleteFileW.bin.") + std::to_wstring(i);
        if (!std::filesystem::exists(srcPath)) {
            CopyFileW(lpFileName, srcPath.c_str(), FALSE);
            std::wcout << xorstr_("[DeleteFileW] ") << lpFileName << " -> " << srcPath << std::endl;
            break;
        }
    }
    return pDeleteFileW(lpFileName);
}

typedef BOOL(WINAPI* tDeleteFileA)(LPCSTR lpFileName);
tDeleteFileA pDeleteFileA;
bool WINAPI hookedDeleteFileA(LPCSTR lpFileName) {
    for (int i = 1; i <= 1000; ++i) {
        std::string srcPath = globals::dumpPath + xorstr_("DeleteFileA.bin.") + std::to_string(i);
        if (!std::filesystem::exists(srcPath)) {
            CopyFileA(lpFileName, srcPath.c_str(), FALSE);
            std::cout << xorstr_("[DeleteFileA] ") << lpFileName << " -> " << srcPath << std::endl;
            break;
        }
    }
    return pDeleteFileA(lpFileName);
}

typedef NTSTATUS(NTAPI* tNtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
tNtRaiseHardError pNtRaiseHardError;
NTSTATUS hookedNtRaiseHardError(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response) {
    return NULL;
}