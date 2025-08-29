# NaviSearch Development Plan - Steps for Success
## AI generated with GPT for helping plan out phases with my input.

## Overview
This document outlines the phased development approach for NaviSearch, focusing on incremental building and testing of each component before moving to the next phase.

---

## Phase 1: Core File Indexing Engine (C++ Console Application)

### Objective
Build a robust C++ console application that scans directories, creates an efficient hashmap, and saves it to a structured JSON file with comprehensive metrics.

### Key Components to Build

**Setup Requirements:**
- Install nlohmann/json library (via vcpkg, package manager, or header-only inclusion)
- Ensure C++17 or higher compiler support
- Include `<nlohmann/json.hpp>` in your project

1. **DirectoryScanner Class**
   - Use `std::filesystem::recursive_directory_iterator`
   - Handle permission errors gracefully
   - Skip system/hidden files if needed

2. **IndexBuilder Class**
   - Create `std::unordered_map<std::string, std::vector<std::string>>`
   - Key: lowercase filename for case-insensitive search
   - Value: vector of absolute file paths

3. **MetricsCollector Class**
   - Track scan performance
   - Count total files vs unique filenames
   - Calculate memory usage estimates

4. **JSONSerializer Class**
   - Use nlohmann/json library for serialization
   - Implement the exact schema format specified
   - Handle JSON writing with proper error checking

### Expected JSON Output Format
**File location**: `C:\Users\arnav\Documents\NaviSearch\[sanitized-directory-path]_YYYY-MM-DD_HH-MM-SS.json`

**Example filenames:**
- `C_Users_arnav_Documents_2025-08-01_14-30-15.json`
- `D_Projects_Code_2025-08-01_15-45-22.json`
- `E_BackupDrive_Photos_2025-08-01_16-12-08.json`

```json
{
  "schema_version": 1,
  "roots": ["C:/Users/arnav/Documents"],
  "files_total": 15847,
  "unique_names": 12394,
  "build_seconds": 4.23,
  "hashmap": {
    "readme.md": ["C:/Users/arnav/Documents/proj1/readme.md", "C:/Users/arnav/Documents/proj2/README.md"],
    "main.cpp": ["C:/Users/arnav/Documents/code/main.cpp"],
    "config.json": ["C:/Users/arnav/Documents/app/config.json"]
  }
}
```

### User Workflow
1. **Run executable**: `indexer.exe` (no command-line arguments needed)
2. **Program prompts**: "Enter directory to map: "
3. **User inputs**: Directory path (e.g., `C:\Users\arnav\Documents`)
4. **Program scans** directory and creates hashmap with progress updates
5. **Saves JSON** to hardcoded location: `C:\Users\arnav\Documents\NaviSearch\`
6. **Output**: "File saved to: C:\Users\arnav\Documents\NaviSearch\C_Users_arnav_Documents_2025-08-01_14-30-15.json"

### Deliverables
- **Console application**: `indexer.exe`
- **Usage**: Simply run `indexer.exe` → program prompts for directory → creates descriptive JSON file
- **Hardcoded save location**: `C:\Users\arnav\Documents\NaviSearch\Maps` (auto-created if doesn't exist)
- **Filename format**: `[sanitized-path]_YYYY-MM-DD_HH-MM-SS.json` (path-based + timestamped)
- **Path sanitization**: Replaces `\` and `:` with `_` for valid filenames
- **Error handling**: Graceful handling of permission denied, network drives, invalid paths
- **Performance logging**: Console output showing scan progress and final "File saved!" message

### Success Criteria
- [ ] Program prompts user for directory input (no command-line args needed)
- [ ] Successfully scans single directory root with recursive traversal
- [ ] Generates valid JSON with correct metrics in specified schema format
- [ ] Handles duplicate filenames correctly in hashmap structure
- [ ] Saves JSON to hardcoded location: `C:\Users\arnav\Documents\NaviSearch\`
- [ ] Creates descriptive filename based on scanned directory path
- [ ] Sanitizes directory path for valid filename (replaces `\` and `:` with `_`)
- [ ] Includes timestamp for uniqueness when same directory scanned multiple times
- [ ] Auto-creates save directory if it doesn't exist
- [ ] Displays "File saved!" confirmation message
- [ ] Completes scan of 10,000+ files in reasonable time
- [ ] Proper error handling for inaccessible directories and invalid input

---

## Phase 2: JSON Loader and Metrics Validator (C++ Console Application)

### Objective
Create a command-line tool that loads the JSON file, validates the data integrity, and displays metrics to verify the indexing worked correctly.

### Key Components to Build
1. **JSONLoader Class**
   - Load and parse JSON files using nlohmann/json
   - Validate schema version compatibility
   - Reconstruct the hashmap in memory
   - Handle JSON parsing errors gracefully

2. **MetricsValidator Class**
   - Verify files_total matches actual hashmap entries
   - Confirm unique_names count is accurate
   - Display memory usage statistics

3. **IntegrityChecker Class**
   - Sample check: verify some file paths still exist
   - Validate hashmap structure consistency

### Expected Console Output
```
=== NaviSearch Index Loader ===
Loading index from: C:\data\file_index.json

✓ JSON loaded successfully
✓ Schema version: 1 (supported)
✓ Scan roots: 2 directories
  - C:/Users/foo
  - D:/Projects

=== Metrics Summary ===
Total files indexed: 15,847
Unique filenames: 12,394
Duplicate names: 3,453 (21.8%)
Build time: 4.23 seconds
Estimated memory: 2.1 MB

=== Sample Data ===
"readme.md" appears in 23 locations
"config.json" appears in 8 locations
"main.cpp" appears in 156 locations

✓ Index loaded successfully - ready for search operations
```

### Deliverables
- **Console application**: `loader.exe`
- **Usage**: `loader.exe "file_index.json"`
- **Validation**: Confirms JSON integrity and displays human-readable metrics

### Success Criteria
- [ ] Successfully loads and parses JSON files from Phase 1
- [ ] Metrics calculations match original scan results
- [ ] Clear, informative console output
- [ ] Detects and reports any data corruption issues
- [ ] Handles missing or malformed JSON files gracefully

---

## Phase 3: Basic Qt GUI - Hardcoded JSON Display

### Objective
Build a minimal Qt GUI that loads a hardcoded JSON file and displays the searchable file index without any file selection complexity.

### Key Components to Build
1. **MainWindow Class**
   - Simple window with search bar and results list
   - Hardcoded path: `"./file_index.json"`

2. **SearchEngine Class**
   - Case-insensitive substring matching
   - Real-time filtering as user types

3. **ResultsDisplay Class**
   - QListWidget showing filenames prominently with directory paths in lighter text
   - Alternative: Full paths with bold filenames
   - Click on any result to copy absolute path to clipboard

### UI Layout
```
┌─────────────────────────────────────┐
│ NaviSearch                          │
├─────────────────────────────────────┤
│ Search: [________________]          │
├─────────────────────────────────────┤
│ Results:                            │
│ ┌─────────────────────────────────┐ │
│ │ readme.md           C:\proj1\   │ │
│ │ README.md           D:\proj2\   │ │
│ │ readme.txt          C:\docs\    │ │
│ │ ...                             │ │
│ └─────────────────────────────────┘ │
├─────────────────────────────────────┤
│ Status: 156 results found           │
└─────────────────────────────────────┘
```

**Alternative Layout (Full path with bold filename):**
```
┌─────────────────────────────────────┐
│ Results:                            │
│ ┌─────────────────────────────────┐ │
│ │ C:\proj1\readme.md              │ │
│ │ D:\proj2\README.md              │ │
│ │ C:\docs\readme.txt              │ │
│ └─────────────────────────────────┘ │
```

### Deliverables
- **Qt application**: `navisearch_gui.exe`
- **Hardcoded JSON path**: Loads `./file_index.json` on startup
- **Basic search**: Dynamic filtering as user types
- **Results display**: Filenames shown prominently, paths in lighter text (or bold filenames in full paths)
- **Clipboard copy**: Click on any result to copy absolute path to clipboard

### Success Criteria
- [ ] GUI launches without errors
- [ ] Successfully loads and displays JSON data
- [ ] Search filtering works in real-time
- [ ] Clipboard functionality works
- [ ] Handles missing JSON file gracefully

---

## Phase 4: Enhanced GUI - Metrics Display

### Objective
Extend the GUI to show index statistics and provide better user feedback about the loaded data.

### Key Components to Add
1. **StatsPanel Class**
   - Display index metrics in a dedicated panel
   - Show scan roots, file counts, build time

2. **StatusBar Enhancements**
   - Show current filter results count
   - Display load time and memory usage

3. **About Dialog**
   - Show application info and loaded index details

### Enhanced UI Layout
```
┌─────────────────────────────────────┐
│ NaviSearch                     [?]  │
├─────────────────────────────────────┤
│ Index: 15,847 files | 12,394 unique │
│ Search: [________________]          │
├─────────────────────────────────────┤
│ Results:                            │
│ ┌─────────────────────────────────┐ │
│ │ readme.md           C:\proj1\   │ │
│ │ README.md           D:\proj2\   │ │
│ └─────────────────────────────────┘ │
├─────────────────────────────────────┤
│ 156 of 15,847 files | Built: 4.2s  │
└─────────────────────────────────────┘
```

### Deliverables
- **Enhanced GUI**: Extended version with metrics display
- **Stats dialog**: Detailed information about loaded index
- **Better UX**: More informative status updates

### Success Criteria
- [ ] Metrics display correctly from JSON
- [ ] Status bar shows useful information
- [ ] About dialog provides index details
- [ ] UI remains responsive with large datasets

---

## Phase 5: File Selection UI

### Objective
Replace hardcoded JSON path with a file selection dialog, allowing users to load any saved index.

### Key Components to Add
1. **FileDialog Integration**
   - Open file dialog for JSON selection
   - Remember last used directory

2. **Menu System**
   - File menu with "Open Index" option
   - Recent files list

3. **Error Handling UI**
   - User-friendly error messages
   - Loading progress indicators

### Enhanced UI Features
```
┌─────────────────────────────────────┐
│ File  Help                          │
│ ├─ Open Index...        Ctrl+O     │
│ ├─ Recent Files        ►           │
│ └─ Exit                             │
├─────────────────────────────────────┤
│ No index loaded - Open File > Index │
│ OR                                  │
│ [Index: 15,847 files loaded]       │
│ Search: [________________]          │
└─────────────────────────────────────┘
```

### Deliverables
- **File selection**: Browse and load any JSON index file
- **Menu system**: Professional application menus
- **Recent files**: Quick access to previously loaded indexes

### Success Criteria
- [ ] File dialog opens and filters JSON files
- [ ] Successfully loads different index files
- [ ] Recent files list works correctly
- [ ] Proper error handling for invalid files
- [ ] Keyboard shortcuts work

---

## Phase 6: Index Creation UI

### Objective
Add the ability to create new indexes directly from the GUI, completing the full application workflow.

### Key Components to Add
1. **IndexCreation Dialog**
   - Directory selection for scan roots
   - Progress bar during scanning
   - Save location selection

2. **Background Processing**
   - Non-blocking index creation
   - Cancel operation capability
   - Progress updates

3. **Workflow Integration**
   - Create → Save → Load seamless flow
   - Auto-load newly created indexes

### Complete UI Flow
```
File Menu:
├─ New Index...           # Phase 6 - NEW
├─ Open Index...         # Phase 5
├─ Recent Files         ►
└─ Exit

New Index Dialog:
┌─────────────────────────────────────┐
│ Create New Index                    │
├─────────────────────────────────────┤
│ Scan Directories:                   │
│ ┌─────────────────────────────────┐ │
│ │ C:\Users\Username               │ │
│ │ D:\Projects                     │ │
│ └─────────────────────────────────┘ │
│ [Add Directory] [Remove Selected]   │
├─────────────────────────────────────┤
│ Save As: [path/to/index.json] [...] │
├─────────────────────────────────────┤
│ Progress: ████████░░ 80% (8,450/10k)│
│ [Cancel]              [Create Index]│
└─────────────────────────────────────┘
```

### Deliverables
- **Index creation dialog**: Complete directory scanning UI
- **Progress tracking**: Real-time scan progress
- **Integrated workflow**: Create → Save → Load in one app

### Success Criteria
- [ ] Directory selection works correctly
- [ ] Progress bar updates during scanning
- [ ] Cancel operation works properly
- [ ] Newly created index auto-loads
- [ ] Error handling for scan failures
- [ ] Save location selection works

---

## Final Success Criteria

### Performance Targets
- **Scanning**: Handle 50,000+ files efficiently
- **Search**: Sub-100ms response time for filtering
- **Memory**: Reasonable usage for large indexes
- **Startup**: Quick application launch

### Quality Standards
- **Error Handling**: Graceful degradation for all error conditions
- **User Experience**: Intuitive workflow and helpful feedback
- **Data Integrity**: Reliable JSON serialization/deserialization
- **Cross-platform**: Works on different Windows versions

### Testing Approach
- Test each phase thoroughly before moving to next
- Use real directory structures for testing
- Validate with both small and large datasets
- Test error conditions and edge cases

---

## Development Notes

### Tools and Libraries
- **C++17/20**: Modern C++ features
- **nlohmann/json**: JSON serialization
- **Qt6**: GUI framework (Phase 3+)
- **std::filesystem**: Directory traversal
- **Google Test**: Unit testing (optional)

### File Structure
```
NaviSearch/
├── src/
│   ├── core/           # Phase 1-2 components
│   ├── gui/            # Phase 3+ components
│   └── main.cpp
├── data/               # Test JSON files
├── build/             # Build artifacts
└── docs/              # Documentation
```

This phased approach ensures each component is solid before building the next layer, reducing integration issues and making debugging much easier.
