
#ifndef _PASSE_1_
#define _PASSE_1_

#include <stdio.h>
#include "defs.h"
#include "arch.h"
#include "utils/miniccutils.h"

void analyse_passe_1(node_t root);
void analyse_program(node_t node);
void analyse_listdecl(node_t node,bool g);
void analyse_listdeclnonnull(node_t node,bool g);
void analyse_vardecl(node_t node,bool g);
void analyse_type(node_t node);
void analyse_listtypedecl(node_t node,bool g, node_type t);
void analyse_decl(node_t node, bool g, node_type t);
void analyse_func(node_t node, bool g);
void analyse_listinst(node_t node);
void analyse_listinstnonnull(node_t node);
void analyse_inst(node_t node);
void analyse_block(node_t node,bool g);
void analyse_expr(node_t node);
void analyse_listparamprint(node_t node);
void analyse_paramprint(node_t node);
void analyse_ident(node_t node);
void analyse_intval(node_t node);
void analyse_string(node_t node);
void analyse_bool(node_t node);

#endif

