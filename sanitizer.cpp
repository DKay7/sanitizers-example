// trace-pc-guard-cb.cc
#include <bits/types/FILE.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <sanitizer/coverage_interface.h>
#include <string>

static FILE* trace_output;

// This callback is inserted by the compiler as a module constructor
// into every DSO. 'start' and 'stop' correspond to the
// beginning and end of the section with the guards for the entire
// binary (executable or DSO). The callback will be called at least
// once per DSO and may be called multiple times with the same parameters.
extern "C" void __sanitizer_cov_trace_pc_guard_init(uint32_t *start,
                                                    uint32_t *stop) {
                                                        
                                                        
    void *PC = __builtin_return_address(0);
    char PcDescr[1024];
    // This function is a part of the sanitizer run-time.
    // To use it, link with AddressSanitizer or other sanitizer. 

    static uint64_t N;  // Counter for the guards.
    if (start == stop || *start) return;  // Initialize only once.
    for (uint32_t *x = start; x < stop; x++)
        *x = ++N;  // Guards should start from 1.
}

// This callback is inserted by the compiler on every edge in the
// control flow (some optimizations apply).
// Typically, the compiler will emit the code like this:
//    if(*guard)
//      __sanitizer_cov_trace_pc_guard(guard);
// But for large functions it will emit a simple call:
//    __sanitizer_cov_trace_pc_guard(guard);
extern "C" void __sanitizer_cov_trace_pc_guard(uint32_t *guard) {
       // Duplicate the guard check.
    if (*guard == 0) 
        return;      
    
    FILE* trace_file = fopen("isp-compiler.trace", "a");
    void *PC = __builtin_return_address(0);
        
    char module_offset[1024];
    __sanitizer_symbolize_pc(PC, "%M", module_offset, sizeof(module_offset));
    
    fprintf(trace_file, "%d;%s\n", *guard, module_offset);

    // printf("guard: %p %x PC %s\n", guard, *guard, module_offset);
}


extern "C" void __sanitizer_cov_load1(uint8_t *addr)  {
    printf("I'm 1	%p %c\n", addr, **(char**) addr);
}

extern "C" void __sanitizer_cov_load2(uint16_t *addr)  {
    printf("I'm 2	%p %c\n", addr, **(char**) addr);
}

extern "C" void __sanitizer_cov_load4(uint32_t *addr)  {
    printf("I'm 4	%p %c\n", addr, **(char**) addr);
}

extern "C" void __sanitizer_cov_load8(uint64_t *addr)  {
    printf("I'm 8	%p %c\n", addr, **(char**) addr);
}

extern "C" void __sanitizer_cov_load16(__int128 *addr)  {
    printf("I'm 16	%p %c\n", addr, **(char**) addr);
}

extern "C" void __sanitizer_cov_trace_pc() {
        printf("__sanitizer_cov_trace_pc called\n");
}
