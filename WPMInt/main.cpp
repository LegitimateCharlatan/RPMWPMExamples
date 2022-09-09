#include <Windows.h>
#include <tlhelp32.h>

DWORD getProcessIdByName(LPCWSTR processName)
{
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry))
    {
        CloseHandle(snapshot);
        return 0;
    }

    while (Process32Next(snapshot, &entry))
    {
        if (lstrcmp(entry.szExeFile, processName) == 0)
        {
            return entry.th32ProcessID;
        }
    }

    CloseHandle(snapshot);

    return 0;
}

int main()
{
    DWORD processId = getProcessIdByName(L"TestInt.exe");
    HANDLE pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId);
    LPVOID offset = 0;
    int buffer = 7654321;
    SIZE_T size = sizeof(buffer);

    //VirtualProtect();
    WriteProcessMemory(pHandle, offset, &buffer, size, nullptr);
    //VirtualProtect();

    CloseHandle(pHandle);
}