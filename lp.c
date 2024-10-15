#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

// TODO extract into separate function to use recursively
void find_files_recursively(char* dir[]);

int main(int argc, char* argv[]) {
    // windows struct that holds metadata of found files
    // https://learn.microsoft.com/de-de/windows/win32/api/minwinbase/ns-minwinbase-win32_find_dataa
    WIN32_FIND_DATA metadata;
    
    // Search handle -> contains information about first found file / directory
    // https://learn.microsoft.com/de-de/windows/win32/api/fileapi/nf-fileapi-findfirstfilea
    HANDLE handle;

    // error handler
    DWORD err=0;

    // TODO: tchar?
    TCHAR szDir[MAX_PATH];

    // print usage if the directory name isn't specified
    if (argc != 2) {
        printf("Usage: %s <DIRECTORY NAME>\n", argv[0]);
        return 1;
    }

    printf("Hello from C: %s\n", argv[1]);

    // prepare string for use with FindFile functions
    StringCchCopy(szDir, MAX_PATH, argv[1]);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    // find first file in directory
    handle = FindFirstFile(argv[1], &metadata);

    if (handle == INVALID_HANDLE_VALUE) {
        // GetLastError return DWORD (aka unsigned long -> %lu)
        printf("Search failed: %lu\n", GetLastError());
        return 1;
    } 
    
    // TODO skip "." and ".."
    // FIXME stops after first file or dir is found
    do {
        // recurse into subdirectory if found item is dir
        if (metadata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // TODO recurse into dir
            // PathCombine(szDir, argv[1], metadata.cFileName);
            // find_files_recursively(metadata.cFileName);
            
            printf("Dir: %s\n", metadata.cFileName);
        } else {
            printf("File: %s\n", metadata.cFileName);
        }
    }
    while (FindNextFile(handle, &metadata) != 0);

    err = GetLastError();
    if (err != ERROR_NO_MORE_ITEMS) {
        // GetLastError return DWORD (aka unsigned long -> %lu)
        printf("err: %lu\n", err);
    }
    
    FindClose(handle);
    
    return 0;
}
