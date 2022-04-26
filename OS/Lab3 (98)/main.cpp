#include <iostream>
#include "threadParams.h"
#include "windows.h"
#include "errors.h"

HANDLE* hThreads;
HANDLE hStartAllEvent;
HANDLE* hDeadlocks;
HANDLE** hAnswers;

CRITICAL_SECTION csMarkElement;

DWORD WINAPI markerThread(void* args)
{
    threadParams* params = static_cast<threadParams*>(args);
    int* arr = params->arr;
    int n = params->n;
    int id = params->id;
    int nMarkedElements = 0;
    int num;
    srand(time(0));

    WaitForSingleObject(hStartAllEvent, INFINITE);

    while (true)
    {
        num = rand() % n;
        EnterCriticalSection(&csMarkElement);
        if (arr[num] != 0)
        {
            Sleep(5);
            arr[num] = id;
            Sleep(5);

            LeaveCriticalSection(&csMarkElement);
        }
        else
        {
            LeaveCriticalSection(&csMarkElement);

            std::cout << "[marker:" << id << "] "
            << "nMarkedElements = " << nMarkedElements
            << "; failed with index " << num << std::endl;

            SetEvent(hDeadlocks[id - 1]);
            DWORD answer = WaitForMultipleObjects(2, hAnswers[id - 1], FALSE, INFINITE);
            if (answer == WAIT_FAILED)
            {
                logErrorWin32("Wait function failed");
                return -1;
            }
            switch (answer - WAIT_OBJECT_0){
                case 0:
                    continue;
                case 1:
                    EnterCriticalSection(&csMarkElement);
                    for (int i = 0; i < n; i++)
                    {
                        if (arr[i] == id) arr[i] = 0;
                    }
                    LeaveCriticalSection(&csMarkElement);
                    ExitThread(1);
                    break;
                default:
                    ExitThread(2);
                    break;
            }
        }
    }
}

template<typename T>
void outArr(T* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << ' ';
    }
}

void cleanHandles(int totalThreads)
{
    CloseHandle(hStartAllEvent);
    for (int i = 0; i < totalThreads; i++){
        CloseHandle(hThreads[i]);
        CloseHandle(hDeadlocks[i]);
        CloseHandle(hAnswers[i][0]);
        CloseHandle(hAnswers[i][1]);
    }
}

int main()
{
    int totalThreads;
    int runningThreads;
    int nArr;
    int* arr;
    threadParams* params;

    std::cout << "Enter number of threads to create : ";
    std::cin >> totalThreads;
    std::cout << "Enter size of array : ";
    std::cin >> nArr;

    arr = new int[nArr];
    if (!arr)
    {
        logError("Couldn't allocate resources", "Out of memory!");
        return 0;
    }

    hThreads = new HANDLE[totalThreads];
    params = new threadParams[totalThreads];
    if (!hThreads || !params)
    {
        logError("Couldn't allocate resources", "Out of memory!");
    }

    hStartAllEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    for (int i = 1; i <= totalThreads; ++i)
    {
        params[i] = threadParams(i, arr, nArr);
        hThreads[i] = CreateThread(
                NULL,
                0,
                markerThread,
                static_cast<void*>(&params[i]),
                0,
                NULL
        );
        if (!hThreads[i]) logErrorWin32("Creating marker thread");
    }

    InitializeCriticalSection(&csMarkElement);

    hAnswers = new HANDLE*[totalThreads];
    for (int i = 0; i < totalThreads; i++)
    {
        hDeadlocks = new HANDLE[totalThreads];
        hDeadlocks[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

        hAnswers[i] = new HANDLE[2];
        hAnswers[i][0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        hAnswers[i][1] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    runningThreads = totalThreads;
    while (runningThreads)
    {
        break;
        // TODO: thread manipulation
    }

    DeleteCriticalSection(&csMarkElement);
    cleanHandles(totalThreads);
    for (int i = 0; i < totalThreads; i++)
    {
        delete[] hAnswers[i];
    }
    delete[] hAnswers;
    delete[] hThreads;
    delete[] hDeadlocks;
    delete[] params;
    delete[] arr;
    return 0;
}
