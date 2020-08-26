/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "Driver_Common.h"
#include "target_cfg.h"
#include "tfm_hal_isolation.h"

enum tfm_hal_status_t tfm_hal_set_spe_boundary(void)
{
    /* Configures non-secure memory spaces in the target */
    sau_and_idau_cfg();
    if (mpc_init_cfg() != ARM_DRIVER_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }
    ppc_init_cfg();

    return TFM_HAL_SUCCESS;
}
