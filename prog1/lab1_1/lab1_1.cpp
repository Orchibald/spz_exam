#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

struct ThreadArgs {
    LPCWSTR filename;
    int lowercase_count;
};

DWORD WINAPI countLowercaseLetters(LPVOID lpParam) {
    struct ThreadArgs* threadArgs = (struct ThreadArgs*)lpParam;
    LPCWSTR filename = threadArgs->filename;
    int count = 0;

    HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open file (%lu).\n", GetLastError());
        return 1;
    }

    CHAR buffer[1024];
    DWORD bytesRead;

    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        for (DWORD i = 0; i < bytesRead; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                count++;
            }
        }
    }

    CloseHandle(hFile);

    threadArgs->lowercase_count = count;

    return 0;
}

int main() {
    LPCWSTR filename = L"C:\\Study\\302_2\\SPZ\\exam.txt";

    struct ThreadArgs threadArgs;
    threadArgs.filename = filename;
    threadArgs.lowercase_count = 0;

    HANDLE hThread;
    DWORD dwThreadId;

    hThread = CreateThread(
        NULL,
        0,
        countLowercaseLetters,
        &threadArgs,
        0,
        &dwThreadId);

    if (hThread == NULL) {
        fprintf(stderr, "Error creating thread (%lu).\n", GetLastError());
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    printf("\nNumber of lowercase letters in the file: %d\n", threadArgs.lowercase_count);

    printf("\nThread ID: %lu\n", dwThreadId);
    printf("Thread Handle: %p\n\n", hThread);
    printf("Main process ID: %lu\n", GetCurrentProcessId());
    printf("Main thread ID: %lu\n", GetCurrentThreadId());
    printf("Main process Handle: %p\n", GetCurrentProcess());
    printf("Main thread Handle: %p\n", GetCurrentThread());

    CloseHandle(hThread);

    getchar();
    return 0;
}
