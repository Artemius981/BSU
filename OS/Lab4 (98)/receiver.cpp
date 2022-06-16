#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "windows.h"
#include "errors.h"
#include "constants.h"

HANDLE hFileMutex;
HANDLE hEmptySpaceSemaphore;
HANDLE hMsgsToHandleSemaphore;

std::vector<HANDLE> senders;
std::vector<HANDLE> sendersReadyEvents;

HANDLE createProcess(int processID, std::string& binaryFileName)
{
    std::ostringstream args;
    args << processID << ' ' << binaryFileName;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    bool createOk = CreateProcess( senderExeName.c_str(),
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

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

}

bool prepareWin32(int numberOfSenders, int msgsCapacity, std::string& binaryFileName)
{
    hFileMutex = CreateMutex(NULL, FALSE, fileMutexName.c_str());

    hEmptySpaceSemaphore = CreateSemaphore(NULL, msgsCapacity, msgsCapacity, emptySpaceSemName.c_str());
    hMsgsToHandleSemaphore = CreateSemaphore(NULL, 0, msgsCapacity, msgsToHandleSemName.c_str());

    for (int i = 0; i < numberOfSenders; ++i)
    {
        senders[i] = createProcess(i, binaryFileName);
        sendersReadyEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
}

void cleanWin32(int numberOfSenders)
{
    for (int i = 0; i < numberOfSenders; ++i)
    {
        CloseHandle(sendersReadyEvents[i]);
        CloseHandle(senders[i]);
    }

    CloseHandle(hMsgsToHandleSemaphore);
    CloseHandle(hEmptySpaceSemaphore);
    CloseHandle(hFileMutex);
}

void receiverFunc(const std::string& binaryFileName)
{

}

int main(int argc, char *argv[])
{
    std::string binaryFileName;
    int numberOfSenders;
    int msgsCapacity;

    std::cout << "Enter binary file's name: ";
    std::cin >> binaryFileName;
    std::cout << "Enter number of senders: ";
    std::cin >> numberOfSenders;
    prepareWin32(numberOfSenders, msgsCapacity, binaryFileName);

    cleanWin32(numberOfSenders);
    return 0;
}
