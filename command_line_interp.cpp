#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <windows.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

DWORD WINAPI execute_tasklist(LPVOID lpParam){
    system("tasklist");
    return 0;
}

DWORD WINAPI execute_notepad(LPVOID lpParam){
    char* fileName = (char*)lpParam;
    if(fileName != nullptr && strlen(fileName) > 0){
        std::string command = "notepad ";
        command += fileName;
        system(command.c_str());
    }
    else{
        system("notepad");
    }
    return 0;
}

// Function to handle the 'dir' command (list directory contents)
DWORD WINAPI execute_dir(LPVOID lpParam) {
    // Get and print the current directory path
    char buffer[MAX_PATH];
    DWORD length = GetCurrentDirectoryA(MAX_PATH, buffer);

    if (length == 0) {
        std::cout << "Error: Could not get the current directory path.\n";
    } else {
        std::cout << "Current directory: " << buffer << std::endl;
    }

    // List the contents of the current directory
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

void setColor(std::string color) {
#ifdef _WIN32
    if (color == "red")
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    else if (color == "green")
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    else if (color == "blue")
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    else if (color == "default")
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
    if (color == "red")
        std::cout << "\033[31m";
    else if (color == "green")
        std::cout << "\033[32m";
    else if (color == "blue")
        std::cout << "\033[34m";
    else if (color == "default")
        std::cout << "\033[0m";
#endif
}

// Function to handle the 'help' command
DWORD WINAPI execute_help(LPVOID lpParam) {
    std::cout << "Supported commands:\n";
    std::cout << "dir  - List the contents of the current directory.\n";
    std::cout << "help - Display this help message.\n";
    //std::cout << "echo - sdfgsdfgsdfg\n";
    //std::cout << "color - sdfgsdfgsdfg\n";
    std::cout << "exit - Exit the shell.\n";
    return 0;
}

DWORD WINAPI execute_vol(LPVOID lpParam) {
    system("vol"); 
    return 0;
}

DWORD WINAPI execute_path(LPVOID lpParam) {  // how do we use this one 
    char* systemPath = getenv("PATH");
    if (systemPath != NULL) {
        std::cout << "system Path:\n"<< systemPath << std::endl;
    } else {
        std::cout << "Unable to retrieve System Path\n";
    }
    return 0;
}

void create_and_wait_thread(LPTHREAD_START_ROUTINE commandFunc, const char* param = nullptr) {
    // Create a child thread to execute the command
    HANDLE hThread = CreateThread(
        NULL,           // default security attributes
        0,              // default stack size
        commandFunc,    // start address of the thread function
        (LPVOID)param,           // parameter to pass to the thread
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

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "echo") {
            std::string message;
            getline(iss, message);
            if (!message.empty() && message[0] == ' ') {
                message = message.substr(1);
            }
            std::cout << message << std::endl;
        }
        else if (input.find("color(") == 0 && input[input.size() - 1] == ')') {
            std::string color = input.substr(6, input.size() - 7);
            if (color == "red" || color == "green" || color == "blue" || color == "default") {
                setColor(color);
            } else {
                std::cout << "Unsupported color! Use 'red', 'green', 'blue', or 'default'.\n";
            }
        }
        else if (input == "dir") {
            create_and_wait_thread(execute_dir);
        } else if (input == "help") {
            create_and_wait_thread(execute_help);
        } else if (input == "vol") {
            create_and_wait_thread(execute_vol);
        } else if (input == "path") {
            create_and_wait_thread(execute_path);
        } else if (input == "tasklist"){
            create_and_wait_thread(execute_tasklist);
        }else if(input.rfind("notepad", 0) == 0){
            std::string fileName = input.length() > 7 ? input.substr(8) : "";
            create_and_wait_thread(execute_notepad, fileName.c_str());
        }else if (input == "exit" || input == "quit") {
            std::cout << "Exiting myShell...\n";
            break;
        } else {
            std::cout << "Error: Unsupported command.\n";
        }
    }

    return 0;
}