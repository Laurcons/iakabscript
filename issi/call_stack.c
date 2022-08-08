#include "call_stack.h"
#include "util.h"

static stack_frame currentFrame = NULL;

stack_frame stack_getCurrentFrame() {
    return currentFrame;
}

void stack_createBlockFrame() {
    dbgprintf("Creating block stack frame\n");
    stack_frame frame = malloc(sizeof(stack_frame_t));
    frame->prev = currentFrame;
    currentFrame = frame;
    frame->variables = arr_create(&vimm_free);
    frame->isFunctionFrame = 0;
    frame->returnValue = NULL;
}

void stack_createFunctionFrame() {
    dbgprintf("Creating function stack frame\n");
    stack_frame frame = malloc(sizeof(stack_frame_t));
    frame->prev = currentFrame;
    currentFrame = frame;
    frame->variables = arr_create(&vimm_free);
    frame->isFunctionFrame = 1;
    frame->returnValue = NULL;
}

void stack_popFrame() {
    dbgprintf("Popping stack frame\n");
    stack_frame frame = currentFrame;
    currentFrame = frame->prev;
    for (int i = 0; i < arr_len(frame->variables); i++) {
        framed_variable fvar = arr_get(frame->variables, i);
        framedvar_free(fvar);
    }
    if (frame->returnValue != NULL)
        vimm_free(frame->returnValue);
    free(frame);
}

void stack_setReturnValue(value_immediate vimm) {
    stack_frame curr = currentFrame;
    // navigate up to the nearest function frame
    while (curr != NULL && !curr->isFunctionFrame) {
        curr = curr->prev;
    }
    if (curr == NULL)
        runtimeStop("Cannot set return value in a non-function scope");
    curr->returnValue = vimm_copy(vimm);
}

framed_variable framedvar_create(char* identifier, value_immediate value) {
    framed_variable fvar = malloc(sizeof(framed_variable_t));
    fvar->identifier = strdup(identifier);
    fvar->value = vimm_copy(value);
    return fvar;
}

void framedvar_free(framed_variable fvar) {
    free(fvar->identifier);
    vimm_free(fvar->value);
    free(fvar);
}

framed_variable stack_lookup(char* identifier) {
    stack_frame curr = currentFrame;
    while (curr != NULL) {
        for (int i = 0; i < arr_len(curr->variables); i++) {
            framed_variable fvar = arr_get(curr->variables, i);
            if (strcmp(fvar->identifier, identifier) == 0)
                return fvar;
        }
        if (curr->isFunctionFrame)
            break;
        curr = curr->prev;
    }
    return NULL;
}