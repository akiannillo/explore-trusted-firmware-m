/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_HAL_ISOLATION_H__
#define __TFM_HAL_ISOLATION_H__

#include <stddef.h>
#include <stdint.h>
#include "tfm_hal_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Memory access attributes */
#define TFM_HAL_ACCESS_EXECUTABLE       0x1
#define TFM_HAL_ACCESS_READABLE         0x2
#define TFM_HAL_ACCESS_WRITABLE         0x4
#define TFM_HAL_ACCESS_UNPRIVILEGED     0x8
#define TFM_HAL_ACCESS_PRIVILEGED       0x10
#define TFM_HAL_ACCESS_DEVICE           0x20
#define TFM_HAL_ACCESS_NS               0x40

/* Commonly used memory access modes */
#define TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RW (TFM_HAL_ACCESS_PRIVILEGED |\
                                              TFM_HAL_ACCESS_UNPRIVILEGED |\
                                              TFM_HAL_ACCESS_READABLE |\
                                              TFM_HAL_ACCESS_WRITABLE)
#define TFM_HAL_ACCESS_MODE_ALL_PRIVILEGE_RO (TFM_HAL_ACCESS_PRIVILEGED |\
                                              TFM_HAL_ACCESS_UNPRIVILEGED |\
                                              TFM_HAL_ACCESS_READABLE)

#define TFM_HAL_ACCESS_MODE_PRIVILEGED_RW    (TFM_HAL_ACCESS_PRIVILEGED |\
                                              TFM_HAL_ACCESS_READABLE |\
                                              TFM_HAL_ACCESS_WRITABLE)
#define TFM_HAL_ACCESS_MODE_PRIVILEGED_RO    (TFM_HAL_ACCESS_PRIVILEGED |\
                                              TFM_HAL_ACCESS_READABLE)

/**
 * \brief  This API sets up the isolation boundary between the SPE and the NSPE.
 *
 * \return TFM_HAL_SUCCESS - the isolation boundary is set up.
 *         TFM_HAL_ERROR_GENERIC - the isolation boundary setup failed.
 */
enum tfm_hal_status_t tfm_hal_set_spe_boundary(void);

/**
 * \brief  Initializes the isolation hardwares of SPE
 *         Clear any existing isolations regions
 *
 * \return TFM_HAL_SUCCESS - initialization finished
 *         TFM_HAL_ERROR_GENERIC - error occurred
 */
enum tfm_hal_status_t tfm_hal_isolation_init(void);

/**
 * \brief  Enables the memory access permissions with the given attributes.
 *         Called by the SPM to set up isolation boundaries inside the SPE such
 *         as PSA RoT isolation boundary and Partition isolation boundary.
 *
 * \param[in]   base - The base address of the memory region.
 * \param[in]   limit - The limit address of the memory region.
 * \param[in]   attr - Memory access attrubites
 * \param[in]   idx - The index of the memory region. Starts from 0.
 *
 * \return TFM_HAL_SUCCESS - The memory access has set up.
 *         TFM_HAL_ERROR_INVALID_INPUT - Invalid input such as limit < base
 *         TFM_HAL_ERROR_MAX_VALUE - The input parameter idx exceeds the
 *                                   supported number of memories regions
 *         TFM_HAL_ERROR_GENERIC - Any other errors occurred.
 */
enum tfm_hal_status_t tfm_hal_enable_memory_access(uint32_t base,
                                                   uint32_t limit,
                                                   uint32_t attr,
                                                   uint32_t idx);

/**
 * \brief  Disables the memory access permissions with the given index.
 *         It is called by the SPM during scheduling Secure Partitions to
 *         disable the access permissions of the partition to be scheduled out.
 *
 * \param[in] idx - The index of the memory region. Starts from 0
 *
 * \return TFM_HAL_SUCCESS - The memory access has been disabled.
 *         TFM_HAL_ERROR_MAX_VALUE - The idx exceeds the supported number of
 *                                   memories regions
 *         TFM_HAL_ERROR_GENERIC - Any other errors occurred.
 */
enum tfm_hal_status_t tfm_hal_disable_memory_access(uint32_t idx);

/**
 * \brief  This API checks if the memory region defined by base and size
 *         has the given atrributes - attr.
 *         The attrubites can include NSPE access, privileged mode, and
 *         read-write permissions.
 *
 * \param[in]   base - The base address of the region.
 *              size - The size of the region.
 *              attr - The memory access attrubites.
 *
 * \return TFM_HAL_SUCCESS - The memory region has the access permissions.
 *         TFM_HAL_ERROR_MEM_FAULT - The memory region has not the access
 *                                   permissions.
 *         TFM_HAL_ERROR_INVALID_INPUT - Invalid inputs.
 *         TFM_HAL_ERROR_GENERIC - An error occurred.
 */
enum tfm_hal_status_t tfm_hal_memory_has_access(const void *base,
                                                size_t size,
                                                uint32_t attr);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_HAL_ISOLATION_H__ */
