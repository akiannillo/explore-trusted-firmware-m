/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_spm_log.h"

#define MAX_DIGIT_BITS 10  /* Max bits of uint32_t value 0xFFFFFFFF add '0x' */
const static char HEX_TABLE[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                 '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/**
 * \brief Convert digit number into HEX format string, return the number of
 *        characters in the string.
 *
 * \param[in]  value  A value need to be converted.
 * \param[in]  msg    A string message that the value converted to.
 *
 * \retval     bits   The number of characters in the msg string.
 */

static uint32_t tfm_itoa(uint32_t value, unsigned char msg[])
{
    int i = 0;
    uint32_t bits;
    uint32_t temp_value = value;

    do {
        i++;  /* Get the bits of the HEX number */
    } while ((temp_value >>= 4) > 0);

    i += 2;   /* Add the prefix '0x' */
    bits = i;
    i--;

    while(i >= 2) {
        msg[i--] = HEX_TABLE[value & 0x0000000F];
        value >>= 4;
    }
    msg[0] = '0';
    msg[1] = 'x';
    return bits;
}

int32_t spm_log_msgval(const char *msg, size_t len, uint32_t value)
{
    int32_t result_msg, result_val;
    uint32_t bits;
    unsigned char value_str[MAX_DIGIT_BITS];

    result_msg = tfm_hal_output_spm_log((const char *)msg, len);
    if (result_msg < TFM_HAL_SUCCESS) {
        return result_msg;
    }

    bits = tfm_itoa(value, value_str);
    result_val = tfm_hal_output_spm_log((const char *)value_str, bits);
    if (result_val < TFM_HAL_SUCCESS) {
        return result_val;
    }
    return (result_msg + result_val);
}
