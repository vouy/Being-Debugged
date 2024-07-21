#include <windows.h>
#include <iostream>
#include <thread>
#include <winternl.h>

void CheckQueryInformation()
{
    while (true)
    {
        typedef NTSTATUS(__stdcall* CheckQueryInformationProcess)(_In_ HANDLE, _In_  unsigned int, _Out_ PVOID, _In_ ULONG, _Out_ PULONG);
        HANDLE Process = INVALID_HANDLE_VALUE;
        PROCESS_BASIC_INFORMATION Info = { 0 };
        ULONG Length = 0;
        HMODULE Library = LoadLibraryW(L"ntdll.dll");
        CheckQueryInformationProcess NtQueryInformationProcess = NULL;
        NtQueryInformationProcess = (CheckQueryInformationProcess)GetProcAddress(Library, "NtQueryInformationProcess");
        Process = GetCurrentProcess();
        NTSTATUS Status = NtQueryInformationProcess(Process, ProcessBasicInformation, &Info, sizeof(Info), &Length);
        if (NT_SUCCESS(Status))
        {
            PPEB Peb = Info.PebBaseAddress;
            if (Peb)
            {
                if (Peb->BeingDebugged)
                {
                    exit(0);
                }
            }
        }
    }
}

int main()
{
    std::thread(CheckQueryInformation).detach();
	std::cin.get();
}