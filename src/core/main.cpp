#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> 

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
    auto start_time = chrono::steady_clock::now();
    unordered_map<string, vector<string>> file_index;
    unordered_set<string> unique_filenames;


    cout << "Found Files: \n";
    try{
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dir,filesystem::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file()) {
                
                string filename = entry.path().filename().string();
                string lowercase_filename = filename;
                transform(lowercase_filename.begin(), lowercase_filename.end(), lowercase_filename.begin(), ::tolower);
                file_index[lowercase_filename].push_back(entry.path().string()); //adding to unordered map
                unique_filenames.insert(lowercase_filename); //sets only allow unique filenames

                cout << entry.path() << "\n";
                files++;
                }
            else if (entry.is_directory()){
                cout << "\n\nNew Folder: " << entry.path() <<  "\n";
                folders ++;
            }

        }
        auto end_time = chrono::steady_clock::now();
        cout << "\n\n" << "Final Tally: " << "\n" << "Inner Folders: " << folders << "\n" << "Total Files: " << files << endl;
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        double build_seconds = duration.count() / 1000.0;
        cout << "Unique Files: " << unique_filenames.size() << endl;
        cout <<  "Build Time: " << build_seconds << endl;

    }
    catch (const std::filesystem::filesystem_error& ex) {
        cout << "\nFilesystem error: " << ex.what() << endl;
        cout << "Partial results - Inner Folders: " << folders << ", Files: " << files << endl;
    }
    catch (const std::exception& ex) {
        cout << "\nGeneral error: " << ex.what() << endl;
        cout << "Partial results - Inner Folders: " << folders << ", Files: " << files << endl;
    }

    return 0;
}
