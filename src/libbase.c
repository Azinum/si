// libbase.c

#include <stdlib.h>
#include <stdio.h>

#include "mem.h"
#include "hash.h"
#include "api.h"
#include "lib.h"

int print_state(struct VM_state* vm, struct Scope* scope, int level) {
  if (!scope)
    return 0;
  printf("{\n");
  for (int i = 0; i < ht_get_size(&scope->var_locations); i++) {
    const Hkey* key = ht_lookup_key(&scope->var_locations, i);
    const Hvalue* value = ht_lookup_byindex(&scope->var_locations, i);
    if (key != NULL && value != NULL) {
      for (int i = 0; i < level; i++) printf("  ");
      printf("  %s: ", *key);
      struct Object* object = &vm->variables[*value];
      if (object->type == T_FUNCTION) {
        print_state(vm, &object->value.func.scope, level + 1);
      }
      else {
        object_print(object);
        printf(",\n");
      }
    }
  }
  for (int i = 0; i < level; i++) printf("  ");
  printf("}\n");
  return 0;
}

static int base_print(struct VM_state* vm) {
  int arg_count = si_get_argc(vm);
  if (arg_count <= 0) {
    si_error("Missing arguments\n");
    return 0;
  }
  for (int i = 0; i < arg_count; i++) {
    struct Object* obj = &vm->stack[vm->stack_bp + i];
    object_print(obj);
    printf(" ");
  }
  printf("\n");
  return 0;
}

// Printing 'raw' just means don't use colors
static int base_print_raw(struct VM_state* vm) {
  int arg_count = si_get_argc(vm);
  if (arg_count <= 0) {
    si_error("Missing arguments\n");
    return 0;
  }
  for (int i = 0; i < arg_count; i++) {
    struct Object* obj = &vm->stack[vm->stack_bp + i];
    object_print_raw(obj);
    printf(" ");
  }
  printf("\n");
  return 0;
}

static int base_print_state(struct VM_state* vm) {
  print_state(vm, &vm->global.scope, 0);
  return 0;
}

static int base_print_mem(struct VM_state* vm) {
  memory_print_info();
  return 0;
}

static int base_assert(struct VM_state* vm) {
  struct Object* obj = &vm->stack[vm->stack_bp];
  if (!object_checktrue(obj)) {
    printf("Assertion failed!\n");
    return 0;
  }
  return 0;
}

static struct Lib_def baselib_funcs[] = {
  {"print", base_print},
  {"printr", base_print_raw},
  {"print_state", base_print_state},
  {"print_mem", base_print_mem},
  {"assert", base_assert},
  {NULL, NULL},
};

extern struct Lib_def* libbase() {
  return baselib_funcs;
}
