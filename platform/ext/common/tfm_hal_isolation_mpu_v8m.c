/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <arm_cmse.h>
#include <cmsis.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "Driver_Common.h"
#include "mpu_armv8m_drv.h"
#include "spm_ipc.h"
#include "target_cfg.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_isolation.h"

struct mpu_armv8m_dev_t mpu_s = { MPU_BASE };

/*
 * Number of regions supported
 * The MPU driver does not have this function, put it here for now.
 */
static inline uint32_t mpu_armv8m_region_number(void)
{
    return (((MPU_Type *)mpu_s.base)->TYPE >> 8U) & 0xFFUL;
}

enum tfm_hal_status_t tfm_hal_isolation_init(void)
{
    enum mpu_armv8m_error_t mpu_err;

    /* Clear all MPU regions */
    mpu_err = mpu_armv8m_clean(&mpu_s);
    if (mpu_err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_enable_memory_access(uint32_t base,
                                                   uint32_t limit,
                                                   uint32_t attr,
                                                   uint32_t idx)
{
    enum mpu_armv8m_error_t mpu_err;
    struct mpu_armv8m_region_cfg_t cfg;

    if (idx >= mpu_armv8m_region_number()) {
        return TFM_HAL_ERROR_MAX_VALUE;
    }

    if (limit <= base) {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

    cfg.region_nr = idx;
    cfg.region_base = base;
    cfg.region_limit = limit;
    cfg.attr_sh = MPU_ARMV8M_SH_NONE;

    switch (attr & (TFM_HAL_ACCESS_PRIVILEGED |
                    TFM_HAL_ACCESS_UNPRIVILEGED |
                    TFM_HAL_ACCESS_READABLE |
                    TFM_HAL_ACCESS_WRITABLE)) {
    case TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RW:
        cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
        break;
    case TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RO:
        cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_UNPRIV;
        break;
    case TFM_HAL_ACCESS_MODE_PRIVILEGED_RW:
        cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_ONLY;
        break;
    case TFM_HAL_ACCESS_MODE_PRIVILEGED_RO:
        cfg.attr_access = MPU_ARMV8M_AP_RO_PRIV_ONLY;
        break;
    default:
        return TFM_HAL_ERROR_INVALID_INPUT;
        break;
    }

    if (attr & TFM_HAL_ACCESS_EXECUTABLE) {
        cfg.attr_exec = MPU_ARMV8M_XN_EXEC_OK;
        /* Only code is executable */
        cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_CODE_IDX;
    } else {
        cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
        if (attr & TFM_HAL_ACCESS_DEVICE) {
            cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DEVICE_IDX;
        } else {
            cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
        }
    }

    mpu_err = mpu_armv8m_region_enable(&mpu_s, &cfg);
    if (mpu_err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    mpu_err = mpu_armv8m_enable(&mpu_s, PRIVILEGED_DEFAULT_ENABLE,
                                HARDFAULT_NMI_ENABLE);
    if (mpu_err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_disable_memory_access(uint32_t idx)
{
    enum mpu_armv8m_error_t mpu_err;

    if (idx >= mpu_armv8m_region_number()) {
        return TFM_HAL_ERROR_MAX_VALUE;
    }

    mpu_err = mpu_armv8m_region_disable(&mpu_s, idx);
    if (mpu_err != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

    return TFM_HAL_SUCCESS;
}

enum tfm_hal_status_t tfm_hal_memory_has_access(const void *base,
                                                size_t size,
                                                uint32_t attr)
{
    int flags = 0;
    uint32_t privilege_mode;
    bool range_access_allowed_by_mpu = false;

    if (attr & TFM_HAL_ACCESS_PRIVILEGED) {
        privilege_mode = TFM_PARTITION_PRIVILEGED_MODE;
    } else {
        privilege_mode = TFM_PARTITION_UNPRIVILEGED_MODE;
    }

    if (attr & TFM_HAL_ACCESS_NS) {
        CONTROL_Type ctrl;

        /* FIXME: need consider if ns has scheduled and changed priv mode. */
        ctrl.w = __TZ_get_CONTROL_NS();
        if (ctrl.b.nPRIV == 1) {
            privilege_mode = TFM_PARTITION_UNPRIVILEGED_MODE;
        } else {
            privilege_mode = TFM_PARTITION_PRIVILEGED_MODE;
        }
        flags |= CMSE_NONSECURE;
    }

    if (privilege_mode == TFM_PARTITION_UNPRIVILEGED_MODE) {
        flags |= CMSE_MPU_UNPRIV;
    }

    if ((attr & TFM_HAL_ACCESS_READABLE) && (attr & TFM_HAL_ACCESS_WRITABLE)) {
        flags |= CMSE_MPU_READWRITE;
    } else if (attr & TFM_HAL_ACCESS_READABLE) {
        flags |= CMSE_MPU_READ;
    } else {
        return TFM_HAL_ERROR_INVALID_INPUT;
    }

    range_access_allowed_by_mpu =
                    cmse_check_address_range((void *)base, size, flags) != NULL;

    if (!range_access_allowed_by_mpu) {
        return TFM_HAL_ERROR_MEM_FAULT;
    } else {
        return TFM_HAL_SUCCESS;
    }
}
