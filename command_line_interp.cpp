#include <iostream>
#include <string>
#include <windows.h>

// Function to handle the 'dir' command (list directory contents)
DWORD WINAPI execute_dir(LPVOID lpParam) {
    WIN32_FIND_DATAA findFileData; // Use the ANSI version
    HANDLE hFind = FindFirstFileA("*.*", &findFileData); // Use FindFirstFileA

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "Error: Could not open the current directory.\n";
    } else {
        std::cout << "Directory contents:\n";
        do {
            std::cout << findFileData.cFileName << std::endl; // Use cFileName for ANSI strings
        } while (FindNextFileA(hFind, &findFileData) != 0);  // Use FindNextFileA
        
        FindClose(hFind);
    }
    return 0;
}

// Function to handle the 'help' command
DWORD WINAPI execute_help(LPVOID lpParam) {
    std::cout << "Supported commands:\n";
    std::cout << "dir  - List the contents of the current directory.\n";
    std::cout << "help - Display this help message.\n";
    std::cout << "exit - Exit the shell.\n";
    return 0;
}

DWORD WINAPI execute_vol(LPVOID lpParam) {
    system("vol"); 
}

DWORD WINAPI execute_path(LPVOID lpParam) {  // how do we use this one 
    char* systemPath = getenv("PATH");
    if (systemPath != NULL) {
        std::cout << "system Path:\n"<< systemPath << std::endl;
    } else {
        std::cout << "Unable to retrieve System Path\n";
    }
}

void create_and_wait_thread(LPTHREAD_START_ROUTINE commandFunc) {
    // Create a child thread to execute the command
    HANDLE hThread = CreateThread(
        NULL,           // default security attributes
        0,              // default stack size
        commandFunc,    // start address of the thread function
        NULL,           // parameter to pass to the thread
        0,              // default creation flags
        NULL);          // thread ID not required

    if (hThread == NULL) {
        std::cerr << "Error: Unable to create thread.\n";
        return;
    }

    // Wait for the thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Close the thread handle
    CloseHandle(hThread);
}

int main() {
    std::string input;

    while (true) {
        // Prompt the user for input
        std::cout << "myShell> ";
        std::getline(std::cin, input);

        // Compare the input to supported commands
        if (input == "dir") {
            create_and_wait_thread(execute_dir);
        } else if (input == "help") {
            create_and_wait_thread(execute_help);
        } else if (input == "vol") {
            create_and_wait_thread(execute_vol);
        } else if (input == "path") {
            create_and_wait_thread(execute_path);
        } else if (input == "exit" || input == "quit") {
            std::cout << "Exiting myShell...\n";
            break;
        } else {
            std::cout << "Error: Unsupported command.\n";
        }
    }

    return 0;
}