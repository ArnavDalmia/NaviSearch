# NaviSearch

## Project Description

I'm broke, and my current computer has a hard drive. Searching for files has become the bane of my existence. So, introducing **NaviSearch**, my personal solution to faster file retrieval.

Also, PS: For those who get the reference for the repo name :applause:. If you don't, it's okay, we can't all be cultured. The name comes from **Navi**, the guide from *Legend of Zelda*. If I could recommend something to you, it'd be to play **Breath of the Wild** (BOTW). It'll change your life. Seriously. 

OKOK, back to the project. Basically, this is a **Windows-only tool** (HAHA NO MAC, :sob: not until I can afford one). Upon running it, the tool does a deep dive into your specified directories and creates a hashmap of file names to paths. You may be asking, *"Why a hashmap, Arnav?"* Well, I refer you to **LeetCode** and **Big O**. If you take a look at hashmaps, you'll learn that they average **O(1)** search time, which, in my eyes, is quite nippy.

Once the hashmap is created, it saves to a file. After loading it, you’ll have access to a **dynamic search bar**. Go ahead and search to your heart's desire. Once you find a file or folder, you can **hover** over it to copy its path directly to your clipboard. It's that simple!

---

## Features

- **Fast Search:** Dynamic search bar that updates results in real-time as you type.
- **Path Copy:** Hover over any file/folder and copy its full path to the clipboard.
- **Windows Only:** Specifically built for Windows users. (Sorry Mac users, not yet!)
- **Hashmap Indexing:** Creates an efficient index using a hashmap for **O(1)** search time.

---

## Setup Instructions

### Prerequisites

- **C++17 or higher** compiler (for Windows)
- **Qt Framework** (for GUI)
- **nlohmann/json** library (for saving/loading JSON files)

### Steps to Set Up

1. **Clone the repository:**

   ```bash
   git clone https://github.com/your-username/navisearch.git
   cd navisearch

2. **Install Qt**:

   - Download and install the Qt Framework from Qt's official site.
   - After installation, make sure the Qt tools are added to your system’s path (this might be done automatically during the installation).
   - You can also choose to install Qt Creator (an IDE from Qt) to make the development process easier, but it's optional.

3. **Install nlohmann/json**:

   - You can install the nlohmann/json library through vcpkg or manually add it to your project:

4. **Build the Project**:

   - Open the project in Qt Creator (or your preferred IDE).
   - Build the project by clicking Build > Build Project.

5. **Run the Application**


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
