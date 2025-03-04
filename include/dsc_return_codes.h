#ifndef DSC_RETURN_CODES_H
#define DSC_RETURN_CODES_H

enum DscReturnCode {
    DSC_OK = 0,
    DSC_ERROR_MEM_ALLOC,
    DSC_ERROR_INVALID_PARAM,
    DSC_ERROR_IS_EMPTY,
    DSC_ERROR_RESIZE_FAIL,
} DscReturnCode;

#endif