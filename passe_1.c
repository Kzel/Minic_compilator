
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "passe_1.h"

extern int trace_level;
extern int yylineno;
node_type actuelle_type=0;
bool glob=false;
char mess[100];

void analyse_passe_1(node_t root){

    analyse_program(root);

}

void analyse_program(node_t node){
    push_global_context();
    glob = true;
    printf_level(1, "NODE_PROGRAM\n");
    if(node->opr[0]){

        analyse_listdeclnonnull(node->opr[0], glob);

    }

    analyse_func(node->opr[1], glob);
}

void analyse_listdecl(node_t node, bool g){

    if(node){

        analyse_listdeclnonnull(node, g);

    }

}

void analyse_listdeclnonnull(node_t node, bool g){

    if(node->nature == NODE_LIST){

        printf_level(1, "NODE_LIST\n");
        analyse_listdeclnonnull(node->opr[0], g);
        analyse_vardecl(node->opr[1], g);

    } else {

        analyse_vardecl(node, g);

    }
}

void analyse_vardecl(node_t node, bool g){

    printf_level(1, "NODE_DECLS\n");
    actuelle_type = node->opr[0]->type;

    if(actuelle_type != TYPE_VOID){

        analyse_type(node->opr[0]);
        analyse_listtypedecl(node->opr[1], g, actuelle_type);

    } else {

        fprintf(stderr, "Error line %d: Le type ne peut pas être void\n", node->lineno);
        exit(1);

    }
}

void analyse_type(node_t node){

    printf_level(1, "NODE_TYPE\n");
    node_type t = node -> type;

    switch(t){

        case 0: 
            printf_level(1, "'none'\n");
            break;
        case 1:
            printf_level(1, "'int'\n");
            break;
        case 2:
            printf_level(1, "'bool'\n");
            break;
        case 3:
            printf_level(1, "'string'\n");
            break;
        case 4:
            printf_level(1, "'void'\n");
            break;

    }   
}

void analyse_listtypedecl(node_t node, bool g, node_type t){

    if(node->nature == NODE_LIST){
        printf_level(1, "NODE_LIST\n");
        analyse_listtypedecl(node->opr[0], g, t);
        analyse_decl(node->opr[1], g, t);

    } else {

        analyse_decl(node, g, t);

    }
}

void analyse_decl(node_t node, bool g, node_type t){

    printf_level(1, "NODE_DECL\n");
    int offset_decl = env_add_element(node->opr[0]->ident, node);

    if(offset_decl < 0){

        fprintf(stderr, "Erreur line %d: La variable du même nom existe déjà", node->lineno);
        exit(1);

    } else {

        node->opr[0]->offset = offset_decl;
        node->opr[0]->decl_node = NULL; 
        node->opr[0]->type = t;
        analyse_ident(node->opr[0]);

        if(node->opr[1]){
            analyse_expr(node->opr[1]);
        }

    }
}

void analyse_func(node_t node, bool g){

    printf_level(1, "NODE_FUNC\n");

    if(strcmp(node->opr[1]->ident, "main") == 0 && (node->opr[0]->type == TYPE_VOID)){

        reset_env_current_offset();
        node->offset=get_env_current_offset();
        printf_level(1, "offset:%d\n", node->offset);
        analyse_type(node->opr[0]);
        analyse_ident(node->opr[1]);
        analyse_block(node->opr[2], g);

    } else {

        fprintf(stderr, "Error line %d: Le type doit être void\n", node->lineno);
        exit(1);

    }
}

void analyse_listinst(node_t node){

    if(node){

        analyse_listinstnonnull(node);

    }
}

void analyse_listinstnonnull(node_t node){
    if(node->nature == NODE_LIST){

        printf_level(1, "NODE_LIST\n");
        analyse_listinstnonnull(node->opr[0]);
        analyse_inst(node->opr[1]);

    } else {

        analyse_inst(node);

    }
}

void analyse_inst(node_t node){

    if(node->nops >= 1){

        switch(node->nature){

            case NODE_IF:
                printf_level(1, "NODE_IF\n");

                if(node->nops == 3){

                    analyse_expr(node->opr[0]);
                    analyse_inst(node->opr[1]);
                    analyse_inst(node->opr[2]);

                }else if(node->nops == 2){

                    analyse_expr(node->opr[0]);
                    analyse_inst(node->opr[1]);

                }

                if(node->opr[0]->type != TYPE_BOOL){

                    fprintf(stderr, "Error line %d: La condition pour if est fausse\n", node->opr[0]->lineno);
                    exit(1);

                }
                break;

            case NODE_FOR:

                printf_level(1,"NODE_FOR\n");
                analyse_expr(node->opr[0]);
                analyse_expr(node->opr[1]);
                analyse_expr(node->opr[2]);
                analyse_inst(node->opr[3]);

                if(node->opr[1]->type != TYPE_BOOL){

                    fprintf(stderr, "Error line %d: La condition pour for est fausse\n", node->opr[1]->lineno);
                    exit(1);

                }
                break;

            case NODE_WHILE:

                printf_level(1, "NODE_WHILE\n");
                analyse_expr(node->opr[0]);
                analyse_inst(node->opr[1]);

                if(node->opr[0]->type != TYPE_BOOL){

                    fprintf(stderr, "Error line %d: La condition pour while est fausse\n", node->opr[0]->lineno);
                    exit(1);

                }
                break;

            case NODE_DOWHILE:

                printf_level(1, "NODE_DOWHILE\n");
                analyse_inst(node->opr[0]);
                analyse_expr(node->opr[1]);

                if(node->opr[1]->type != TYPE_BOOL){

                    fprintf(stderr, "Error line %d: La condition pour dowhile est fausse\n", node->opr[1]->lineno);
                    exit(1);

                }
                break;   
                
            case NODE_PRINT:

                printf_level(1, "NODE_PRINT\n");
                analyse_listparamprint(node->opr[0]);
                break;

            case NODE_BLOCK:
                analyse_block(node, glob);
        }        
    } else {

        analyse_expr(node);

    }
}

void analyse_block(node_t node, bool g){

    printf_level(1, "NODE_BLOCK\n");
    push_context();
    g = false;
    analyse_listdecl(node->opr[0], g);
    analyse_listinst(node->opr[1]);
    pop_context();
    g = true;

}

void analyse_expr(node_t node){

    if(node->nops == 1){

        analyse_expr(node->opr[0]);

    }
    else if(node->nops == 2){

        analyse_expr(node->opr[0]);
        analyse_expr(node->opr[1]);

    }

    node_nature nature = node->nature;

    if(nature==NODE_PLUS || nature==NODE_MUL || nature==NODE_DIV || nature==NODE_MOD || nature==NODE_BAND 
    || nature==NODE_BOR || nature==NODE_BXOR || nature==NODE_SLL || nature==NODE_SRL || nature==NODE_SRA){
        
        printf_level(1, "NODE_%s\n",node_nature2string(nature));

        if(node->opr[0]->type == TYPE_INT && node->opr[1]->type == TYPE_INT){ 

            node->type = TYPE_INT;

        } else {

            fprintf(stderr, "Error line %d: Le type de %s et %s ne sont pas identiques\n", node->lineno,node->opr[0]->ident,node->opr[1]->ident);
            exit(1);

        }
    }
    else if( nature == NODE_LT || nature == NODE_GT || nature == NODE_LE || nature == NODE_GE){

        printf_level(1, "NODE_%s\n", node_nature2string(nature));

        if(node->opr[0]->type == TYPE_INT && node->opr[1]->type == TYPE_INT){

            node->type =TYPE_BOOL;

        } else {

            fprintf(stderr, "Error line %d: Le type de %s et %s ne sont pas identiques\n", node->lineno,node->opr[0]->ident,node->opr[1]->ident);
            exit(1);

        }
    }

    else if(nature == NODE_AND || nature == NODE_OR){

        printf_level(1, "NODE_%s\n", node_nature2string(nature));

        if(node->opr[0]->type == TYPE_BOOL && node->opr[1]->type == TYPE_BOOL){

            node->type = TYPE_BOOL;

        } else {

            fprintf(stderr, "Error line %d: Le type de %s et %s ne sont pas identiques\n", node->lineno,node->opr[0]->ident,node->opr[1]->ident);
            exit(1);

        }
    }
    else if(nature == NODE_EQ || nature == NODE_NE){

        printf_level(1, "NODE_%s\n", node_nature2string(nature));

        if(node->opr[0]->type == TYPE_BOOL && node->opr[1]->type == TYPE_BOOL){

            node->type = TYPE_BOOL;

        }
        else if(node->opr[0]->type == TYPE_INT && node->opr[1]->type == TYPE_INT){

            node->type = TYPE_BOOL;

        } else {

            fprintf(stderr, "Error line %d: Le type de %s et %s ne sont pas identiques\n", node->lineno,node->opr[0]->ident,node->opr[1]->ident);
            exit(1);

        }
    }
    else if(nature == NODE_MINUS){

        printf_level(1, "NODE_%s\n", node_nature2string(nature));

        if(node->nops == 2){

            if(node->opr[0]->type == TYPE_INT && node->opr[1]->type == TYPE_INT){

                node->type = TYPE_INT;

            } else {

                fprintf(stderr, "Error line %d: Le type de %s et %s ne sont pas identiques\n", node->lineno,node->opr[0]->ident,node->opr[1]->ident);
                exit(1);

            }
        }
        else if(node->nops == 1){

            if(node->opr[0]->type == TYPE_INT){

                node->type = TYPE_INT;

            } else {

                fprintf(stderr, "Error line %d: Le type de %s doit être en int\n", node->lineno,node->opr[0]->ident);
                exit(1);

            }
        }  
    }
    else if(nature == NODE_BNOT){

        printf_level(1, "NODE_%s\n", node_nature2string(nature));

        if(node->opr[0]->type == TYPE_INT){

            node->type = TYPE_INT;

        } else {

            fprintf(stderr, "Error line %d: Le type de %s doit être en int\n", node->lineno,node->opr[0]->ident);
            exit(1);

        }
    }
    else if(nature == NODE_NOT){

        printf_level(1, "NODE_%s\n", node_nature2string(nature));
        if(node->opr[0]->type == TYPE_BOOL){

            node->type = TYPE_BOOL;

        } else {

            fprintf(stderr, "Error line %d: Le type de %s doit être en bool\n", node->lineno,node->opr[0]->ident);
            exit(1);

        }
    }
    else if(nature == NODE_AFFECT){

        printf_level(1, "NODE_AFFECT\n");
        analyse_ident(node->opr[0]);
        analyse_expr(node->opr[1]);

        if(node->opr[1]->nature == NODE_AFFECT){

            if(node->opr[0]->type != node->opr[1]->opr[1]->type){

                fprintf(stderr, "Error line %d: Type n'est pas coherent\n", node->opr[0]->lineno);
                exit(1);

            }

        } else {

            if(node->opr[0]->type != node->opr[1]->type){

                fprintf(stderr, "Error line %d: Type n'est pas coherent\n", node->opr[1]->lineno);
                exit(1);

            }    
        }
        node->type = node->opr[0]->type;
    }
    else if(nature == NODE_INTVAL){

        analyse_intval(node);

    }
    else if(nature == NODE_BOOLVAL){

        analyse_bool(node);

    }
    else if(node->nature == NODE_IDENT){

        printf_level(1, "NODE_IDENT\n");

        if(strcmp(node->ident,"main") !=  0){

            node_t tmp = (node_t)get_decl_node(node->ident);

            if(tmp == NULL){

                fprintf(stderr, "Error line %d: La variable %s n'est pas declaree\n", node->lineno, node->ident);
                exit(1);

            } else {

                node->type = tmp->type;
                printf_level(1, "ident:'%s'\n", node->ident); 
                printf_level(1, "decl:'%d'\n", tmp->offset); 

            }
        } else {

            printf_level(1, "ident:'%s'\n", node->ident);

        }
    } else {

        analyse_expr(node);

    }
}

void analyse_listparamprint(node_t node){

    if(node->nature == NODE_LIST){

        printf_level(1, "NODE_LIST\n");
        analyse_listparamprint(node->opr[0]);
        analyse_paramprint(node->opr[1]);

    } else {
        analyse_paramprint(node);
    }
}

void analyse_paramprint(node_t node){

    if(node->nature == NODE_STRINGVAL){
        analyse_string(node);

    } else {

        analyse_ident(node);

    }
}

void analyse_ident(node_t node){

    printf_level(1 ,"NODE_IDENT\n");

    if(strcmp(node->ident,"main") != 0){

        node_t tmp = (node_t)get_decl_node(node->ident);

        if(tmp == NULL){

            fprintf(stderr, "Error line %d: La variable %s n'est pas declaree\n", node->lineno, node->ident);
            exit(1);

        } else {

            tmp->type = node->type;
            tmp->offset = node->offset;
            node->decl_node = tmp;
            printf_level(1, "ident:'%s'\n", node->ident); 
            printf_level(1, "offset:'%d'\n", node->offset); 

        }
    } else {
        printf_level(1, "ident:'%s'\n", node->ident);
    }        
}

void analyse_intval(node_t node){

    printf_level(1, "NODE_INTVAL\n");
    printf_level(1, "%ld\n", node->value);

}

void analyse_string(node_t node){

    printf_level(1, "NODE_STRINGVAL\n");
    printf_level(1, "%s\n", node->str);
    node->offset = add_string(node->str);

}

void analyse_bool(node_t node){
    printf_level(1, "NODE_BOOLVAL\n");
    printf_level(1, "%ld\n", node->value);
}
