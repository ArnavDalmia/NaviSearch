# NaviSearch Phase 2 - Loader Program Documentation

## Overview

The **NaviSearch Loader** is Phase 2 of the NaviSearch project, designed to provide fast file search capabilities by loading and searching through pre-built index files created by the Phase 1 indexer. This program implements a cache management system for handling multiple map files and provides an interactive search interface.

## Architecture

The loader follows a **cache-first architecture** optimized for large datasets (500k-1M files):

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Map Files     │    │  Cache Manager  │    │ Search Engine   │
│   (.json)       │◄──►│ (.metadata_     │◄──►│ (in-memory      │
│                 │    │  cache.json)    │    │  hashmap)       │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## Key Components

### 1. CacheMetadata Structure

```cpp
struct CacheMetadata {
    string original_dir;     // Source directory that was indexed
    string saved_filename;   // Name of the JSON map file
    int files_total;         // Number of files in the index
    string created;          // Timestamp when created
    double file_size_mb;     // Size of the map file in MB
}
```

**Purpose**: Stores lightweight metadata about each map file to enable fast selection without loading the entire file.

**JSON Serialization**: 
- `to_json()`: Converts structure to JSON for storage
- `from_json()`: Creates structure from JSON data

### 2. NaviSearchLoader Class

The main class that orchestrates all loader functionality.

#### Private Members

```cpp
string cache_file_path_;                               // Path to .metadata_cache.json
string maps_directory_;                                // Directory containing map files
unordered_map<string, vector<string>> file_index_;     // In-memory file index
bool index_loaded_;                                    // Flag indicating if index is loaded
```

#### Constructor

```cpp
NaviSearchLoader(const string& maps_dir = "C:\\Users\\arnav\\Documents\\GitHub\\NaviSearch\\Maps")
```

- Sets up the Maps directory path
- Creates the cache file path (.metadata_cache.json)
- Ensures the Maps directory exists
- Initializes the loader in an unloaded state

### 3. Cache Management System

#### Cache Loading (`load_cache_metadata()`)

**Flow**:
1. Check if `.metadata_cache.json` exists
2. If exists: Parse JSON and load cache entries
3. If not exists: Return empty cache (will be created)
4. Handle errors gracefully with warnings

**Performance**: O(n) where n = number of cached entries

#### Cache Saving (`save_cache_metadata()`)

**Flow**:
1. Create JSON structure with schema version and timestamp
2. Serialize all cache entries
3. Write to `.metadata_cache.json` with 2-space indentation
4. Provide user feedback on success/failure

**JSON Structure**:
```json
{
  "schema_version": 1,
  "last_updated": "2025-09-10 11:30:00",
  "cache_entries": [
    {
      "original_dir": "C:\\Users\\test",
      "saved_filename": "C__Users_test_THREADED_2025-09-10_11-29-45.json",
      "files_total": 15432,
      "created": "2025-09-10 11:29:45",
      "file_size_mb": 2.4
    }
  ]
}
```

#### Cache Scanning (`scan_and_update_cache()`)

**Purpose**: Synchronizes cache with actual map files on disk

**Algorithm**:
1. Load existing cache metadata
2. Scan Maps directory for `.json` files
3. For each file:
   - Skip `.metadata_cache.json`
   - Check if already in cache → keep existing entry
   - If new → extract metadata and add to cache
4. Save updated cache

**Performance Benefits**:
- Only processes new files
- Preserves existing metadata
- Handles file deletions automatically

### 4. Metadata Extraction (`extract_metadata_from_file()`)

**Purpose**: Extracts essential information from a map file without loading the entire hashmap

**Process**:
1. **File Validation**: Check file exists and is readable
2. **JSON Parsing**: Parse only the metadata sections
3. **Data Extraction**:
   - `saved_filename`: Extract from file path
   - `files_total`: Read from JSON `files_total` field
   - `file_size_mb`: Calculate from file system
   - `original_dir`: Extract from JSON `roots[0]` field
   - `created`: Use file modification time
4. **Error Handling**: Return empty metadata on failure

**Time Complexity**: O(1) - only reads metadata, not the hashmap

### 5. Display System (`display_cache()`)

**Purpose**: Provides a formatted table view of available map files

**Output Format**:
```
=== Available Map Files ===
#   Original Directory                      Files   Size(MB) Created
--------------------------------------------------------------------------------
1   C:\Users\test\Documents                  15432   2.4      2025-09-10 11:29:45
2   C:\Program Files                         8901    1.8      2025-09-10 10:15:23
```

**Features**:
- **Truncation**: Long directory names are truncated with "..."
- **Formatting**: Fixed-width columns for alignment
- **Indexing**: Numbered entries for easy selection

### 6. Map File Loading (`load_map_file()`)

**Purpose**: Loads a complete map file into memory for searching

**Two-Phase Loading Strategy**:

**Phase 1 - Validation**:
- Verify file exists
- Parse JSON structure
- Validate required fields (hashmap)

**Phase 2 - Loading**:
- Clear existing index
- Load hashmap into `unordered_map<string, vector<string>>`
- Set `index_loaded_` flag
- Report loading statistics

**Performance Metrics**:
- **Loading Time**: Measured in milliseconds
- **Memory Usage**: Number of unique filenames loaded
- **Feedback**: Real-time progress information

**Error Handling**:
- File not found
- Invalid JSON format
- Missing hashmap field
- Parse exceptions

### 7. Search Engine (`search()`)

**Purpose**: Provides exact-match filename search with microsecond performance

**Algorithm**:
```cpp
// Input: filename query
// 1. Convert to lowercase
string lowercase_query = query;
transform(lowercase_query.begin(), lowercase_query.end(), lowercase_query.begin(), ::tolower);

// 2. Direct hashmap lookup - O(1) average case
auto it = file_index_.find(lowercase_query);
if (it != file_index_.end()) {
    results = it->second;  // Return all paths for this filename
}
```

**Performance Characteristics**:
- **Time Complexity**: O(1) average case for hashmap lookup
- **Response Time**: Measured in microseconds
- **Memory Efficiency**: Returns references to existing data

**Search Features**:
- **Case-Insensitive**: All queries converted to lowercase
- **Exact Match**: Finds files with exactly matching names
- **Multiple Results**: Returns all paths containing the filename
- **Performance Tracking**: Reports search time

## Program Flow

### Startup Sequence

1. **Initialization**
   ```
   NaviSearchLoader loader;  // Create loader instance
   ```

2. **Cache Management**
   ```
   auto cache_entries = loader.scan_and_update_cache();
   ```
   - Scan Maps directory
   - Update cache with new files
   - Save updated cache

3. **Validation**
   ```
   if (cache_entries.empty()) {
       // No map files found - exit with instructions
   }
   ```

4. **Display Options**
   ```
   loader.display_cache(cache_entries);
   ```
   - Show formatted table of available maps

### Interactive Selection

1. **User Input**
   ```
   cout << "Enter the number of the map to load (1-" << cache_entries.size() << "): ";
   cin >> selection;
   ```

2. **Validation**
   ```
   if (selection < 1 || selection > cache_entries.size()) {
       // Invalid selection handling
   }
   ```

3. **Loading**
   ```
   if (!loader.load_map_file(selected_entry.saved_filename)) {
       // Loading failed
   }
   ```

### Search Loop

1. **Search Interface**
   ```
   cout << "Enter filename to search (or 'quit' to exit): ";
   ```

2. **Query Processing**
   ```
   while (getline(cin, query)) {
       if (query == "quit" || query == "exit") break;
       
       auto results = loader.search(query);
       // Display results
   }
   ```

3. **Result Display**
   - Show number of matches
   - Display up to 10 results
   - Indicate if more results exist

## Performance Characteristics

### Memory Usage

| Component | Memory Impact | Notes |
|-----------|---------------|-------|
| Cache Metadata | ~1KB per map file | Lightweight metadata only |
| File Index | ~50-100 bytes per file | Filename → paths mapping |
| Large Dataset (1M files) | ~50-100MB RAM | Depends on path lengths |

### Speed Benchmarks

| Operation | Target Performance | Typical Performance |
|-----------|-------------------|-------------------|
| Cache Loading | <100ms | 10-50ms |
| Map File Loading | <10s for 1M files | 2-8s depending on file size |
| Search Query | <10ms | <1ms (microseconds) |
| Startup Time | <100ms | 50-200ms |

### Scalability

| Dataset Size | Memory Usage | Load Time | Search Performance |
|--------------|--------------|-----------|-------------------|
| 100k files | ~5-10MB | 0.5-2s | <1ms |
| 500k files | ~25-50MB | 2-5s | <1ms |
| 1M files | ~50-100MB | 5-10s | <1ms |

## File Structure

```
NaviSearch/
├── Maps/                           # Map files directory
│   ├── .metadata_cache.json        # Cache metadata
│   ├── C__Users_test_THREADED_*.json  # Index files
│   └── ...
├── src/
│   └── loader/
│       └── main.cpp               # Loader program
└── build-phase2/
    └── Debug/
        └── loader.exe             # Compiled executable
```

## Error Handling

### Cache File Errors
- **Missing cache file**: Creates new cache automatically
- **Corrupted cache**: Warns user, rebuilds from scratch
- **Permission errors**: Displays error message with details

### Map File Errors
- **File not found**: Clear error message with file path
- **Invalid JSON**: Parsing error with details
- **Missing hashmap**: Validation error message
- **Large files**: Progress indication during loading

### Search Errors
- **No index loaded**: Prompts user to load a map first
- **Empty queries**: Prompts for valid input
- **No results**: Clear "not found" message

## Integration with Phase 1 Indexer

The loader is designed to work seamlessly with map files created by the Phase 1 indexer:

### Expected JSON Format
```json
{
  "schema_version": 1,
  "threading_version": "Part1_Basic",
  "num_threads_used": 8,
  "roots": ["C:\\Users\\test"],
  "files_total": 15432,
  "folders_total": 2341,
  "unique_filenames": 12890,
  "build_seconds": 2.456,
  "hashmap": {
    "readme.txt": [
      "C:\\Users\\test\\readme.txt",
      "C:\\Users\\test\\docs\\readme.txt"
    ],
    "config.json": [
      "C:\\Users\\test\\app\\config.json"
    ]
  }
}
```

### Compatibility
- **Schema Version**: Validates JSON format version
- **Threading Version**: Identifies indexer type used
- **Hashmap Structure**: Direct compatibility with search engine

## Future Enhancements

### Planned Features
1. **Partial Search**: Support for wildcard and fuzzy matching
2. **Search Filters**: Filter by file size, date, extension
3. **Multiple Index Loading**: Load and search across multiple maps
4. **Cache Optimization**: Smart preloading of frequently used maps
5. **REST API**: Web service interface for remote access

### Performance Optimizations
1. **Memory Mapping**: For very large files
2. **Compressed Storage**: Reduce file sizes
3. **Parallel Loading**: Multi-threaded map file loading
4. **Smart Caching**: LRU cache for multiple loaded maps

## Usage Examples

### Basic Usage
```bash
# Build the loader
cd build-phase2
cmake --build . --config Debug

# Run the loader
.\Debug\loader.exe
```

### Expected Output
```
=== NaviSearch Phase 2 - Loader Program ===
Simple Threading Loader with Cache Management

Scanning Maps directory for JSON files...
New file found: C__Users_test_THREADED_2025-09-10_11-29-45.json, extracting metadata...
Found 1 valid map files
Cache metadata saved with 1 entries

=== Available Map Files ===
#   Original Directory                      Files   Size(MB) Created
--------------------------------------------------------------------------------
1   C:\Users\test                          15432   2.4      2025-09-10 11:29:45

Enter the number of the map to load (1-1): 1
Loading map file: C__Users_test_THREADED_2025-09-10_11-29-45.json...
Successfully loaded 12890 unique filenames
Load time: 234 ms

=== Search Interface ===
Enter filename to search (or 'quit' to exit): readme.txt
Search completed in 15 microseconds
Found 2 file(s):
  1. C:\Users\test\readme.txt
  2. C:\Users\test\docs\readme.txt

Enter filename to search (or 'quit' to exit): quit
Thanks for using NaviSearch!
```

## Technical Design Decisions

### Cache Strategy
- **Metadata-only caching**: Keeps memory usage low while providing fast selection
- **Lazy loading**: Only loads the selected map file into memory
- **Automatic synchronization**: Cache updates automatically when new files are detected

### Data Structures
- **unordered_map for search**: O(1) average-case lookup performance
- **vector for file paths**: Efficient storage of multiple paths per filename
- **Structured metadata**: Easy JSON serialization and human-readable cache

### User Experience
- **Clear progress indication**: Users know what's happening during long operations
- **Graceful error handling**: Helpful error messages with suggested actions
- **Interactive interface**: Simple number selection and text-based search

This design achieves the goals of **fast startup**, **efficient memory usage**, and **microsecond search performance** while maintaining **simplicity** and **reliability**.
