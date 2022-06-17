#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "errors.h"
#include "employee.h"
#include "windows.h"

CRITICAL_SECTION csFileWrite;
CRITICAL_SECTION csBlockedFlags;

HANDLE hStartAllEvent;

std::vector<HANDLE> hReadyEvents;
std::vector<HANDLE> hServerThreads;
std::vector<bool> entryBlockedFlags;

void prepareBinaryFile(int employeesCount, std::string& binaryFileName)
{
    std::fstream fout(binaryFileName.c_str(), std::ios::binary | std::ios::out);
    std::ostringstream prefix;
    for (int i = 0; i < employeesCount; ++i)
    {
        Employee emp;
        prefix << '[' << i + 1 << "] ";
        std::cout << prefix.str() << "ID : ";
        std::cin >> emp.id;
        std::cout << prefix.str() << "Name : ";
        std::cin >> emp.name;
        std::cout << prefix.str() << "Working hours : ";
        std::cin >> emp.hours;

        fout << emp;

        prefix.str("");
        prefix.clear();
    }
}

bool createClient(int clientID)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    std::ostringstream args;
    args << clientID;

    bool createOk = CreateProcess( clientExeName.c_str(),
                                   &args.str()[0],
                                   NULL,
                                   NULL,
                                   FALSE,
                                   CREATE_NEW_CONSOLE,
                                   NULL,
                                   NULL,
                                   &si,
                                   &pi
    );
    if (!createOk)
    {
        logErrorWin32("CreateProcess failed!");
    }

    CloseHandle(pi.hThread);
    return createOk;
}

bool prepareSyncObjects(int clientsCount)
{
    InitializeCriticalSection(&csBlockedFlags);
    InitializeCriticalSection(&csFileWrite);
    hStartAllEvent = CreateEvent(NULL, TRUE, FALSE, startAllEventName.c_str());
    entryBlockedFlags = std::vector<bool>(clientsCount);
    if (hStartAllEvent == NULL)
    {
        logErrorWin32("CreateEvent failed!");
        return false;
    }
    return true;
}

bool prepareWin32(int clientsCount)
{
    bool syncObjPrepared = prepareSyncObjects(clientsCount);
    bool clientsPrepared = true;

    hReadyEvents = std::vector<HANDLE>(clientsCount);

    std::ostringstream handleName;
    for (int i = 0; i < clientsCount; ++i)
    {
        handleName << creatorName << "_ready_" << i;
        hReadyEvents[i] = CreateEvent(NULL, TRUE, FALSE, &handleName.str()[0]);
        clientsPrepared &= createClient(i);

        handleName.str("");
        handleName.clear();
    }
    return syncObjPrepared && clientsPrepared;
}

void cleanWin32(int clientsCount)
{
    for (int i = 0; i < clientsCount; ++i)
    {
        CloseHandle(hReadyEvents[i]);
    }

    CloseHandle(hStartAllEvent);
    DeleteCriticalSection(&csFileWrite);
    DeleteCriticalSection(&csBlockedFlags);
}

DWORD WINAPI processingThread(LPVOID params)
{

}

bool makeConnection(int clientsCount)
{
    hServerThreads = std::vector<HANDLE>(clientsCount);
    HANDLE hPipe;
    for (int i = 0; i < clientsCount; ++i)
    {
        hPipe = CreateNamedPipe(
                pipeName.c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE |
                PIPE_READMODE_MESSAGE |
                PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                0,
                0,
                0,
                NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            logErrorWin32("CreateNamedPipe failed!");
            return false;
        }

        bool isConnected = ConnectNamedPipe(hPipe, NULL) != 0 || (GetLastError() == ERROR_PIPE_CONNECTED);
        if (isConnected)
        {
            hServerThreads[i] = CreateThread(NULL, 0, processingThread, (LPVOID) hPipe, 0, NULL);
            if (hServerThreads[i] == NULL)
            {
                logErrorWin32("CreateThread for server failed!");
                return false;
            }
        }
        else
        {
            CloseHandle(hPipe);
        }
    }
    return true;
}

int main()
{
    int employeesCount;
    int clientsCount;
    std::string binaryFileName;

    std::cout << "Enter the number of employees : ";
    std::cin >> employeesCount;
    std::cout << "Enter the number of clients : ";
    std::cin >> clientsCount;
    std::cout << "Enter binary file's name : ";
    std::cin >> binaryFileName;

    consoleMessage("Creating binary file.");
    consoleMessage("Please enter all entries : ");
    prepareBinaryFile(employeesCount, binaryFileName);

    bool prepareWin32OK = prepareWin32(clientsCount);
    if (!prepareWin32OK)
    {
        consoleMessage("Something went wrong while preparing Win32 stuff!");
        return -1;
    }
    else
    {
        consoleMessage("Successfully prepared Win32 stuff.");
    }

    consoleMessage("Waiting for all clients to be ready...");
    WaitForMultipleObjects(clientsCount, hReadyEvents.data(), TRUE, INFINITE);
    consoleMessage("All clients are ready. Starting...");
    SetEvent(hStartAllEvent);

    bool connectionOK = makeConnection(clientsCount);
    if (!connectionOK)
    {
        consoleMessage("Something went wrong while establishing server-client connections!");
        return -1;
    }
    else
    {
        consoleMessage("All clients connected successfully.");
    }

    cleanWin32(clientsCount);
    return 0;
}