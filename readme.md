# Hook Based Dumper (EzDumper)
An injectable DLL that hooks Windows API Functions inside a program to dump the variables passed in and the returned variables such as DeleteFileW/DeleteFileA.

# Hooks
- ReadProcessMemory (Dumps the returned variable it is trying to read)
- WriteProcessMemory (Dumps the buffer it is trying to write)
- DeleteFileA & DeleteFileW (Dumps the file contents before deleting it)

# Security
I will add more security features in the future to prevent detection.
- Kernel32 GetThreadContext HW Breakpoint Detection (Prevent HW Breakpoint Detect)
- Kernel32 NtRaiseHardError (Prevent Forced BSOD through Kernel Calling)
- _RtlCreateUserThread Hook and RIP/EIP Spoofing to bypass thread tracing of DLL.

# Project
This is just a Pre-Dump Project that I'll develop on and re-release under a new repo with better features and more hooks related to file dumping and etc.
