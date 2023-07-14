#include <cstdio>
#include <cstdlib>
#include <malloc.h>
#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
#include <stdio.h>

typedef void* so_lib_t;
typedef void (*foo_t)(char*);

extern "C" void foo(char *str);
extern "C" void foo2(char *str);

so_lib_t load_shared_object(const char* lib_name) {
	return dlopen(lib_name, RTLD_NOW | RTLD_GLOBAL);
}

void unload_shared_object(so_lib_t shared_object) {
	dlclose(shared_object);
}

void* load_procedure(so_lib_t shared_object, const char* procedure_name) {
	return dlsym(shared_object, procedure_name);
}

//--------------------------------------------------------------------------------

// static int
// callback(struct dl_phdr_info *info, size_t size, void *data)
// {
    // int j;
// 
//    printf("name=%s (%d segments)\n", info->dlpi_name,
        // info->dlpi_phnum);
// 
//    for (j = 0; j < info->dlpi_phnum; j++)
        //  printf("\t\t header %2d: address=%10p\n", j,
            //  (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
    // return 0;
// }

int main() {

    char *buffer  = (char*) malloc(1024);
    char *buffer2 = (char*) malloc(1024);

    so_lib_t shared_obj = NULL;
    foo_t foo = NULL;
    foo_t foo2 = NULL;


	#define check_err(var, func)	\
        var = func
		// do {if (!(var = func)) {fprintf(stderr, "Error occured in %s:%d : %s\n", __FILE__, __LINE__, dlerror()); abort();}} while(0)
    
    check_err(shared_obj, load_shared_object("./build/example-module.so"));
    check_err(foo,  (foo_t) load_procedure(shared_obj, "foo"));
    check_err(foo2, (foo_t) load_procedure(shared_obj, "foo2"));

    foo(buffer);
    foo2(buffer2);
    // dl_iterate_phdr(callback, NULL);

    unload_shared_object(shared_obj);
    shared_obj = NULL;
    
    free(buffer);
    free(buffer2);
}
