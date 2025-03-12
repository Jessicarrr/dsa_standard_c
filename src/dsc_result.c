#include <stdio.h>
#include <stdlib.h>
#include "dsc_result.h"

DscError* create_dsc_error(enum DscReturnCode code, const char* message) {
    DscError* err = (DscError*) malloc(sizeof(DscError));
    if (err != NULL) {
        err->code = code;
        err->message = message;
    }
    return err;
}