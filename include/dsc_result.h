#ifndef DSC_RESULT_H
#define DSC_RESULT_H

enum DscReturnCode {
    DSC_OK = 0,
    DSC_ERROR_MEM_ALLOC,
    DSC_ERROR_INVALID_PARAM,
    DSC_ERROR_IS_EMPTY,
    DSC_ERROR_RESIZE_FAIL,
};

/**
 * @brief Represents an error encountered within the library.
 *
 * DscError objects are dynamically allocated when an error occurs.
 * The caller is responsible for freeing the memory using free() when the error
 * is no longer needed.
 */
typedef struct DscError {
    enum DscReturnCode code;
    const char* message;
} DscError;

DscError* create_dsc_error(enum DscReturnCode code, const char* message);

#endif