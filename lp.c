#include <stdio.h>
#include <windows.h>

void listFilesRecursively(const char *directory) {
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
                listFilesRecursively(fullPath);
            } else {
                printf("%s\n", fullPath);
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

    // List all files in the specified directory recursively
    listFilesRecursively(argv[1]);

    return 0;
}
