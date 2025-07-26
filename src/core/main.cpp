#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

// Phase 1: Basic setup test
int main(int argc, char* argv[]) {
    std::cout << "=== NaviSearch - Phase 1 ===" << std::endl;
    std::cout << "C++17 filesystem support: " << std::boolalpha 
              << std::filesystem::exists(".") << std::endl;
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <directory_path>" << std::endl;
        std::cout << "Example: " << argv[0] << " \"C:\\Users\"" << std::endl;
        return 1;
    }
    
    std::string target_dir = argv[1];
    std::cout << "Target directory: " << target_dir << std::endl;
    
    if (!std::filesystem::exists(target_dir)) {
        std::cout << "Error: Directory does not exist!" << std::endl;
        return 1;
    }
    
    std::cout << "FOUND! Directory exists! Ready for indexing development." << std::endl;    
    return 0;
}
