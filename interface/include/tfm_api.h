/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_API_H__
#define __TFM_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "interface/include/psa_client.h"

/* FixMe: sort out DEBUG compile option and limit return value options
 * on external interfaces */
/* Note:
 * TFM will only return values recognized and parsed by TFM core.
 * Service return codes are not automatically passed on to REE.
 * Any non-zero return value is interpreted as an error that may trigger
 * TEE error handling flow.
 */
enum tfm_status_e
{
    TFM_SUCCESS = 0,
    TFM_PARTITION_PENDED,
    TFM_PARTITION_BUSY,
    TFM_ERROR_PARTITION_ALREADY_PENDED,
    TFM_ERROR_SECURE_DOMAIN_LOCKED,
    TFM_ERROR_INVALID_PARAMETER,
    TFM_ERROR_PARTITION_NON_REENTRANT,
    TFM_ERROR_NS_THREAD_MODE_CALL,
    TFM_ERROR_INVALID_EXC_MODE,
    TFM_SECURE_LOCK_FAILED,
    TFM_SECURE_UNLOCK_FAILED,
    TFM_ERROR_GENERIC = 0x1F,
    TFM_PARTITION_SPECIFIC_ERROR_MIN,
};

//==================== Secure function declarations ==========================//

/**
 * \brief Return version of secure function provided by secure binary
 *
 * \param[in]  sid          ID of secure service
 *
 * \return Version number of secure function
 */
uint32_t tfm_psa_version_veneer(uint32_t sid);

/**
 * \brief Connect to secure function
 *
 * \param[in]  sid              ID of secure service
 * \param[in]  minor_version    Minor version of SF requested by client
 *
 * \return Returns handle to connection
 */
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t minor_version);

/**
 * \brief Call a secure function referenced by a connection handle
 *
 * \param[in]  handle   Handle to connection
 * \param[in]  in_vecs  invec containing pointer/count of input vectors
 * \param[in]  out_vecs invec containing pointer/count of output vectors
 *
 * \return Returns \ref psa_error_t error code
 */
psa_error_t tfm_psa_call_veneer(psa_handle_t handle,
                    const psa_invec *in_vecs,
                    const psa_invec *out_vecs);

/**
 * \brief Close connection to secure function referenced by a connection handle
 *
 * \param[in]  handle   Handle to connection
 *
 * \return Returns \ref psa_error_t error code
 */
psa_error_t tfm_psa_close_veneer(psa_handle_t handle);

//================ End Secure function declarations ==========================//

#ifdef __cplusplus
}
#endif

#endif /* __TFM_API_H__ */
