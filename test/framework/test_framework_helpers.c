/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "test_framework_helpers.h"

#include <stdio.h>

const char *psa_status_to_str(psa_status_t status)
{
    switch (status) {
    case PSA_SUCCESS:
        return "PSA_SUCCESS";
    case PSA_ERROR_PROGRAMMER_ERROR:
        return "PSA_ERROR_PROGRAMMER_ERROR";
    case PSA_ERROR_CONNECTION_REFUSED:
        return "PSA_ERROR_CONNECTION_REFUSED";
    case PSA_ERROR_CONNECTION_BUSY:
        return "PSA_ERROR_CONNECTION_BUSY";
    case PSA_ERROR_GENERIC_ERROR:
        return "PSA_ERROR_GENERIC_ERROR";
    case PSA_ERROR_NOT_PERMITTED:
        return "PSA_ERROR_NOT_PERMITTED";
    case PSA_ERROR_NOT_SUPPORTED:
        return "PSA_ERROR_NOT_SUPPORTED";
    case PSA_ERROR_INVALID_ARGUMENT:
        return "PSA_ERROR_INVALID_ARGUMENT";
    case PSA_ERROR_INVALID_HANDLE:
        return "PSA_ERROR_INVALID_HANDLE";
    case PSA_ERROR_BAD_STATE:
        return "PSA_ERROR_BAD_STATE";
    case PSA_ERROR_BUFFER_TOO_SMALL:
        return "PSA_ERROR_BUFFER_TOO_SMALL";
    case PSA_ERROR_ALREADY_EXISTS:
        return "PSA_ERROR_ALREADY_EXISTS";
    case PSA_ERROR_DOES_NOT_EXIST:
        return "PSA_ERROR_DOES_NOT_EXIST";
    case PSA_ERROR_INSUFFICIENT_MEMORY:
        return "PSA_ERROR_INSUFFICIENT_MEMORY";
    case PSA_ERROR_INSUFFICIENT_STORAGE:
        return "PSA_ERROR_INSUFFICIENT_STORAGE";
    case PSA_ERROR_INSUFFICIENT_DATA:
        return "PSA_ERROR_INSUFFICIENT_DATA";
    case PSA_ERROR_SERVICE_FAILURE:
        return "PSA_ERROR_SERVICE_FAILURE";
    case PSA_ERROR_COMMUNICATION_FAILURE:
        return "PSA_ERROR_COMMUNICATION_FAILURE";
    case PSA_ERROR_STORAGE_FAILURE:
        return "PSA_ERROR_STORAGE_FAILURE";
    case PSA_ERROR_HARDWARE_FAILURE:
        return "PSA_ERROR_HARDWARE_FAILURE";
    case PSA_ERROR_INVALID_SIGNATURE:
        return "PSA_ERROR_INVALID_SIGNATURE";
    default:
        return "Unknown error";
    }
}

const char *asset_perms_to_str(uint8_t permissions)
{
    switch (permissions) {
    case 0:
        return "No permissions";
    case 1:
        return "SECURE_ASSET_REFERENCE";
    case 2:
        return "SECURE_ASSET_WRITE";
    case 3:
        return "SECURE_ASSET_REFERENCE | SECURE_ASSET_WRITE";
    case 4:
        return "SECURE_ASSET_READ";
    case 5:
        return "SECURE_ASSET_REFERENCE | SECURE_ASSET_READ";
    case 6:
        return "SECURE_ASSET_WRITE | SECURE_ASSET_READ";
    case 7:
        return "SECURE_ASSET_REFERENCE | SECURE_ASSET_WRITE | "
               "SECURE_ASSET_READ";
    default:
        return "Unknown permissions";
    }
}

void printf_set_color(enum serial_color_t color_id)
{
    TEST_LOG("\33[3%dm", color_id);
}
