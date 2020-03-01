// object.c

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "error.h"
#include "list.h"
#include "vm.h"
#include "token.h"
#include "str.h"
#include "object.h"

static int func_init(struct Function* func);

struct Object token_to_object(struct Token token) {
	struct Object object = { .type = token.type };
	switch (token.type) {
		case T_NUMBER: {
			obj_number number;
			char* num_string = string_new_copy(token.string, token.length);
			string_to_number(num_string, &number);
			string_free(num_string);
			object.value.number = number;
		}	
			break;

		case T_DECL_NUMBER: {
			object.type = T_NUMBER;
			object.value.number = 0;
		}
			break;

		case T_DECL_VOID:
			object.type = T_VOID;
			break;

		default:
			error("Invalid token type\n");
			break;
	}
	return object;
}

int func_init(struct Function* func) {
	assert(func != NULL);
	func->addr = 0;
	func->stack_offset = 0;
	scope_init(&func->scope, NULL);
	return NO_ERR;
}

int scope_init(struct Scope* scope, struct Scope* parent) {
	assert(scope != NULL);
	scope->constants_count = 0;
	scope->constants = NULL;
	scope->var_locations = ht_create_empty();
	scope->parent = parent;
	return NO_ERR;
}

int func_state_init(struct Func_state* state) {
	assert(state != NULL);
	state->argc = 0;
	state->return_type = T_UNKNOWN;
	func_init(&state->func);
	return NO_ERR;
}

void object_print(const struct Object* object) {
	assert(object != NULL);
	switch (object->type) {
		case T_NUMBER:
			printf("%g\n", object->value.number);
			break;

		case T_VOID:
			printf("<void: %p>\n", object);
			break;

		default:
			break;
	}
}