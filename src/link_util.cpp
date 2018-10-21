#include "link_util.h"
#include "log.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hybris/common/dlfcn.h>
#include <hybris/common/hooks.h>

extern "C" {
#include "../../android_linker/src/common/jb/linker.h"
}

struct _hook {
  const char* name;
  void* func;
};

static struct _hook* user_hooks = NULL;
static int user_hooks_size = 0;
static int user_hooks_arr_size = 0;

void user_hooks_resize() {
  if (user_hooks_arr_size == 0) {
    user_hooks_arr_size = 512;
    user_hooks =
        (struct _hook*) malloc(user_hooks_arr_size * sizeof(struct _hook));
  } else {
    user_hooks_arr_size *= 2;
    struct _hook* new_array =
        (struct _hook*) malloc(user_hooks_arr_size * sizeof(struct _hook));
    memcpy(&new_array[0], &user_hooks[0],
           user_hooks_size * sizeof(struct _hook));
    free(user_hooks);
    user_hooks = new_array;
  }
}

void add_user_hook(struct _hook h) {
  if (user_hooks_size + 1 >= user_hooks_arr_size)
    user_hooks_resize();

  for (int i = 0; i < user_hooks_size; i++) {
    if (strcmp(user_hooks[i].name, h.name) == 0) {
      Log::warn("Linker", "duplicate hook %s", h.name);
    }
  }
  user_hooks[user_hooks_size++] = h;
}

void hybris_register_hooks(struct _hook* hooks) {
  struct _hook* ptr = &hooks[0];
  while (ptr->name != NULL) {
    add_user_hook(*ptr);
    ptr++;
  }
}

void hybris_hook(const char* name, void* func) {
  struct _hook h;
  h.name = name;
  h.func = func;
  add_user_hook(h);
}

void* hybris_hook_callback(const char* name, const char* requester) {
  for (int i = 0; i < user_hooks_size; i++) {
    struct _hook* h = &user_hooks[i];
    if (strcmp(name, h->name) == 0) {
      return h->func;
    }
  }

  return NULL;
}

void* load_os_library(const char* path, const char** symbols) {
  void* handle = dlopen(path, RTLD_LAZY);

  if (handle == NULL) {
    Log::error("Linker", "failed to load OS library %s", path);
    return NULL;
  }

  Log::trace("Linker", "Loaded OS library `%s'", path);

  int i = 0;
  while (1) {
    const char* sym = symbols[i];
    if (sym == NULL)
      break;
    void* ptr = dlsym(handle, sym);
    hybris_hook(sym, ptr);
    i++;
  }

  return handle;
}
