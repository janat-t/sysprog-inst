#define _GNU_SOURCE
#include <dlfcn.h>  /* dladdr */
#include <stdarg.h> /* va_list */
#include <stdio.h>  /* printf */
#include <stdlib.h> /* atexit, getenv */
#include <stdint.h> /* uintptr_t */

#define MAX_DEPTH 32
#define MAX_CALLS 1024

void *ROOT = NULL;
#define mod 10007
int cnt[mod];

void close_bracket() {
    FILE *f = fopen("cg.dot", "a");
    if (f != NULL) {
        fprintf(f, "}\n");
        fclose(f);
    }
}

__attribute__((no_instrument_function))
int log_to_stderr(const char *file, int line, const char *func, const char *format, ...) {
    char message[4096];
    va_list va;
    va_start(va, format);
    vsprintf(message, format, va);
    va_end(va);
    return fprintf(stderr, "%s:%d(%s): %s\n", file, line, func, message);
}
#define LOG(...) log_to_stderr(__FILE__, __LINE__, __func__, __VA_ARGS__)

__attribute__((no_instrument_function))
const char *addr2name(void* address) {
    Dl_info dli;
    if (dladdr(address, &dli) == 0) return NULL;
    return dli.dli_sname;
}

__attribute__((no_instrument_function))
const void *addr2saddr(void* address) {
    Dl_info dli;
    if (dladdr(address, &dli) == 0) return NULL;
    return dli.dli_saddr;
}

__attribute__((no_instrument_function))
void __cyg_profile_func_enter(void *addr, void *call_site) {
    /* Not Yet Implemented */
    if (ROOT == NULL) {
        ROOT = addr;
        atexit(close_bracket);
        FILE *f = fopen("cg.dot", "w");
        if (f != NULL) {
            fprintf(f, "strict digraph G {\n");
            fclose(f);
        }
    } else {
        FILE *f = fopen("cg.dot", "a");
        if (f != NULL) {
            if (addr2name(call_site) != NULL) {
                int curcnt = ++cnt[((uintptr_t)addr2saddr(call_site) + (uintptr_t)addr)%mod];
                if (getenv("SYSPROG_LABEL") != NULL) {
                    fprintf(f, "  %s -> %s [label = \"%d\"];\n", addr2name(call_site), addr2name(addr), curcnt);
                } else {
                    fprintf(f, "  %s -> %s;\n", addr2name(call_site), addr2name(addr));
                }
            }
            fclose(f);
        }
    }
}

__attribute__((no_instrument_function))
void __cyg_profile_func_exit(void *addr, void *call_site) {
    /* Not Yet Implemented */
}
