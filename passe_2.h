#ifndef _PASSE_2_H_
#define _PASSE_2_H_

#include <stdio.h>
#include "defs.h"
#include "arch.h"
#include "common.h"
#include "utils/miniccutils.h"

void gen_code_passe_2(node_t root);
void node_traite(node_t root);
void node_suivant(node_t root);
void gen_code_decl(node_t root);
void gen_code_func(node_t root);
void gen_code_print(node_t root);
void gen_code_ope(node_t root);
void gen_code_affect(node_t root);

#endif

