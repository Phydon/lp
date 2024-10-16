#include <stdio.h>
#include <windows.h>
#include <string.h>

// TODO adjust if needed
#define MAX_LINE_LENGTH 1024

// TODO
// Function to search for the word "todo" in a file
void scanfile(const char *filePath, const char *words[], int wordCount) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filePath);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;
    while (fgets(line, sizeof(line), file)) {
        lineNumber++;
        for (int i = 0; i < wordCount; i++) {
            if (strstr(line, words[i]) != NULL) {
                printf("Leak found: '%s' in: '%s' (Line %d): %s", words[i], filePath, lineNumber, line);
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
    // Define the list of words to search for
    // TODO change search patterns later
    const char *words[] = {"todo", "fixme", "info"};
    int wordCount = sizeof(words) / sizeof(words[0]);

    // print usage if the directory name isn't specified
    if (argc != 2) {
        printf("Usage: %s <PATH>", argv[0]);
        return 1;
    }

    // List all files in the specified directory recursively
    listFilesRecursively(argv[1], words, wordCount);

    return 0;
}
