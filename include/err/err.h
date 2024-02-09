#pragma once

/**
 * @brief Macro to check if a value represents an error.
 *
 * This macro checks if the given value is equal to the error code.
 *
 * @param x The value to check.
 * @param value The error code to compare against.
 * @return Returns true if the value represents an error, false otherwise.
 */
#define ERR_VALUE(x, value) ((x) == (value))

/**
 * @brief Macro to check if a pointer is NULL.
 *
 * @param ptr The pointer to check.
 * @return Returns true if the pointer is NULL, false otherwise.
 */
#define IS_NULL_PTR(ptr) ((ptr) == NULL)

typedef enum ERR
{
  ERR_SUCCESS,
  ERR_FAILURE
} ERR;

/**
 * @brief Macro to check if a value represents a failure error code.
 *
 * This macro checks if the given error code represents a failure.
 *
 * @param err The error code to check.
 * @return Returns true if the error code represents a failure, false otherwise.
 */
#define IS_ERR_FAILURE(err) ERR_VALUE(err, ERR_FAILURE)