/*
 * Copyright (c) 2017, 2020 ADLINK Technology Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
 * which is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *
 * Contributors:
 *   ADLINK zenoh team, <zenoh@adlink-labs.tech>
 */
#include "zenoh/net.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char *uri = "/demo/example/zenoh-c-eval";
char *value = "Eval from C!";

void query_handler(ZNQuery *query, const void *arg) {
    const zn_string *res = zn_query_res_name(query);
    const zn_string *pred = zn_query_predicate(query);
    printf(">> [Query handler] Handling '%.*s?%.*s'\n", res->len, res->val, pred->len, pred->val);
    zn_send_reply(query, uri, (const unsigned char *)value, strlen(value));
}

int main(int argc, char** argv) {
    if (argc > 1) {
        uri = argv[1];
    }
    ZNProperties *config = zn_config_peer();
    if (argc > 2) {
        zn_properties_add(config, ZN_PEER_KEY, argv[2]);
    }

    printf("Openning session...\n");
    ZNSession *s = zn_open(config);
    if (s == 0) {
        printf("Unable to open session!\n");
        exit(-1);
    }

    printf("Declaring Queryable on '%s'...\n", uri);
    ZNQueryable *qable = zn_declare_queryable(s, zn_rname(uri), EVAL, query_handler, NULL);
    if (qable == 0) {
        printf("Unable to declare queryable.\n");
        exit(-1);
    }
    
    char c = 0;
    while (c != 'q') {
        c = fgetc(stdin);
    }

    zn_undeclare_queryable(qable);
    zn_close(s);
    return 0;    
}
