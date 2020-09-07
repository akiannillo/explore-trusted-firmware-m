/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ISOLATION_H__
#define __ISOLATION_H__

#define ISOLATION_IDX_VENEER         0 /* Veneer region */
#define ISOLATION_IDX_CODE           1 /* All codes */
#define ISOLATION_IDX_RODATA         2 /* All RO data */
#if TFM_LVL == 1
#define ISOLATION_IDX_RW_DATA        3 /* All RW data */
#elif TFM_LVL == 2
#define ISOLATION_IDX_PRIV_RW_DATA   3 /* RW data with privileged access only */
#define ISOLATION_IDX_UNPRIV_RW_DATA 4 /* RW data with unprivileged access */
#elif TFM_LVL == 3
#define ISOLATION_IDX_PRIV_RW_DATA   3 /* RW data with privileged access only */
#define ISOLATION_IDX_UNPRIV_ACTIVE  4 /* Active unprivileged RW data region */
#else
#error TFM_LVL ONLY supports 1, 2, and 3!
#endif

/**
 * \brief Set up static isolations that are common for SPM and Secure Partitions
 *
 * \retval IPC_SUCCESS          Static isolation has set up.
 * \retval IPC_ERROR_GENERIC    There was an error setting up the isolations.
 */
int32_t tfm_spm_setup_static_isolation(void);

#endif /* __ISOLATION_H__ */
