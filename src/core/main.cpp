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
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
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


// Simple thread-safe results collector
class SimpleThreadResults {
private:
    std::unordered_map<string, vector<string>> file_index_;
    std::unordered_set<string> unique_filenames_;
    std::mutex mutex_;
    std::atomic<int> files_count_{0};
    std::atomic<int> folders_count_{0};

public:
    void add_file(const string& lowercase_filename, const string& safe_path) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            file_index_[lowercase_filename].push_back(safe_path);
            unique_filenames_.insert(lowercase_filename);
        }
        files_count_++;
    }
    
    void add_folder() {
        folders_count_++;
    }
    // simple get functions
    std::unordered_map<string, vector<string>> get_file_index() const { return file_index_; }
    std::unordered_set<string> get_unique_filenames() const { return unique_filenames_; }
    int get_files_count() const { return files_count_.load(); }
    int get_folders_count() const { return folders_count_.load(); }
};

// Simple worker functions below for processing
// Processes a single directory
void process_directory_simple(const string& dir_path, SimpleThreadResults& results) {
    std::cout << "Thread processing: " << dir_path << std::endl;
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dir_path, std::filesystem::directory_options::skip_permission_denied)) {
            if (entry.is_regular_file()) {
                string filename, path_str;
                try {//utf8 compliance for fulenaming
                    filename = entry.path().filename().u8string();
                    path_str = entry.path().u8string();
                } catch (...) {
                    // Fallback for problematic naming
                    filename = "problematic_filename_" + std::to_string(results.get_files_count());
                    path_str = entry.path().string();
                }
                
                // Sanitize both filename and full path for UTF-8 compliance
                string safe_filename = sanitize_utf8(filename);
                string safe_path = sanitize_utf8(path_str);
                
                // Debug output for problematic filenames
                if (filename != safe_filename) {
                    std::cout << "Sanitized: " << safe_filename << std::endl;
                }
                
                string lowercase_filename = safe_filename;
                std::transform(lowercase_filename.begin(), lowercase_filename.end(), lowercase_filename.begin(), ::tolower);
                
                results.add_file(lowercase_filename, safe_path);
            } else if (entry.is_directory()) {
                results.add_folder();
            }
        }
    } catch (const std::exception& ex) {
        std::cout << "Thread error in " << dir_path << ": " << ex.what() << std::endl;
    }
    
    std::cout << "Thread finished: " << dir_path << std::endl;
}

// Processes ONLY files directly in root directory (non-recursive)
void process_root_files_only(const string& dir_path, SimpleThreadResults& results) {
    std::cout << "Root thread processing files directly in: " << dir_path << std::endl;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
            if (entry.is_regular_file()) {
                string filename, path_str;
                try {
                    filename = entry.path().filename().u8string();
                    path_str = entry.path().u8string();
                } catch (...) {
                    filename = "problematic_filename_" + std::to_string(results.get_files_count());
                    path_str = entry.path().string();
                }
                
                string safe_filename = sanitize_utf8(filename);
                string safe_path = sanitize_utf8(path_str);
                
                if (filename != safe_filename) {
                    std::cout << "Sanitized: " << safe_filename << std::endl;
                }
                
                string lowercase_filename = safe_filename;
                std::transform(lowercase_filename.begin(), lowercase_filename.end(), lowercase_filename.begin(), ::tolower);
                
                results.add_file(lowercase_filename, safe_path);
            } else if (entry.is_directory()) {
                results.add_folder();
            }
        }
    } catch (const std::exception& ex) {
        std::cout << "Root thread error in " << dir_path << ": " << ex.what() << std::endl;
    }
    
    std::cout << "Root thread finished processing files in: " << dir_path << std::endl;
}

using namespace std;

// ============================================================================
// ORIGINAL SINGLE-THREADED MAIN (COMMENTED OUT)
// ============================================================================
/*
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
                
                // Use UTF-8 string methods to avoid encoding issues
                string filename, path_str;
                try {
                    filename = entry.path().filename().u8string();
                    path_str = entry.path().u8string();
                } catch (...) {
                    // Fallback for problematic filenames
                    filename = "problematic_filename_" + to_string(files);
                    path_str = entry.path().string();
                    cout << "Skipped problematic filename, assigned: " << filename << endl;
                }
                
                // Sanitize both filename and full path for UTF-8 compliance
                string safe_filename = sanitize_utf8(filename);
                string safe_path = sanitize_utf8(path_str);
                
                // Debug output for problematic filenames
                if (filename != safe_filename) {
                    cout << "Sanitized: " << safe_filename << endl;
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

        json output = {
            {"schema_version", 1},
            {"roots", {dir}},
            {"files_total", files},
            {"folders_total", folders},
            {"unique_filenames", unique_filenames.size()},
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
*/

// ============================================================================
// PART 1: BASIC THREADED MAIN FUNCTION
// ============================================================================
int main() {
    cout << "=== NaviSearch - PART 1: Basic Threading ===" << endl;
    
    string dir;
    cout << "Enter Dir to scan: ";
    getline(cin, dir);

    if (!filesystem::exists(dir)){
        cout << "Error! Directory doesn't exist. Double check path, ensure it is the absolute path.";
        return 0;
    }
    
    cout << "FOUND! Directory exists! Starting threaded indexing..." << endl;    

    auto start_time = chrono::steady_clock::now();
    
    // Initialize thread-safe results collector
    SimpleThreadResults results;
    
    // For Part 1: Simple approach - just split into a few threads
    // This is basic threading, not full producer-consumer yet
    const int num_threads = thread::hardware_concurrency();
    cout << "Using " << num_threads << " threads" << endl;
    
    vector<thread> workers;
    vector<string> subdirs;
    
    // Simple just process the main directory in multiple threads
    try {
        // Quick scan to find major subdirectories
        for (const auto& entry : filesystem::directory_iterator(dir)) {
            if (entry.is_directory()) {
                subdirs.push_back(entry.path().string());
            }
        }
        
        cout << "Found " << subdirs.size() << " subdirectories to process" << endl;
        
        // ALWAYS dedicate one thread to process root files first
        cout << "Dedicating one thread to root directory files..." << endl;
        workers.emplace_back(process_root_files_only, dir, ref(results)); //updates the results var
        
        // Use remaining threads for subdirectories
        int remaining_threads = num_threads - 1;
        if (remaining_threads > 0 && !subdirs.empty()) {
            cout << "Using " << remaining_threads << " threads for subdirectories..." << endl;            
            for (size_t i = 0; i < subdirs.size() && workers.size() < num_threads; ++i) {
                workers.emplace_back(process_directory_simple, subdirs[i], ref(results));
            }
            
            // If we have remaining subdirs and threads available, process in batches
            for (size_t i = remaining_threads; i < subdirs.size(); i += remaining_threads) {
                // Wait for current batch to complete (except root thread)
                for (size_t j = 1; j < workers.size(); ++j) {
                    if (workers[j].joinable()) workers[j].join();
                }
                
                // Remove completed threads (keep root thread at index 0)
                workers.erase(workers.begin() + 1, workers.end());
                
                // Launch next batch
                for (size_t j = i; j < subdirs.size() && workers.size() < num_threads; ++j) {
                    workers.emplace_back(process_directory_simple, subdirs[j], ref(results));
                }
            }
        } else if (subdirs.empty()) {
            cout << "No subdirectories found, root thread will handle everything" << endl;
        }
        
        // Wait for all threads to complete
        cout << "Waiting for all threads to complete..." << endl;
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        
        auto end_time = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        double build_seconds = duration.count() / 1000.0;
        
        // Get results from thread-safe collector
        auto file_index = results.get_file_index();
        auto unique_filenames = results.get_unique_filenames();
        int files = results.get_files_count();
        int folders = results.get_folders_count();
        
        cout << "\n=== PART 1 THREADING RESULTS ===" << endl;
        cout << "Files processed: " << files << endl;
        cout << "Folders processed: " << folders << endl;
        cout << "Unique filenames: " << unique_filenames.size() << endl;
        cout << "Build time: " << build_seconds << " seconds" << endl;
        
        // Create JSON output (same as original)
        json output = {
            {"schema_version", 1},
            {"threading_version", "Part1_Basic"},
            {"num_threads_used", num_threads},
            {"roots", {dir}},
            {"files_total", files},
            {"folders_total", folders},
            {"unique_filenames", unique_filenames.size()},
            {"build_seconds", build_seconds},
            {"hashmap", file_index}
        };

        // Save file (same logic as original)
        string save_dir = "C:\\Users\\arnav\\Documents\\GitHub\\NaviSearch\\Maps";
        filesystem::create_directories(save_dir);

        string sanitized_path = dir;
        replace(sanitized_path.begin(), sanitized_path.end(), '\\', '_');
        replace(sanitized_path.begin(), sanitized_path.end(), ':', '_');
        replace(sanitized_path.begin(), sanitized_path.end(), '/', '_');

        auto now = chrono::system_clock::now();
        auto time_t = chrono::system_clock::to_time_t(now);

        string pattern = save_dir + "\\" + sanitized_path + "_";
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
            cout << "Creating new threaded map..." << endl;
        }

        stringstream timestamp;
        timestamp << put_time(localtime(&time_t), "%Y-%m-%d_%H-%M-%S");

        string filename = save_dir + "\\" + sanitized_path + "_THREADED_" + timestamp.str() + ".json";

        ofstream file(filename);
        file << output.dump(3);
        file.close();
        cout << "\nThreaded file saved to: " << filename << endl;

    }
    catch (const filesystem::filesystem_error& ex) {
        cout << "\nFilesystem error: " << ex.what() << endl;
        cout << "Partial threaded results - Folders: " << results.get_folders_count() << ", Files: " << results.get_files_count() << endl;
    }
    catch (const exception& ex) {
        cout << "\nGeneral error: " << ex.what() << endl;
        cout << "Partial threaded results - Folders: " << results.get_folders_count() << ", Files: " << results.get_files_count() << endl;
    }

    return 0;
}
