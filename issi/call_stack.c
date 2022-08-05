#include "call_stack.h"
#include "util.h"

static stack_frame currentFrame = NULL;

stack_frame stack_getCurrentFrame() {
    return currentFrame;
}

void stack_createFrame() {
    dbgprintf("Creating stack frame\n");
    stack_frame frame = malloc(sizeof(stack_frame_t));
    frame->prev = currentFrame;
    currentFrame = frame;
    frame->variables = arr_create();
    frame->returnValue = NULL;
}

void stack_popFrame() {
    dbgprintf("Popping stack frame\n");
    stack_frame frame = currentFrame;
    currentFrame = frame->prev;
    for (int i = 0; i < frame->variables->len; i++) {
        framed_variable fvar = frame->variables->stuff[i];
        framedvar_free(fvar);
    }
    if (frame->returnValue != NULL)
        vimm_free(frame->returnValue);
    free(frame);
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
    if (curr == NULL)
        return NULL;
    for (int i = 0; i < curr->variables->len; i++) {
        framed_variable fvar = curr->variables->stuff[i];
        if (strcmp(fvar->identifier, identifier) == 0)
            return fvar;
    }
    return NULL;
}