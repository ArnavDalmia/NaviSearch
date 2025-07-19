# Requirements Document

## Introduction

The File Indexer is a cross-platform desktop utility that enables users to build and search a filename-to-absolute-path index. The application allows users to scan directories recursively, create searchable indexes, save/load indexes to/from JSON files, and quickly find files through a responsive search interface with clipboard integration.

## Requirements

### Requirement 1

**User Story:** As a user, I want to select one or more root directories to scan, so that I can build a comprehensive file index from multiple locations.

#### Acceptance Criteria

1. WHEN the user launches the application THEN the system SHALL provide a directory selection interface
2. WHEN the user selects multiple root directories THEN the system SHALL accept and store all selected paths
3. WHEN the user confirms directory selection THEN the system SHALL validate that all selected paths exist and are accessible

### Requirement 2

**User Story:** As a user, I want the application to recursively scan selected directories and build an in-memory index, so that I can search through all files efficiently.

#### Acceptance Criteria

1. WHEN the user initiates a scan THEN the system SHALL recursively traverse all subdirectories of selected roots
2. WHEN scanning files THEN the system SHALL create a hashmap with lowercase filename as key and vector of absolute paths as value
3. WHEN scanning completes THEN the system SHALL provide statistics including total files scanned, unique filenames, and scan duration
4. WHEN encountering inaccessible directories THEN the system SHALL log errors but continue scanning other accessible paths

### Requirement 3

**User Story:** As a user, I want to save the built index to a JSON file, so that I can reuse the index later without rescanning.

#### Acceptance Criteria

1. WHEN scan completes THEN the system SHALL prompt user for destination path to save index
2. WHEN saving index THEN the system SHALL write JSON with schema version 1 containing roots, files_total, unique_names, build_seconds, and hashmap
3. WHEN JSON write completes THEN the system SHALL confirm successful save to user
4. IF JSON write fails THEN the system SHALL display error message and allow retry

### Requirement 4

**User Story:** As a user, I want to load an existing JSON index at startup, so that I can immediately search without rebuilding the index.

#### Acceptance Criteria

1. WHEN the user selects load index option THEN the system SHALL provide file selection dialog for JSON files
2. WHEN loading JSON index THEN the system SHALL validate schema version and structure
3. WHEN JSON is valid THEN the system SHALL load the hashmap and display root directories that were indexed
4. IF JSON is invalid or corrupted THEN the system SHALL display error message and allow user to select different file
5. WHEN loading completes THEN the system SHALL display load duration and index statistics

### Requirement 5

**User Story:** As a user, I want to search through the index with real-time filtering, so that I can quickly find files as I type.

#### Acceptance Criteria

1. WHEN the index is loaded THEN the system SHALL display a search box interface
2. WHEN the user types in search box THEN the system SHALL filter results by case-insensitive substring match on each keystroke
3. WHEN filtering THEN the system SHALL display matching filenames with their absolute paths
4. WHEN no matches found THEN the system SHALL display "No results found" message
5. WHEN search box is cleared THEN the system SHALL show all indexed files

### Requirement 6

**User Story:** As a user, I want to copy file paths to clipboard by clicking search results, so that I can easily use the paths in other applications.

#### Acceptance Criteria

1. WHEN the user clicks on a search result THEN the system SHALL copy the absolute path to system clipboard
2. WHEN path is copied THEN the system SHALL display a brief "Copied!" toast notification
3. WHEN multiple paths exist for same filename THEN the system SHALL copy the specific path that was clicked
4. WHEN clipboard operation fails THEN the system SHALL display error message

### Requirement 7

**User Story:** As a user, I want to view detailed statistics about the index, so that I can understand the scope and performance of my file indexing.

#### Acceptance Criteria

1. WHEN the user requests statistics THEN the system SHALL display dialog showing files_total, unique_names, build_seconds or load_seconds, and JSON file size
2. WHEN statistics dialog is open THEN the system SHALL provide "Copy Metrics" button
3. WHEN user clicks "Copy Metrics" THEN the system SHALL copy formatted text summary of all statistics to clipboard
4. WHEN statistics dialog is closed THEN the system SHALL return to main search interface

### Requirement 8

**User Story:** As a user, I want the search interface to remain responsive with large datasets, so that I can efficiently work with indexes containing 100k+ files.

#### Acceptance Criteria

1. WHEN displaying search results THEN the system SHALL implement lazy-loading to show first 100 results initially
2. WHEN user scrolls or expands results THEN the system SHALL load additional results incrementally
3. WHEN filtering large datasets THEN the system SHALL maintain sub-100ms response time for search operations
4. WHEN memory usage exceeds reasonable limits THEN the system SHALL implement efficient memory management for large indexes

### Requirement 9

**User Story:** As a user, I want the application to work consistently across different operating systems, so that I can use it on Windows, macOS, and Linux.

#### Acceptance Criteria

1. WHEN running on any supported platform THEN the system SHALL handle platform-specific path separators correctly
2. WHEN accessing file system THEN the system SHALL use cross-platform file operations
3. WHEN displaying UI THEN the system SHALL render consistently across different operating systems
4. WHEN handling file permissions THEN the system SHALL respect platform-specific access controls