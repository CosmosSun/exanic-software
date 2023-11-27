#include <dlfcn.h>

#include "exanic.h"
#include "syscall.h"

#define EXANIC_MK_DECL(ret,fn,args)  ret (*exanic_sys_##fn) args = fn
# include "syscall.h.tmpl"

static int load_sym_fail(const char* sym)
{
  exanic_err_printf("citp_find_calls: ERROR: dlsym(\"%s\") failed '%s'",
            sym, dlerror());
  return -1;
}

static int
exanic_find_all_sys_calls(void)
{
  /*
  ** RTLD_NEXT can be used in place of a library handle, and means 'find
  ** the next occurence of the symbol in the library search order'.
  ** However, it is only available in recent glibc (2.2.5 has it, 2.1.2
  ** doesn't).  If it is not defined, we have to open libc ourselves, and
  ** hope we get the name right!
  */

  /*
  ** NB. If RTLD_NEXT is not defined, we ought to look in libpthread before
  ** we go to libc, since the former adds thread cancellation tests to some
  ** of the sys calls (those prefixed by __libc_).
  */

#ifndef RTLD_NEXT
  void* dlhandle;
  const char* lib = "libc.so.6";  /* ?? */
  dlhandle = dlopen(lib, RTLD_NOW | RTLD_GLOBAL);
  if( dlhandle == 0 ) {    
    exanic_err_printf("%s: ERROR: dlopen(%s) failed dlerror=%s",
              __FUNCTION__, lib, dlerror());
    return -1;
  }
# define EXANIC_MK_DECL_OPTIONAL(ret, fn, args)     \
  exanic_sys_##fn = dlsym(dlhandle, #fn);
#else
# define EXANIC_MK_DECL_OPTIONAL(ret, fn, args)                             \
  exanic_sys_##fn = dlsym(RTLD_NEXT, #fn);                                  \
  if( exanic_sys_##fn == NULL ) {                                           \
    /*                                                                  \
     * NOTE: the socket tester uses dlopen() on libcitransport.so and so \
     *       lookup using RTLD_NEXT may fail in this case. If it does then \
     *       try RTLD_DEFAULT to search all libraries.                  \
     */                                                                 \
    exanic_sys_##fn = dlsym(RTLD_DEFAULT, #fn);                             \
  }
#endif
#define EXANIC_MK_DECL(ret, fn, args)                                       \
  EXANIC_MK_DECL_OPTIONAL(ret, fn, args)                                    \
  if( exanic_sys_##fn == NULL )                                             \
    return load_sym_fail(#fn);

#include "syscall.h.tmpl"

#ifndef RTLD_NEXT
  if( dlclose(dlhandle) != 0 )
    exanic_err_printf("%s: ERROR: dlclose != 0", __FUNCTION__);
#endif

  return 0;
}

int
exanic_syscall_init(void)
{
  if (exanic_find_all_sys_calls() < 0)
    return -1;

  return 0;
}
