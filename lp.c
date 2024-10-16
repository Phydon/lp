#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// TODO adjust if needed
#define MAX_LINE_LENGTH 1024
#define MAX_FILE_SIZE 1024
#define LEAK_PATTERNS_FILE "leak_patterns.txt"

bool get_leak_patterns(const char *path, const char *words[]) {
    char line[MAX_LINE_LENGTH];

    FILE *file = fopen(path, "r");

    if (!file) {
        printf("Unable to open file: '%s'\n", path);
        return false;
    }

    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        words[i] = line;
        i++;
    }

    fclose(file);

    return true;
}

// Function to search for leaks in a file
void scanfile(const char *filePath, const char *words[], int wordCount) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filePath);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;
    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        for (int i = 0; i < wordCount; i++) {
            if (strstr(line, words[i]) != NULL) {
                printf("'%s' in: '%s' (Line %d):\n  %s", words[i], filePath, lineNumber, line);
            }
        }
    }

    fclose(file);
}

void listFilesRecursively(const char *directory, const char *words[], int wordCount) {
    // A structure that stores information about the file or directory
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // Create the search pattern to find files and subdirectories
    char searchPattern[MAX_PATH];
    snprintf(searchPattern, MAX_PATH, "%s\\*", directory);

    // Start searching for files and subdirectories
    hFind = FindFirstFile(searchPattern, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open directory: %s\n", directory);
        return;
    }

    do {
        const char *name = findFileData.cFileName;

        // Skip the special directories "." and ".."
        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0) {
            // Construct full path of the file or directory
            char fullPath[MAX_PATH];
            snprintf(fullPath, MAX_PATH, "%s\\%s", directory, name);

            // Check if it's a directory or a file
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // It's a directory, recursively list its contents
                listFilesRecursively(fullPath, words, wordCount);
            } else {
                // printf("%s\n", fullPath);
                scanfile(fullPath, words, wordCount);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    // Close the find handle after finishing
    FindClose(hFind);
}

int main(int argc, char **argv) {
    // print usage if the directory name isn't specified
    if (argc != 2) {
        printf("Usage: %s <PATH>", argv[0]);
        return 1;
    }

    // get the list of words to search for
    // FIXME unknown length of array 'words' 
    // TODO get actual filesize of 'LEAK_PATTERNS_FILE
    const char *words[MAX_LINE_LENGTH];
    
    if (!get_leak_patterns(LEAK_PATTERNS_FILE, words)) {
        return 1;
    }
    
    // TODO for testing, remove later
    // const char *words[] = {"todo", "fixme", "info"};
    int wordCount = sizeof(words) / sizeof(words[0]);
    
    // TODO for testing, remove later
    int i = 0;
    for (i = 0; i < wordCount; ++i) {
        printf("%s", words[i]);
    }

    // List all files in the specified directory recursively
    listFilesRecursively(argv[1], words, wordCount);

    return 0;
}
