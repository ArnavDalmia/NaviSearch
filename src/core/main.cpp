#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

// Phase 1: Basic setup test
// int main(int argc, char* argv[]) {
//     std::cout << "=== NaviSearch - Phase 1 ===" << std::endl;
//     std::cout << "C++17 filesystem support: " << std::boolalpha 
//               << std::filesystem::exists(".") << std::endl;
    
//     if (argc < 2) {
//         std::cout << "Usage: " << argv[0] << " <directory_path>" << std::endl;
//         std::cout << "Example: " << argv[0] << " \"C:\\Users\"" << std::endl;
//         return 1;
//     }
    
//     std::string target_dir = argv[1];
//     std::cout << "Target directory: " << target_dir << std::endl;
    
//     if (!std::filesystem::exists(target_dir)) {
//         std::cout << "Error: Directory does not exist!" << std::endl;
//         return 1;
//     }
    
//     std::cout << "FOUND! Directory exists! Ready for indexing development." << std::endl;    
//     return 0;
// }

using namespace std;

int main() {
    string dir;
    cout << "Enter Dir to scan: ";
    getline(cin, dir);

    if (!std::filesystem::exists(dir)){
        cout << "Error! Directory doesn't exist. Double check path, ensure it is the absolute path.";
        return 0;
    }
    
    cout << "FOUND! Directory exists! Ready for indexing development." << std::endl;    

    int folders = 0;
    int files = 0;

    try{
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                cout << entry.path() << "\n";
                files++;
                }
            else if (entry.is_directory()){
                cout << "Entering new Folder: " << entry.path() <<  "\n\n";
                folders ++;
            }

            }

            cout << "\n\n" << "Final Tally: " << "\n" << "Folders: " << folders << "\n" << "Files: " << files;
            
    }
    catch (exception except) {
        cout << "Error! Error Message: ";
    }

    return 0;
}
