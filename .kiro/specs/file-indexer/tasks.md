# Implementation Plan

- [x] 1. Set up project structure and build system





  - Create CMake configuration with C++20, Qt 6, and nlohmann/json dependencies
  - Set up directory structure for src/, include/, tests/, and build/
  - Configure compiler warnings-as-errors and modern C++ standards
  - Create .gitignore for build artifacts and IDE files
  - _Requirements: 9.1, 9.3_

- [ ] 2. Implement core data structures and utilities
- [ ] 2.1 Create timing and logging utilities
  - Implement Timer class with high-precision timing using std::chrono
  - Create Logger class for structured logging with performance metrics
  - Write unit tests for timing accuracy and logging functionality
  - _Requirements: 2.3, 7.1_

- [ ] 2.2 Implement clipboard management utilities
  - Create ClipboardManager class with cross-platform clipboard operations
  - Implement CopyToClipboard method using Qt clipboard API
  - Add ShowCopyNotification method for toast display
  - Write unit tests for clipboard operations
  - _Requirements: 6.1, 6.2, 9.1_

- [ ] 2.3 Define core data structures
  - Create IndexData struct with schema_version, roots, files_total, unique_names, build_seconds, and hashmap
  - Define IndexHashmap type alias for std::unordered_map<std::string, std::vector<std::string>>
  - Implement ScanResult struct for indexer operations
  - Create ErrorHandler class for structured error reporting
  - _Requirements: 2.2, 3.2, 4.2_

- [ ] 3. Implement storage component for JSON operations
- [ ] 3.1 Create JSON schema validation
  - Implement SchemaValidator class to validate JSON structure and version
  - Create validation methods for required fields and data types
  - Write unit tests for schema validation with valid and invalid JSON
  - _Requirements: 4.2, 4.4_

- [ ] 3.2 Implement JSON serialization
  - Create IndexSerializer class with SaveIndex method using nlohmann/json
  - Implement JSON structure matching the specified schema version 1
  - Add error handling for file write operations
  - Write unit tests for serialization accuracy
  - _Requirements: 3.1, 3.2, 3.3_

- [ ] 3.3 Implement JSON deserialization
  - Add LoadIndex method to IndexSerializer class
  - Implement schema validation during loading process
  - Add timing measurement for load operations
  - Write unit tests for deserialization and round-trip accuracy
  - _Requirements: 4.1, 4.2, 4.3, 4.5_

- [ ] 4. Implement directory indexing component
- [ ] 4.1 Create directory scanner
  - Implement DirectoryScanner class using std::filesystem for recursive traversal
  - Add cross-platform path handling and normalization
  - Implement error handling for inaccessible directories
  - Write unit tests for directory scanning with various file structures
  - _Requirements: 1.1, 1.3, 2.1, 2.4, 9.1, 9.4_

- [ ] 4.2 Implement index builder
  - Create IndexBuilder class with BuildIndex method
  - Implement hashmap construction with lowercase filename keys
  - Add statistics collection (files_total, unique_names, build_seconds)
  - Calculate memory usage estimates for the hashmap
  - Write unit tests for index building accuracy and performance
  - _Requirements: 2.1, 2.2, 2.3, 8.4_

- [ ] 4.3 Integrate scanner and builder components
  - Connect DirectoryScanner output to IndexBuilder input
  - Implement comprehensive error collection and reporting
  - Add performance instrumentation and logging
  - Write integration tests for end-to-end indexing workflow
  - _Requirements: 1.2, 2.4_

- [ ] 5. Create Qt-based user interface foundation
- [ ] 5.1 Implement main window structure
  - Create MainWindow class inheriting from QMainWindow
  - Set up menu bar with File operations (Load/Save/New)
  - Add toolbar with quick actions and statistics button
  - Implement basic window layout and sizing
  - _Requirements: 1.1, 3.1, 4.1_

- [ ] 5.2 Add search interface components
  - Create search box widget with real-time text change signals
  - Implement QListView for displaying search results
  - Add status bar for operation feedback
  - Connect search box to filtering logic placeholder
  - _Requirements: 5.1, 5.2_

- [ ] 5.3 Implement directory selection dialog
  - Create directory selection interface using QFileDialog
  - Support multiple directory selection
  - Add path validation and accessibility checking
  - Integrate with main window workflow
  - _Requirements: 1.1, 1.2, 1.3_

- [ ] 6. Implement data model for search results
- [ ] 6.1 Create base filename model
  - Implement FilenameModel class inheriting from QAbstractListModel
  - Override rowCount and data methods for Qt model interface
  - Implement SetIndex method to populate model with hashmap data
  - Write unit tests for model data access and Qt integration
  - _Requirements: 5.3, 5.4_

- [ ] 6.2 Add search filtering functionality
  - Implement SetFilter method with case-insensitive substring matching
  - Create FilterEngine class for efficient search operations
  - Add performance timing for filter operations
  - Write unit tests for filtering accuracy and performance
  - _Requirements: 5.2, 5.5, 8.3_

- [ ] 6.3 Implement lazy loading for large datasets
  - Add LazyLoader class for incremental result loading
  - Implement ExpandResults method to load additional batches
  - Set initial display limit to 100 results
  - Add memory management for unused result batches
  - Write unit tests for lazy loading behavior with large datasets
  - _Requirements: 8.1, 8.2, 8.3, 8.4_

- [ ] 7. Connect UI components with data model
- [ ] 7.1 Integrate model with list view
  - Connect FilenameModel to QListView in main window
  - Implement proper model-view data binding
  - Add scroll handling for lazy loading expansion
  - Test UI responsiveness with various dataset sizes
  - _Requirements: 5.3, 8.1, 8.2_

- [ ] 7.2 Implement search box filtering
  - Connect search box text changes to model SetFilter method
  - Add real-time filtering on each keystroke
  - Implement "No results found" display for empty results
  - Add clear search functionality
  - _Requirements: 5.1, 5.2, 5.4, 5.5_

- [ ] 7.3 Add result click handling
  - Implement OnResultClicked slot for list view item selection
  - Extract absolute path from clicked result
  - Integrate with ClipboardManager for path copying
  - Display toast notification on successful copy
  - _Requirements: 6.1, 6.2, 6.3, 6.4_

- [ ] 8. Implement index building workflow
- [ ] 8.1 Connect directory selection to indexing
  - Integrate directory selection dialog with IndexBuilder
  - Add progress feedback during scanning process
  - Implement error display for scan failures
  - Update UI with scan results and statistics
  - _Requirements: 1.1, 1.2, 2.1, 2.3, 2.4_

- [ ] 8.2 Add index saving functionality
  - Connect scan completion to save dialog prompt
  - Integrate IndexSerializer for JSON file writing
  - Add save confirmation and error handling
  - Update UI state after successful save
  - _Requirements: 3.1, 3.2, 3.3, 3.4_

- [ ] 8.3 Implement index loading workflow
  - Add file selection dialog for JSON index files
  - Integrate IndexSerializer for loading and validation
  - Update UI with loaded index data and statistics
  - Handle loading errors with user feedback
  - _Requirements: 4.1, 4.2, 4.3, 4.4, 4.5_

- [ ] 9. Create statistics dialog
- [ ] 9.1 Implement statistics display dialog
  - Create StatsDialog class with index metrics display
  - Show files_total, unique_names, build_seconds/load_seconds, and JSON size
  - Add proper dialog layout and formatting
  - Integrate with main window statistics button
  - _Requirements: 7.1_

- [ ] 9.2 Add copy metrics functionality
  - Implement "Copy Metrics" button in statistics dialog
  - Create formatted text summary of all statistics
  - Integrate with ClipboardManager for metrics copying
  - Add confirmation feedback for copy operation
  - _Requirements: 7.2, 7.3_

- [ ] 10. Implement toast notification system
- [ ] 10.1 Create toast overlay widget
  - Implement ToastOverlay class for temporary notifications
  - Add fade-in/fade-out animations using Qt animations
  - Position toast appropriately over main window
  - Create reusable toast display methods
  - _Requirements: 6.2_

- [ ] 10.2 Integrate toast with clipboard operations
  - Connect clipboard copy operations to toast display
  - Show "Copied!" message with appropriate timing
  - Handle multiple rapid copy operations gracefully
  - Test toast behavior across different UI states
  - _Requirements: 6.2_

- [ ] 11. Add comprehensive error handling
- [ ] 11.1 Implement user-facing error dialogs
  - Create standardized error dialog display methods
  - Add appropriate error messages for different failure scenarios
  - Implement retry mechanisms for recoverable errors
  - Test error handling across all major workflows
  - _Requirements: 1.3, 2.4, 3.4, 4.4, 6.4_

- [ ] 11.2 Add logging and debugging support
  - Integrate Logger class throughout application components
  - Add debug mode status bar with filter timing display
  - Implement memory usage logging for large datasets
  - Create log file output for troubleshooting
  - _Requirements: 8.3, 8.4_

- [ ] 12. Write comprehensive tests
- [ ] 12.1 Create unit tests for core components
  - Write GoogleTest suites for indexer, storage, and model components
  - Add parameterized tests for cross-platform compatibility
  - Create mock objects for file system operations
  - Achieve comprehensive test coverage for critical paths
  - _Requirements: 9.1, 9.2, 9.3, 9.4_

- [ ] 12.2 Implement performance benchmarks
  - Create Google Benchmark tests for scanning throughput
  - Add benchmarks for search filter response time
  - Implement memory usage profiling tests
  - Create JSON serialization performance tests
  - _Requirements: 8.3, 8.4_

- [ ] 12.3 Add integration tests
  - Create end-to-end workflow validation tests
  - Test large dataset handling (100k+ files)
  - Validate cross-platform file path handling
  - Add UI integration tests for critical user workflows
  - _Requirements: 8.1, 8.2, 8.3, 8.4, 9.1_

- [ ] 13. Polish and finalization
- [ ] 13.1 Add application icons and branding
  - Create application icon and window icons
  - Add file type icons in search results
  - Implement consistent visual styling
  - Test UI appearance across different platforms
  - _Requirements: 9.3_

- [ ] 13.2 Optimize performance for large datasets
  - Profile memory usage with 100k+ file datasets
  - Optimize search filtering algorithms
  - Implement efficient memory cleanup strategies
  - Validate performance targets are met
  - _Requirements: 8.1, 8.2, 8.3, 8.4_

- [ ] 13.3 Create documentation and build instructions
  - Write README with build steps and dependencies
  - Document sample performance metrics
  - Create user guide for application features
  - Add developer documentation for code structure
  - _Requirements: 9.1, 9.2, 9.3_