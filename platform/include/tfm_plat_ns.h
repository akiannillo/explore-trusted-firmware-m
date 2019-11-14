/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLAT_NS_H__
#define __TFM_PLAT_NS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Platform peripherals and devices initialization.
 *
 * \return  ARM_DRIVER_OK if the initialization succeeds
*/
int32_t tfm_ns_platform_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLAT_NS_H__ */
