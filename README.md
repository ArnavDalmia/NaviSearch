# NaviSearch

## Project Description

I'm broke, and my current computer has a 8 yr old hard drive. Searching for files has become the bane of my existence. So, introducing **NaviSearch**, my personal solution to faster file retrieval.

Also, PS: For those who get the reference for the repo name :applause:. If you don't, it's okay, we can't all be cultured. The name comes from **Navi**, the guide from *Legend of Zelda*. If I could recommend something to you, it'd be to play **Breath of the Wild**. It'll change your life. Seriously. 

OKOK, back to the project. Basically, this is a **Windows-only tool** (HAHA NO MAC, :sob: not until I can afford one. Also you can use on a mac i can't really stop you). Upon running it, the tool does a deep dive into your specified directories and creates a hashmap of file names to paths. You may be asking, *"Why a hashmap, Arnav?"* Well, I refer you to **LeetCode** and **Big O**. If you take a look at hashmaps, you'll learn that they average **O(1)** search time, which, in my eyes, is quite nippy.

Once the hashmap is created, it saves to a file. After loading it, you’ll have access to a **dynamic search bar**. Go ahead and search to your heart's desire. Once you find a file or folder, you can **click** it to copy its path directly to your clipboard. It's that simple!

---

## Development Status

🚧 **Currently in active development** - This project is being built in phases for maximum reliability:

- ✅ **Phase 1**: Core file indexing engine (In Progress)
- ✅ **Phase 2**: JSON validation and metrics display  
- ⏳ **Phase 3**: Basic Qt GUI with hardcoded file loading
- ⏳ **Phase 4**: Enhanced GUI with statistics display
- ⏳ **Phase 5**: File selection and menu system
- ⏳ **Phase 6**: Complete indexing workflow integration

📋 See `steps-for-success.md` for detailed development roadmap and success criteria.

---

## Features

- **Fast Search:** Dynamic search bar that updates results in real-time as you type.
- **Path Copy:** Click on any file/folder to copy its full path to the clipboard.
- **Windows Only:** Specifically built for Windows users. (Sorry Mac users, again I can't stop you, but not for you! Mostly because Finder is pretty decent.)
- **Hashmap Indexing:** Creates an efficient index using a hashmap for ideally **O(1)** search time.

---

## Setup Instructions

### Prerequisites

- **C++17 or higher** compiler (MinGW or MSVC recommended for Windows)
- **Qt Framework** (Qt5.15+ or Qt6.x for GUI components)
- **nlohmann/json** library (for JSON serialization/deserialization)

### Steps to Set Up

1. **Clone the repository:**

   ```bash
   git clone https://github.com/ArnavDalmia/NaviSearch.git
   cd NaviSearch

2. **Install Qt**:

   - Download and install the Qt Framework from Qt's official site.
   - After installation, make sure the Qt tools are added to your system’s path (this might be done automatically during the installation).
   - You can also choose to install Qt Creator (an IDE from Qt) to make the development process easier, but it's optional.

3. **Install nlohmann/json**:

   **Option A - vcpkg (Recommended):**
   ```bash
   vcpkg install nlohmann-json
   ```
   
   **Option B - Header-only (Simple):**
   - Download `json.hpp` from [nlohmann/json releases](https://github.com/nlohmann/json/releases)
   - Place it in your project's `include/` directory
   
   **Option C - CMake:**
   ```cmake
   find_package(nlohmann_json 3.2.0 REQUIRED)
   target_link_libraries(your_target nlohmann_json::nlohmann_json)
   ```

4. **Build the Project**:

   **Using Qt Creator:**
   - Open the project in Qt Creator
   - Configure the kit (MinGW or MSVC)
   - Build the project by clicking Build > Build Project
   
   **Using CMake (Alternative):**
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH="path/to/qt"
   cmake --build .
   ```

5. **Run the Application** *(Currently in development - see `steps-for-success.md` for phase-by-phase progress)*


## Contributing

Feel free to fork the repo and submit pull requests! If you want to add new features, fix bugs, or improve documentation, go ahead and make a contribution.

### How to Contribute:
1. **Fork the repository** to your own GitHub account.
2. **Create a new branch** for your feature or bug fix:
   ```bash
   git checkout -b my-new-feature
3. Make your changes and commit them:
   ```bash
      git commit -m "Add a new feature"
4. Push to your forked repo:
   ```bash
      git push origin my-new-feature
5. Submit a PR!
