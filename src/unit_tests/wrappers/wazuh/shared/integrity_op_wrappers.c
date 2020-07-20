/* Copyright (C) 2015-2020, Wazuh Inc.
 * Copyright (C) 2009 Trend Micro Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

#include "integrity_op_wrappers.h"
#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

char * __wrap_dbsync_check_msg(const char * component, dbsync_msg msg, long id, const char * start, const char * top,
                                const char * tail, __attribute__((unused)) const char * checksum) {
    check_expected(component);
    check_expected(msg);
    check_expected(id);
    check_expected(start);
    check_expected(top);
    check_expected(tail);

    return mock_type(char*);
}
