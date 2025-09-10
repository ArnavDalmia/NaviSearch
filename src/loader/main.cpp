#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

// Cache metadata structure
struct CacheMetadata {
    string original_dir;
    string saved_filename;
    int files_total;
    string created;
    double file_size_mb;
    
    json to_json() const {
        return json{
            {"original_dir", original_dir},
            {"saved_filename", saved_filename},
            {"files_total", files_total},
            {"created", created},
            {"file_size_mb", file_size_mb}
        };
    }
    
    static CacheMetadata from_json(const json& j) {
        CacheMetadata meta;
        meta.original_dir = j["original_dir"];
        meta.saved_filename = j["saved_filename"];
        meta.files_total = j["files_total"];
        meta.created = j["created"];
        meta.file_size_mb = j["file_size_mb"];
        return meta;
    }
};

class NaviSearchLoader {
private:
    string cache_file_path_;
    string maps_directory_;
    unordered_map<string, vector<string>> file_index_;
    bool index_loaded_;
    
public:
    NaviSearchLoader(const string& maps_dir = "C:\\Users\\arnav\\Documents\\GitHub\\NaviSearch\\Maps") : maps_directory_(maps_dir), index_loaded_(false) {
        cache_file_path_ = maps_directory_ + "\\.metadata_cache.json";
        filesystem::create_directories(maps_directory_);
    }
    
    // Load or create cache metadata
    vector<CacheMetadata> load_cache_metadata() {
        vector<CacheMetadata> cache_entries;
        
        if (filesystem::exists(cache_file_path_)) {
            try {
                ifstream cache_file(cache_file_path_);
                json cache_json;
                cache_file >> cache_json;
                
                for (const auto& entry : cache_json["cache_entries"]) {
                    cache_entries.push_back(CacheMetadata::from_json(entry));
                }
                
                cout << "Loaded " << cache_entries.size() << " cached entries" << endl;
            } catch (const exception& ex) {
                cout << "Warning: Could not load cache file: " << ex.what() << endl;
            }
        } else {
            cout << "No cache file found, will create new one" << endl;
        }
        
        return cache_entries;
    }
    
    // Save cache metadata
    void save_cache_metadata(const vector<CacheMetadata>& cache_entries) {
        try {
            json cache_json = {
                {"schema_version", 1},
                {"last_updated", get_current_timestamp()},
                {"cache_entries", json::array()}
            };
            
            for (const auto& entry : cache_entries) {
                cache_json["cache_entries"].push_back(entry.to_json());
            }
            
            ofstream cache_file(cache_file_path_);
            cache_file << cache_json.dump(2);
            cout << "Cache metadata saved with " << cache_entries.size() << " entries" << endl;
        } catch (const exception& ex) {
            cout << "Error saving cache: " << ex.what() << endl;
        }
    }
    
    // Scan existing map files and update cache
    vector<CacheMetadata> scan_and_update_cache() {
        vector<CacheMetadata> cache_entries = load_cache_metadata();
        vector<CacheMetadata> updated_cache;
        
        cout << "Scanning Maps directory for JSON files..." << endl;
        
        if (!filesystem::exists(maps_directory_)) {
            cout << "Maps directory doesn't exist: " << maps_directory_ << endl;
            return updated_cache;
        }
        
        // Track which files we've seen
        set<string> existing_files;
        
        for (const auto& entry : filesystem::directory_iterator(maps_directory_)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                string filename = entry.path().filename().string();
                
                // Skip the cache file itself
                if (filename == ".metadata_cache.json") continue;
                
                existing_files.insert(filename);
                
                // Check if this file is already in cache
                bool found_in_cache = false;
                for (const auto& cached : cache_entries) {
                    if (cached.saved_filename == filename) {
                        updated_cache.push_back(cached);
                        found_in_cache = true;
                        break;
                    }
                }
                
                // If not in cache, try to extract metadata from the file
                if (!found_in_cache) {
                    cout << "New file found: " << filename << ", extracting metadata..." << endl;
                    
                    CacheMetadata new_meta = extract_metadata_from_file(entry.path().string());
                    if (!new_meta.saved_filename.empty()) {
                        updated_cache.push_back(new_meta);
                    }
                }
            }
        }
        
        // Remove entries for files that no longer exist
        cout << "Found " << updated_cache.size() << " valid map files" << endl;
        
        save_cache_metadata(updated_cache);
        return updated_cache;
    }
    
    // Extract metadata from a JSON map file
    CacheMetadata extract_metadata_from_file(const string& file_path) {
        CacheMetadata meta;
        
        try {
            ifstream file(file_path);
            json map_data;
            file >> map_data;
            
            meta.saved_filename = filesystem::path(file_path).filename().string();
            meta.files_total = map_data.value("files_total", 0);
            meta.file_size_mb = filesystem::file_size(file_path) / (1024.0 * 1024.0);
            
            // Try to extract original directory from roots
            if (map_data.contains("roots") && map_data["roots"].is_array() && !map_data["roots"].empty()) {
                meta.original_dir = map_data["roots"][0];
            } else {
                meta.original_dir = "unknown";
            }
            
            // Try to extract creation time from filename or use file modification time
            auto file_time = filesystem::last_write_time(file_path);
            auto time_t = chrono::system_clock::to_time_t(
                chrono::time_point_cast<chrono::system_clock::duration>(
                    file_time - filesystem::file_time_type::clock::now() + chrono::system_clock::now()
                )
            );
            
            stringstream timestamp;
            timestamp << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            meta.created = timestamp.str();
            
        } catch (const exception& ex) {
            cout << "Error extracting metadata from " << file_path << ": " << ex.what() << endl;
            return CacheMetadata{}; // Return empty metadata
        }
        
        return meta;
    }
    
    // Display cache contents
    void display_cache(const vector<CacheMetadata>& cache_entries) {
        if (cache_entries.empty()) {
            cout << "\nNo cached map files found." << endl;
            cout << "Run the indexer first to create some map files!" << endl;
            return;
        }
        
        cout << "\n=== Available Map Files ===" << endl;
        cout << left << setw(4) << "#" 
             << left << setw(40) << "Original Directory"
             << left << setw(8) << "Files"
             << left << setw(8) << "Size(MB)"
             << left << setw(20) << "Created" << endl;
        cout << string(80, '-') << endl;
        
        for (size_t i = 0; i < cache_entries.size(); ++i) {
            const auto& entry = cache_entries[i];
            cout << left << setw(4) << (i + 1)
                 << left << setw(40) << (entry.original_dir.length() > 37 ? 
                                       entry.original_dir.substr(0, 34) + "..." : entry.original_dir)
                 << left << setw(8) << entry.files_total
                 << left << setw(8) << fixed << setprecision(1) << entry.file_size_mb
                 << left << setw(20) << entry.created << endl;
        }
        cout << endl;
    }
    
    // Load a specific map file into memory
    bool load_map_file(const string& filename) {
        string full_path = maps_directory_ + "\\" + filename;
        
        if (!filesystem::exists(full_path)) {
            cout << "Error: Map file not found: " << full_path << endl;
            return false;
        }
        
        cout << "Loading map file: " << filename << "..." << endl;
        auto start_time = chrono::steady_clock::now();
        
        try {
            ifstream file(full_path);
            json map_data;
            file >> map_data;
            
            if (!map_data.contains("hashmap")) {
                cout << "Error: Invalid map file format (no hashmap)" << endl;
                return false;
            }
            
            // Load the hashmap
            file_index_.clear();
            for (const auto& [filename, paths] : map_data["hashmap"].items()) {
                file_index_[filename] = paths;
            }
            
            auto end_time = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
            
            cout << "Successfully loaded " << file_index_.size() << " unique filenames" << endl;
            cout << "Load time: " << duration.count() << " ms" << endl;
            
            index_loaded_ = true;
            return true;
            
        } catch (const exception& ex) {
            cout << "Error loading map file: " << ex.what() << endl;
            return false;
        }
    }
    
    // Search for files
    vector<string> search(const string& query) {
        if (!index_loaded_) {
            cout << "No map file loaded. Please load a map file first." << endl;
            return {};
        }
        
        string lowercase_query = query;
        transform(lowercase_query.begin(), lowercase_query.end(), lowercase_query.begin(), ::tolower);
        
        auto start_time = chrono::steady_clock::now();
        
        vector<string> results;
        auto it = file_index_.find(lowercase_query);
        if (it != file_index_.end()) {
            results = it->second;
        }
        
        auto end_time = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        
        cout << "Search completed in " << duration.count() << " microseconds" << endl;
        return results;
    }
    
    // Get current timestamp
    string get_current_timestamp() {
        auto now = chrono::system_clock::now();
        auto time_t = chrono::system_clock::to_time_t(now);
        stringstream timestamp;
        timestamp << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return timestamp.str();
    }
};

int main() {
    cout << "=== NaviSearch Phase 2 - Loader Program ===" << endl;
    //cout << "Simple Threading Loader with Cache Management" << endl << endl;
    
    NaviSearchLoader loader;
    
    while (true) {
        auto cache_entries = loader.scan_and_update_cache();
        
        if (cache_entries.empty()) { //basically if we have no maps
            cout << "\nNo map files found in the Maps directory." << endl;
            //cout << "Please run the indexer program first to create some map files." << endl;
            
            cout << "Launching indexer to create new map..." << endl;
                
            string indexer_command = "..\\build-phase2\\Debug\\indexer.exe";
            if (!filesystem::exists("..\\build-phase2\\Debug\\indexer.exe")) {
                indexer_command = "indexer.exe";
            }
            
            cache_entries = loader.scan_and_update_cache();
        }
        
        loader.display_cache(cache_entries);
        
        int selection = -1;
        string input;
        
        while (true) { //looped for validated inputs
            cout << "Enter the number of the map to load (1-" << cache_entries.size() << ") or 'ncreate' to create new map: ";
            cin >> input;
            
            if (input == "ncreate") { //reps Navi Create Map feature, on the spot new map creation
                cout << "Launching indexer to create new map..." << endl;
                
                // Try to run indexer.exe from build directory - will seperate later on
                string indexer_command = "..\\build-phase2\\Debug\\indexer.exe";
                if (!filesystem::exists("..\\build-phase2\\Debug\\indexer.exe")) {
                    indexer_command = "indexer.exe";
                }
                
                int result = system(indexer_command.c_str());
                if (result != 0) { //basic handeling
                    cout << "Warning: Could not launch indexer (exit code: " << result << ")" << endl;
                }
                
                //cout << "\nRescanning for new map files..." << endl;
                cache_entries = loader.scan_and_update_cache();
                
                // no map exists even after the run weird, shouldn't happen, need to update later
                if (cache_entries.empty()) {
                    cout << "No map files found after indexer run." << endl;
                    return 0;
                }
                
                loader.display_cache(cache_entries);
                continue;
            }

            if (input == "q" || input == "quit" || input == "Quit"){ //closing functionality
                cout << "Thanks for using NaviSearch!" << endl;
                return 0;
            }
            
            // if not a built in command continue to validate input
            try {
                selection = stoi(input); //built in parse
                if (selection >= 1 && selection <= static_cast<int>(cache_entries.size())) {
                    break;  // Valid selection
                } else {
                    cout << "Invalid selection. Please enter a number between 1 and " << cache_entries.size() << endl;
                }
            } catch (const exception&) {
                cout << "Invalid input. Please enter a number between 1 and " << cache_entries.size() << " or 'ncreate'" << endl;
            }
        }
        
        // After validating input load selected map
        const auto& selected_entry = cache_entries[selection - 1];
        if (!loader.load_map_file(selected_entry.saved_filename)) {
            cout << "Failed to load map file" << endl;
            continue;  // Go back to map selection instead of exiting - shouldn't happpen, file corruption beyond scope
        }
        
        cin.ignore(); // Clear the newline from previous input - redundant
        
        //dummy string for user input filenames
        string query;
        bool back_to_maps = false; //loop bool
        
        cout << "\n=== Search Interface ===" << endl;
        cout << "Enter filename to search, 'nback' to return to map selection, or 'quit' to exit: ";
        
        while (getline(cin, query)) {
            if (query == "quit" || query == "exit") {
                cout << "Thanks for using NaviSearch!" << endl;
                return 0;
            }
            
            if (query == "nback") {
                cout << "Returning to map selection..." << endl;
                back_to_maps = true;
                break;
            }
            
            if (query.empty()) {
                cout << "Enter filename to search, 'nback' to return to map selection, or 'quit' to exit: ";
                continue;
            }
            
            auto results = loader.search(query); //custom search exact match
            
            if (results.empty()) {
                cout << "No files found matching: " << query << endl;
            } else {
                cout << "Found " << results.size() << " file(s):" << endl;
                for (size_t i = 0; i < results.size() && i < 25; ++i) {
                    cout << "  " << (i + 1) << ". " << results[i] << endl;
                }
                if (results.size() > 25) {
                    cout << "  ... and " << (results.size() - 10) << " more" << endl;
                }
            }
            
            cout << "\nEnter filename to search, 'nback' to return to map selection, or 'quit' to exit: ";
        }
        
        if (back_to_maps) {
            continue;  // Go back to the outer loop (map selection)
        } else {
            break;  // Exit the program
        }
    }
    
    cout << "Thanks for using NaviSearch!" << endl;
    return 0;
}
