/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "region.h"
#include "spm_ipc.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_isolation.h"
#include "tfm_internal_defines.h"

REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Base);
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Limit);

REGION_DECLARE(Image$$, PT_CODE_START, $$Base);
REGION_DECLARE(Image$$, PT_CODE_END, $$Base);
REGION_DECLARE(Image$$, PT_RODATA_START, $$Base);
REGION_DECLARE(Image$$, PT_RODATA_END, $$Base);
#if TFM_LVL == 1
REGION_DECLARE(Image$$, PT_RWZI_START, $$Base);
REGION_DECLARE(Image$$, PT_RWZI_END, $$Base);
#elif TFM_LVL == 2
REGION_DECLARE(Image$$, PT_UNPRIV_RWZI_START, $$Base);
REGION_DECLARE(Image$$, PT_UNPRIV_RWZI_END, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_START, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_END, $$Base);
#endif

int32_t tfm_spm_setup_static_isolation(void)
{
    enum tfm_hal_status_t status;

    status = tfm_hal_isolation_init();
    if (status != TFM_HAL_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }

    status = tfm_hal_enable_memory_access(
                (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Base),
                (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit),
                TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RO |
                TFM_HAL_ACCESS_EXECUTABLE,
                ISOLATION_IDX_VENEER);
    if (status != TFM_HAL_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }

    status = tfm_hal_enable_memory_access(
                (uint32_t)&REGION_NAME(Image$$, PT_CODE_START, $$Base),
                (uint32_t)&REGION_NAME(Image$$, PT_CODE_END, $$Base),
                TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RO |
                TFM_HAL_ACCESS_EXECUTABLE,
                ISOLATION_IDX_CODE);
    if (status != TFM_HAL_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }

    status = tfm_hal_enable_memory_access(
                (uint32_t)&REGION_NAME(Image$$, PT_RODATA_START, $$Base),
                (uint32_t)&REGION_NAME(Image$$, PT_RODATA_END, $$Base),
                TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RO,
                ISOLATION_IDX_RODATA);
    if (status != TFM_HAL_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }

#if TFM_LVL == 1
    /* For Isolation Level 1, set all RW data accessable to unprivileged mode */
    status = tfm_hal_enable_memory_access(
                (uint32_t)&REGION_NAME(Image$$, PT_RWZI_START, $$Base),
                (uint32_t)&REGION_NAME(Image$$, PT_RWZI_END, $$Base),
                TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RW,
                ISOLATION_IDX_RW_DATA);
    if (status != TFM_HAL_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }
#elif TFM_LVL == 2
    /* For Isolation Level 2, isolate privileged and unprivileged data */
    status = tfm_hal_enable_memory_access(
                (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_START, $$Base),
                (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_END, $$Base),
                TFM_HAL_ACCESS_MODE_PRIVILEGED_RW,
                ISOLATION_IDX_PRIV_RW_DATA);
    if (status != TFM_HAL_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }

    status = tfm_hal_enable_memory_access(
                (uint32_t)&REGION_NAME(Image$$, PT_UNPRIV_RWZI_START, $$Base),
                (uint32_t)&REGION_NAME(Image$$, PT_UNPRIV_RWZI_END, $$Base),
                TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RW,
                ISOLATION_IDX_UNPRIV_RW_DATA);
    if (status != TFM_HAL_SUCCESS) {
        return IPC_ERROR_GENERIC;
    }
#endif

    return IPC_SUCCESS;
}