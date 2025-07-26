# Local File Indexing and Search Tool (with GUI)

## Project Overview:

This project aims to create a **Windows desktop application** that allows users to **quickly search and access files** by name. The tool will first create an index of files on the local machine, then allow the user to save and load this index for later use. The primary functionality is file name indexing, dynamic search, and easy access to file paths via a user-friendly GUI.

---

## Core Components:

### 1. **File Indexing:**
   - The tool will **scan directories** and **index files** based on their **names** (not metadata). This means the software will extract only the file names and their paths, without scraping additional information like file sizes, types, or modification times.
   - The index will be **saved to disk** in a serialized format (such as JSON or binary), enabling the application to quickly reload the indexed data on subsequent launches.

### 2. **Search Functionality:**
   - A **dynamic search bar** will allow users to type and immediately filter through indexed file names. This will eliminate the need for users to press "Enter" after typing in their search query.
   - The search will be **case-insensitive** and will search for substrings in the file names. The search results will be displayed dynamically as the user types.

### 3. **User Interaction:**
   - Users will interact with the application primarily through a **GUI**.
   - The **load button** will allow the user to load a previously saved index file. After loading, the tool will present a search bar and a list of files.
   - Each file in the search results will be clickable and **hoverable**. When hovering over a file, a **"Copy Path" button** will appear. Clicking this button will copy the full **file path** to the system clipboard, allowing users to quickly access the file location.

### 4. **File Saving and Loading:**
   - The first time the tool is run, it will scan the user's files and **build the index**. This index will be saved to a file on disk.
   - Upon future launches, the application will allow the user to **load the index** from a previously saved file. The application will then present the dynamic search UI for fast file lookups.
   
---

## Features:

- **Index Creation:** Scanning the file system for files and creating an index based on their names.
- **Dynamic Search:** As users type in the search bar, results are updated immediately without needing to press the Enter key.
- **Copy Path:** Users can easily copy a file’s path to their clipboard by hovering over the file in the search results.
- **Load Saved Index:** The ability to load previously saved index files for quicker startup and searching.
- **File Persistence:** Saving the index to a file (in JSON or binary format) that can be loaded again in the future.

---

## Technology Stack:

### **Primary Language: C++**
The tool will be primarily developed in **C++** due to its performance advantages, direct access to system resources, and wide availability of libraries for both system-level tasks (file I/O, indexing) and GUI development.

### **Libraries and Frameworks:**

- **Qt Framework** (for GUI):
  - Qt will be used for building the **Graphical User Interface (GUI)**, which will handle all the user interaction (buttons, text boxes, and the display of search results).
  - **Qt Widgets** will be used for designing the basic UI, including components like:
    - **Search bar** (QLineEdit).
    - **File list** (QListWidget or QTableView).
    - **Buttons** (for "Load" and "Copy Path").
  - **Qt File Dialogs** for opening and saving files when loading/saving the index.
  
- **nlohmann/json** (for JSON serialization):
  - This C++ library will be used for serializing the file index data into JSON format when saving the index and for reading it back into memory when loading the index. JSON is a simple, human-readable format that is easy to work with in C++ and will allow easy inspection of the index if needed.

- **C++17 std::filesystem** (for file system traversal):
  - The **std::filesystem** library (introduced in C++17) will be used to traverse directories and collect file names. This will allow the tool to index files by name and create an internal map of file paths to names.

- **Windows Clipboard API** (for copying file paths):
  - The Windows API or Qt’s `QClipboard` will be used to handle copying the file path to the system clipboard when the "Copy Path" button is clicked.

### **File Formats:**
- **JSON** (for saving and loading the index):
  - The file index will be saved as a **JSON file**. This format is easy to use and inspect, and the **nlohmann/json** library in C++ makes it straightforward to read/write JSON files.
  - Each file name will be the key in the JSON object, with the corresponding file paths stored as a list (for cases where multiple files have the same name).
  
  Example structure:
  ```json
  {
    "file1.txt": ["C:/Users/YourUser/Documents/file1.txt", "D:/file1.txt"],
    "report.pdf": ["C:/Documents/reports/report.pdf"]
  }


### **User Workflow**
- **1. Initial run**:
  - When the user runs the application for the first time, the software will traverse the file system and create an index of all files in the directories the user specifies.
  - After indexing is complete, the user will have the option to save this index to a file (JSON format).
- **2. Subsequent Runs**:
  - On future runs, the user will click the Load button to load the previously saved index file.
  - The index will be read from the file, and the user will be presented with the search bar and the list of indexed files.
  - The user can start typing in the search bar to filter the file names. As they type, the file list will automatically update.
  - Users can hover over each file name in the list, and a "Copy Path" button will appear. When clicked, the full file path will be copied to the clipboard.
- **3. Dynamic Search**:
  - The search will be case-insensitive and will allow the user to find files by name quickly. As the user types, the file list will automatically update without needing to press the Enter key.

### **Optional Future Features (Post MVP):**
- File Open: Alternative option shows when hovered, OPEN, which opens the file
- More Complex Search Filters: Implement additional search filters (e.g., by file extension, directory path, etc.).
- Metadata Indexing: Extend the index to store additional metadata (e.g., file size, modification time) if needed in the future.
