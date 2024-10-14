#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    // print usage if the directory name isn't specified
    if (argc != 2) {
        printf("Usage: %s <DIRECTORY NAME>\n", argv[0]);
        return 1;
    }

    printf("Hello from C\n");
    
    return 0;
}
