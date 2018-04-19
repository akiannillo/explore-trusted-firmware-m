/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "interface/include/psa_client.h"
#include "tfm_ns_lock.h"

/**** API functions ****/

uint32_t psa_version(uint32_t sid)
{
    return tfm_ns_lock_svc_dispatch(SVC_PSA_VERSION,
                                    sid,
                                    0,
                                    0,
                                    0);
}

psa_handle_t psa_connect(uint32_t sid, uint32_t minor_version)
{
    return tfm_ns_lock_svc_dispatch(SVC_PSA_CONNECT,
                                    sid,
                                    minor_version,
                                    0,
                                    0);
}

psa_error_t psa_call(psa_handle_t handle,
                     const psa_invec *in_vec,
                     size_t in_len,
                     const psa_outvec *out_vec,
                     size_t out_len)
{
    /* Due to v8M restrictions, TF-M NS API needs to add another layer of
     * serialization in order for NS to pass arguments to S
     */
    psa_invec in_vecs, out_vecs;
    in_vecs.base = in_vec;
    in_vecs.len = in_len;
    out_vecs.base = out_vec;
    out_vecs.len = out_len;
    return tfm_ns_lock_svc_dispatch(SVC_PSA_CALL,
                                    (uint32_t)handle,
                                    (uint32_t)&in_vecs,
                                    (uint32_t)&out_vecs,
                                    0);
}

void psa_close(psa_handle_t handle)
{
    tfm_ns_lock_svc_dispatch(SVC_PSA_CLOSE,
                             (uint32_t)handle,
                             0,
                             0,
                             0);
}

/**** SVC handlers ****/

uint32_t tfm_psa_ns_version(uint32_t sid)
{
    return tfm_psa_version_veneer(sid);
}

psa_handle_t tfm_psa_ns_connect(uint32_t sid, uint32_t minor_version)
{
    return tfm_psa_connect_veneer(sid, minor_version);
}

psa_error_t tfm_psa_ns_call(psa_handle_t handle,
                            const psa_invec *in_vecs,
                            const psa_invec *out_vecs)
{
    /* FixMe: sanity check can be added to offload some NS thread checks from
     * TFM secure API
     */
    return tfm_psa_call_veneer(handle, in_vecs, out_vecs);
}

psa_error_t tfm_psa_ns_close(psa_handle_t handle)
{
    return tfm_psa_close_veneer(handle);
}
