#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> 
#include <nlohmann/json.hpp>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
using json = nlohmann::json;

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

// Function to sanitize strings for UTF-8 compliance
string sanitize_utf8(const string& input) {
    string result;
    result.reserve(input.size() * 2); // Reserve more space for potential expansions
    
    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char c = input[i];
        
        // Standard ascii
        if (c >= 32 && c <= 126) {
            result += c;
        } 
        // Logic below copied from github, search up UTF-8 compliance sanitization function c++, and you can then grab all the conditions
        
        // Handle common Windows-1252 characters that appear in filenames
        else if (c == 0x92) { // right single quotation mark
            result += '\'';
        } else if (c == 0x93 || c == 0x94) { // Left/right double quotation marks
            result += '"';
        } else if (c == 0x91) { // Left single quotation mark
            result += '\'';
        } else if (c == 0x96 || c == 0x97) { //en and em dashes
            result += '-';
        }
        // Skip or replace other problematic characters
        else if (c < 32) {
            result += ' ';
        } else {
            // fallback
            result += '_';
        }
    }
    return result;
}

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
                string path_str = entry.path().string();
                
                // Sanitize both filename and full path for UTF-8 compliance
                string safe_filename = sanitize_utf8(filename);
                string safe_path = sanitize_utf8(path_str);
                
                // Debug output for problematic filenames
                if (filename != safe_filename) {
                    cout << "Sanitized filename: " << filename << " -> " << safe_filename << endl;
                }
                
                string lowercase_filename = safe_filename;
                transform(lowercase_filename.begin(), lowercase_filename.end(), lowercase_filename.begin(), ::tolower);
                file_index[lowercase_filename].push_back(safe_path); //adding to unordered map
                unique_filenames.insert(lowercase_filename); //sets only allow unique filenames

                //cout << entry.path() << "\n";
                files++;
                }
            else if (entry.is_directory()){
                //cout << "\n\nNew Folder: " << entry.path() <<  "\n";
                folders ++;
            }

        }
        auto end_time = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        double build_seconds = duration.count() / 1000.0;
        
        // Deprecated testing string output
        //cout << "\n\n" << "Final Tally: " << "\n" << "Inner Folders: " << folders << "\n" << "Total Files: " << files << endl;
        //cout << "Unique Files: " << unique_filenames.size() << endl;
        //cout <<  "Build Time: " << build_seconds << endl;


        json output = {
            {"schema_version", 1},
            {"roots", {dir}},
            {"files_total", files},
            {"folders_total", folders},
            {"unique_names", unique_filenames.size()},
            {"build_seconds", build_seconds},
            {"hashmap", file_index}
        };

        string save_dir = "C:\\Users\\arnav\\Documents\\GitHub\\NaviSearch\\Maps"; //replace
        filesystem::create_directories(save_dir);

        string sanitized_path = dir; //cleaning the dir for the save file
        replace(sanitized_path.begin(), sanitized_path.end(), '\\', '_');
        replace(sanitized_path.begin(), sanitized_path.end(), ':', '_');
        replace(sanitized_path.begin(), sanitized_path.end(), '/', '_');

        auto now = chrono::system_clock::now(); //for the timestamp
        auto time_t = chrono::system_clock::to_time_t(now);

        string pattern = save_dir + "\\" + sanitized_path + "_"; //basic first part of the saved file name
        bool existing_found = false;
        string existing_file = "";
        
        for (const auto& entry : filesystem::directory_iterator(save_dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                string filename_str = entry.path().filename().string();
                if (filename_str.substr(0, pattern.length() - save_dir.length() - 1) == sanitized_path + "_") {
                    existing_found = true;
                    existing_file = entry.path().string();
                    break;
                }
            }
        }
        
        if (existing_found) {
            cout << "\nWarning: A map for this path already exists: " << filesystem::path(existing_file).filename().string() << endl;
            cout << "Do you want to create a new map? (y/n): ";
            string response;
            getline(cin, response);
            
            if (response != "y" && response != "Y" && response != "yes" && response != "Yes") {
                cout << "Operation cancelled. Using existing map." << endl;
                return 0;
            }
            cout << "Creating new map..." << endl;
        }

        stringstream timestamp;
        timestamp << put_time(localtime(&time_t), "%Y-%m-%d_%H-%M-%S");

        string filename = save_dir + "\\" + sanitized_path + "_" + timestamp.str() + ".json";

        // Create and save json file
        ofstream file(filename);
        file << output.dump(3);
        file.close();
        cout << "\nFile saved to: " << filename << endl;

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
