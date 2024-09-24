#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>

using namespace std;

// Function to execute system commands:
DWORD WINAPI ExecuteCommand(LPVOID lpParam) {
    std::string* command = static_cast<std::string*>(lpParam);
    system(command->c_str());
    delete command;
    return 0;
}

// Function to parse input string into arguments

/////////////////////////////////////

int main() {
    // welcome message
    //////////////////

    // list of supported commands
    /////////////////////////////

        // parse input into arguments
        // check for exit conditions

        // Check if the command is supported
        if (std::find(supportedCommands.begin(), supportedCommands.end, args[0])
            != supportedCommands.end()) {
                // Create new string on heap to pass the thread
                std::string* command = new std::string(input);

                // Create a thread to execute the command
                HANDLE hThread = CreateThread(
                    NULL,                 // default security attributes
                    0,                    // default stack size
                    ExecuteCommand,       // thread function
                    command,              // parameter to thread function
                    0,                    // default creation flags
                    NULL                  // returns the thread identifier
                );
            
            if (hThread == NULL) {
                std::cerr << "Error creating thread\n";
                delete command;
                continue;
            }

            // Wait for thread to finish
            WaitForSingleObject(hThread, INFINITE);

            // Close the thread handle
            CloseHangle(hThread);
        } else {
            std::cout << "Command not recognized.\n";
        }
}

return 0;