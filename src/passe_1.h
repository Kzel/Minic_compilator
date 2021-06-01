
#ifndef _PASSE_1_
#define _PASSE_1_

#include <stdio.h>
#include "defs.h"
#include "arch.h"
#include "utils/miniccutils.h"

void analyse_passe_1(node_t root);
void analyse_block(node_t node);
//void retourner(node_t node);
void analyse_DECLS(node_t node);
void analyse_DECL(node_t node,node_type type);
void analyse_IDEN(node_t node);
//void analyse_type(node_t node, node_type t)
void analyse_exp(node_t node);
#endif

