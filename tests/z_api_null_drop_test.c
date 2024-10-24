//
// Copyright (c) 2022 ZettaScale Technology
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
// which is available at https://www.apache.org/licenses/LICENSE-2.0.
//
// SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
//
// Contributors:
//   ZettaScale Zenoh Team, <zenoh@zettascale.tech>

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#undef NDEBUG
#include <assert.h>

#include "zenoh.h"

// fill v with invalid values
// set v to null
// check if v it is null
// make sure that drop on null does not crash
// make sure that double drop on null does not crash
// fill v with invalid values again
//
// set v1 to null
// move v to v1
// make sure that v is null now
//
// set v2 to null
// move v1 to v2 using z_take from mutable loaned
// make sure that v1 is null now
//
#define TEST(name)                   \
    {                                \
        name v;                      \
        memset(&v, -1, sizeof(v));   \
        z_internal_null(&v);         \
        assert(!z_check(v));         \
        z_drop(z_move(v));           \
        z_drop(z_move(v));           \
        name v1;                     \
        z_internal_null(&v1);        \
        memset(&v, -1, sizeof(v));   \
        z_take(&v1, z_move(v));      \
        assert(!z_check(v));         \
        name v2;                     \
        z_internal_null(&v2);        \
        memset(&v1, -1, sizeof(v1)); \
        z_take(&v2, z_loan_mut(v1)); \
        assert(!z_check(v1));        \
    }

int main(void) {
    TEST(z_owned_session_t)
    TEST(z_owned_keyexpr_t)
    TEST(z_owned_config_t)
    TEST(z_owned_hello_t)
    TEST(z_owned_closure_sample_t)
    TEST(z_owned_closure_query_t)
    TEST(z_owned_closure_reply_t)
    TEST(z_owned_closure_hello_t)
#if defined(Z_FEATURE_UNSTABLE_API_t)
    TEST(z_owned_closure_zid_t)
#endif
    TEST(z_owned_string_t)
    TEST(z_owned_string_array_t)
    TEST(z_owned_sample_t)
    TEST(z_owned_query_t)
    TEST(z_owned_slice_t)
    TEST(z_owned_bytes_t)
    TEST(z_owned_bytes_writer_t)
    TEST(z_owned_encoding_t)
    TEST(z_owned_publisher_t)
    TEST(z_owned_subscriber_t)
    TEST(z_owned_queryable_t)
    TEST(z_owned_reply_t)

    TEST(ze_owned_serializer_t)
    // Double drop not supported for these types
    // TEST(z_owned_task_t)
    // TEST(z_owned_mutex_t)
    // TEST(z_owned_condvar_t)

    return 0;
}