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

DWORD WINAPI execute_dir(LPVOID lpParam) {     // Get and print the current directory path
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

// Function to handle the 'help' command
DWORD WINAPI execute_help(LPVOID lpParam) {
    std::cout << "Supported commands:\n";
    std::cout << "dir       - List the contents of the current directory.\n";
    std::cout << "help      - Display this help message.\n";
    std::cout << "vol       - Display the volume label and serial number.\n";
    std::cout << "path      - Display the system PATH environment variable.\n";
    std::cout << "tasklist  - Display a list of currently running tasks.\n";
    std::cout << "notepad   - Open Notepad with an optional file name.\n";
    std::cout << "echo      - Display a message on the console.\n";
    std::cout << "color     - Change the console background and foreground colors.\n";
    std::cout << "ping      - Send ICMP Echo Request packets to a network host.\n";
    std::cout << "exit/quit - Exit the shell.\n";
    return 0;
}

DWORD WINAPI execute_vol(LPVOID lpParam) {  //Displays the disk volume label and serial number, if they exist. If used without parameters, vol displays information for the current drive.
    system("vol"); 
    return 0;
}

DWORD WINAPI execute_path(LPVOID lpParam) {   //Sets the command path in the PATH environment variable, specifying the set of directories used to search for executable (.exe) files. If used without parameters, this command displays the current command path.
    char* systemPath = getenv("PATH");
    if (systemPath != NULL) {
        std::cout << "system Path:\n"<< systemPath << std::endl;
    } else {
        std::cout << "Unable to retrieve System Path\n";
    }
    return 0;
}

DWORD WINAPI execute_tasklist(LPVOID lpParam){   //Displays a list of currently running processes on the local computer or on a remote computer.
    system("tasklist");
    return 0;
}

DWORD WINAPI execute_notepad(LPVOID lpParam){  //Launches notepad.exe on Windows. Could be ran with a parameter to search for a file
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

DWORD WINAPI execute_echo(LPVOID lpParam) {  //Displays messages or turns on or off the command echoing feature. If used without parameters, echo displays the current echo setting.
    char* message = static_cast<char*>(lpParam);  // Cast LPVOID to char*
    if (message != nullptr) {
        std::cout << message << std::endl;  // Print the message
    } else {
        std::cout << "Error: No message to echo.\n";
    }
    return 0;
}

void printColorHelp() {  //Help message for changing colors, should be used if no parameter.
    std::cout << "COLOR [attr]\n\n";
    std::cout << "  attr        Specifies color attribute of console output\n\n";
    std::cout << "Color attributes are specified by TWO hex digits -- the first\n";
    std::cout << "corresponds to the background; the second the foreground. Each digit\n";
    std::cout << "can be any of the following values:\n\n";
    std::cout << "    0 = Black       8 = Gray\n";
    std::cout << "    1 = Blue        9 = Light Blue\n";
    std::cout << "    2 = Green       A = Light Green\n";
    std::cout << "    3 = Aqua        B = Light Aqua\n";
    std::cout << "    4 = Red         C = Light Red\n";
    std::cout << "    5 = Purple      D = Light Purple\n";
    std::cout << "    6 = Yellow      E = Light Yellow\n";
    std::cout << "    7 = White       F = Bright White\n\n";
    std::cout << "If no argument is given, this command restores the color to what it was\n";
    std::cout << "when CMD.EXE started. This value either comes from the current console\n";
    std::cout << "window, the /T command line switch, or from the DefaultColor registry value.\n";
}
DWORD WINAPI setColor(LPVOID lpParam) {  //Changes the foreground and background colors in the Command Prompt window for the current session.
    std::string* attrPtr = static_cast<std::string*>(lpParam);
    int colorAttr = std::stoi(*attrPtr, nullptr, 16); //convert hex string to integer
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorAttr);
    return 0;
}

DWORD WINAPI execute_ping(LPVOID lpParam) {  //Verifies IP-level connectivity to another TCP/IP computer by sending Internet Control Message Protocol (ICMP) echo Request messages
                                                        //Use your own IP-address or another one on the network to test. (Different for everyone)
    char* target = static_cast<char*>(lpParam);  // Cast LPVOID to char*
    if (target != nullptr) {
        std::string command = "ping ";
        command += target;  // Append the target (IP or hostname) to the ping command
        system(command.c_str());  // Execute the ping command
    } else {
        std::cout << "Error: No target specified for ping. Do: 'ping <ipaddresshere>\n";
    }
    return 0;
}

void create_and_wait_thread(LPTHREAD_START_ROUTINE commandFunc, LPVOID param = nullptr) {      // Create a child thread to execute the command, parent should wait until Child finishes.
    HANDLE hThread = CreateThread(
        NULL,           // default security attributes
        0,              // default stack size
        commandFunc,    // start address of the thread function
        param,          // parameter to pass to the thread
        0,              // default creation flags
        NULL);          // thread ID not required

    if (hThread == NULL) {
        std::cerr << "Error: Unable to create thread.\n";
        return;
    }

    WaitForSingleObject(hThread, INFINITE);  //parent will wait here 

    // Close the thread handle
    CloseHandle(hThread);
}
int main() {
    std::string input;

    while (true) {
        std::cout << "myShell> ";  //prompt the user with inputs 
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "echo") {
            std::string message;
            getline(iss, message);  // Get the rest of the input as the message, solves our parameter problem due to our if/else 

            // Remove leading space
            if (!message.empty() && message[0] == ' ') {
                message = message.substr(1);
            }

            char* echoMessage = new char[message.length() + 1];  // Create char array for the message
            strcpy(echoMessage, message.c_str());  // Copy the message into the char array

            create_and_wait_thread(execute_echo, (LPVOID)echoMessage);  // Call the echo function
            delete[] echoMessage;  // free
        }
        else if (command == "color") {
            std::string attr;
            iss >> attr;

            if (attr.empty()) {
                // If no attribute is given, print the help message
                printColorHelp();
            } else {
                // Pass the color attribute to the setColor thread
                std::string* attrPtr = new std::string(attr);
                create_and_wait_thread(setColor, attrPtr);
                delete attrPtr;  // free 
            }
        }
        else if (command == "ping") {
            std::string target;
            iss >> target;  // Get the target (hostname or IP address)

            if (!target.empty()) {
                char* targetCStr = new char[target.length() + 1];  // Create char array for the target
                strcpy(targetCStr, target.c_str());  // Copy the target into the char array

                create_and_wait_thread(execute_ping, (LPVOID)targetCStr);  
                delete[] targetCStr;  // free up memory
            } else {
                std::cout << "Error: Please specify a target (hostname or IP address).\n";
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
        }else if (input.rfind("notepad", 0) == 0) {
            std::string fileName = input.length() > 7 ? input.substr(8) : "";
            create_and_wait_thread(execute_notepad, (LPVOID)fileName.c_str());  // casted to LPVOID to type error 
        }else if (input == "exit" || input == "quit") {
            std::cout << "Exiting myShell...\n";
            break;
        } else {
            std::cout << "Error: Unsupported command.\n";
        }
    }

    return 0;
}