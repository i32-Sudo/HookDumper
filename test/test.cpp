#include <windows.h>
#include <fstream>
#include <thread>

void CreateAndDeleteFile() {
    while (true) {
        // Create a file
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::ofstream file("test_file.txt");
        if (file.is_open()) {
            file << "This is a test file.";
            file.close();
        }

        // Sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Delete the file
        DeleteFile("test_file.txt");
    }
}

int main() {
    CreateAndDeleteFile();
}