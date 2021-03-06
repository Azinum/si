// object.c

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "error.h"
#include "mem.h"
#include "list.h"
#include "vm.h"
#include "token.h"
#include "str.h"
#include "strarr.h"
#include "object.h"

struct Object token_to_object(struct VM_state* vm, struct Token token) {
	struct Object object = { .type = token.type };
	switch (token.type) {
		case T_NUMBER:
			object.value.number = token.value.number;
			break;

    case T_STRING: {
      // NOTE(lucas): We are doing a string copy here and in the strarr_append function too.
      char* copy = string_new_copy(token.string, token.length);
      int length = token.length;
      strarr_append(&vm->buffers, copy);
      char* top = strarr_top(&vm->buffers);
      assert(top != NULL);
      object.value.str.data = top;
      object.value.str.length = length;
      string_nfree(copy, length);
      break;
    }

    case T_IDENTIFIER:
			object.type = T_NIL;
			break;

		case T_FUNCTION:
			object.type = T_FUNCTION;
			break;

		case T_NIL:
			object.type = T_NIL;
			break;

		default:
			error("Invalid token type\n");
			break;
	}
	return object;
}

int func_init(struct Function* func) {
	assert(func != NULL);
  func_init_with_parent_scope(func, NULL);
	return NO_ERR;
}

int func_init_with_parent_scope(struct Function* func, struct Scope* parent) {
  assert(func != NULL);
  func->addr = 0;
  return scope_init(&func->scope, parent);
}

int scope_init(struct Scope* scope, struct Scope* parent) {
	assert(scope != NULL);
	scope->constants_count = 0;
	scope->constants = NULL;
	scope->var_locations = ht_create_empty();
	scope->parent = parent;
	return NO_ERR;
}

int scope_free(struct Scope* scope) {
	assert(scope != NULL);
	mfree(scope->constants, scope->constants_count * sizeof(struct Object));
	ht_free(&scope->var_locations);
	return NO_ERR;
}

void object_printline(const struct Object* object) {
	assert(object != NULL);
	object_print(object);
	printf("\n");
}

void object_printline_raw(const struct Object* object) {
  assert(object != NULL);
  object_print_raw(object);
  printf("\n");
}

void object_print(const struct Object* object) {
	assert(object != NULL);
	switch (object->type) {
		case T_NUMBER:
			printf(COLOR_NUMBER "%.10g" COLOR_NONE, object->value.number);
			break;

    case T_STRING:
      printf(COLOR_STRING "%.*s" COLOR_NONE, object->value.str.length, object->value.str.data);
      break;

		case T_FUNCTION:
			printf(COLOR_TYPE "[Function]" COLOR_NONE " (addr: %i)", object->value.func.addr);
			break;

    case T_CFUNCTION:
      printf(COLOR_TYPE "[CFunction]" COLOR_NONE " (addr: %p)", object->value.cfunc);
      break;

		case T_NIL:
			printf(COLOR_NIL "[Nil]" COLOR_NONE);
			break;

		case T_LIST:
			printf(COLOR_TYPE "[List] " COLOR_NONE "(of length: %i)", object->value.list->length);
			break;

		default:
			printf(COLOR_TYPE "[Undefined]" COLOR_NONE);
			break;
	}
}

void object_print_raw(const struct Object* object) {
  assert(object != NULL);
	switch (object->type) {
    case T_NUMBER:
      printf("%.10g", object->value.number);
      break;

    case T_STRING:
      printf("%.*s", object->value.str.length, object->value.str.data);
      break;

    case T_FUNCTION:
      printf("[Function] (addr: %i)", object->value.func.addr);
      break;

    case T_CFUNCTION:
      printf("[CFunction] (addr: %p)", object->value.cfunc);
      break;

    case T_NIL:
      printf("[Nil]");
      break;

    case T_LIST:
      printf("[List] (of length: %i)", object->value.list->length);
      break;

    default:
      printf("[Undefined]");
      break;
  }
}

inline int object_checktrue(const struct Object* object) {
  assert(object != NULL);
  switch (object->type) {
    case T_NUMBER:
      return object->value.number != 0;

    case T_STRING:
      return object->value.str.data != NULL;

    case T_FUNCTION:
      return 1;

    case T_NIL:
      return 0;

    case T_LIST:
      return object->value.list->length != 0;

    default:
      assert(0);
      return 0;
  }
}
