// <#file>

#include <dlfcn.h>
#include <stdio.h>

int main(void)
{
    char* file_path = "./libdlopen-calc.so";
    char* func_name = "calc";

    void* handle;
    int (*calc_fn)(int, int);

    // Open a handle to the shared library
    // See: https://manpages.ubuntu.com/manpages/bionic/en/man3/dlopen.3.html
    handle = dlopen(file_path, RTLD_NOW);
    if (!handle) {
        printf("WARNING: %s\n", dlerror());
    }
    else {
        // Load the function from within the library
        calc_fn = dlsym(handle, func_name);
        if (!calc_fn) {
            printf("WARNING: %s\n", dlerror());
        }
    }

    // (Optionally) Use the function 'calc'
    if (handle && calc_fn) {
        printf("Plugin 'calc' loaded, use the function 'calc'\n");
        int value = calc_fn(100, 11);
        printf("value: %d\n", value);
    }
    else {
        printf("Plugin 'calc' NOT loaded, do something else\n");
        int value = 999;
        printf("value: %d\n", value);
    }

    if (handle) {
        // Close the handle to the shared library
        dlclose(handle);
    }

    return 0;
}
