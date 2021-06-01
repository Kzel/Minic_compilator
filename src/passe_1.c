
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "passe_1.h"
#include "common.h"

extern int trace_level;
extern int yylineno;
node_type actuelle_type;
bool glob;

void analyse_passe_1(node_t root){

    if(root){
        yylineno = root -> lineno;
        actuelle_type = root-> type;
        glob = false;

        switch (root -> nature){

            case NODE_PROGRAM:
                push_global_context();
                glob = true;
                printf_level(1,"NODE_PROGRAM\n");
                break;

            case NODE_LIST:
                printf_level(1,"NODE_LIST\n");
                break;

            case NODE_DECLS:
                printf_level(1,"NODE_DECLS\n");
                actuelle_type = root->opr[0]->type;
                if(root->opr[0]->type == TYPE_VOID){
                    fprintf(stderr,"Error line %d: Le type ne peut pas être void\n", root->lineno);
                }
                break;

            case NODE_DECL:
                printf_level(1,"NODE_DECL\n");
                printf("type:::::::::%d\n",actuelle_type);
                analyse_DECL(root,root->opr[0]->type);
                break;

            case NODE_FUNC:
                printf_level(1,"NODE_FUNC\n");
                reset_env_current_offset();
                reset_temporary_max_offset();
                set_max_registers(get_num_registers());
                root->offset=get_env_current_offset();
                printf_level(1, "offset:%d\n", root->offset);  
                break;

            case NODE_TYPE:
                printf_level(1,"NODE_TYPE\n");
                actuelle_type = root -> type;
                switch(actuelle_type){
                    case 0: 
                        printf_level(1,"'none'\n");
                        break;
                    case 1:
                        printf_level(1,"'int'\n");
                        break;
                    case 2:
                        printf_level(1,"'bool'\n");
                        break;
                    case 3:
                        printf_level(1,"'string'\n");
                        break;
                    case 4:
                        printf_level(1,"'void'\n");
                        break;
                }   
                break;
                

            case NODE_IDENT:
                printf_level(1,"NODE_IDENT\n");
                printf_level(1,"ident:'%s'\n",root->ident);
                if ((strcmp(root->ident,"main")!= 0)){
                    printf_level(1,"offset:'%d'\n",root->offset);   
                }
                analyse_IDEN(root);
                break;

            case NODE_BLOCK:
                printf_level(1,"NODE_BLOCK\n");
                push_context();
                glob = false;
                for (int i=0; i<root->nops; i++){
                    analyse_passe_1(root->opr[i]);
                }
                pop_context();
                glob = true;
                break;  

            case NODE_IF:
                printf_level(1,"NODE_IF\n");
                analyse_exp(root->opr[0]);
                if(root->opr[0]->type != TYPE_BOOL){
                    fprintf(stderr,"Error line %d: La condition pour if est fausse\n",root->opr[0]->lineno);
                    exit(1);
                }
                analyse_exp(root->opr[1]);
                analyse_exp(root->opr[2]);
                break;
                
            case NODE_WHILE:
                printf_level(1,"NODE_WHILE\n");
                analyse_exp(root->opr[0]);
                analyse_exp(root->opr[1]);
                if(root->opr[0]->type != TYPE_BOOL){
                    fprintf(stderr,"Error line %d: La condition pour while est fausse\n",root->opr[0]->lineno);
                    exit(1);
                }
                break;

            case NODE_DOWHILE:
                printf_level(1,"NODE_DOWHILE\n");
                analyse_exp(root->opr[0]);
                analyse_exp(root->opr[1]);
                if(root->opr[1]->type != TYPE_BOOL){
                    fprintf(stderr,"Error line %d: La condition pour dowhile est fausse\n",root->opr[1]->lineno);
                    exit(1);
                }
                break;

            case NODE_FOR:
                printf_level(1,"NODE_FOR\n");
                analyse_exp(root->opr[0]);
                analyse_exp(root->opr[1]);
                analyse_exp(root->opr[2]);
                analyse_exp(root->opr[4]);
                if(root->opr[1]->type != TYPE_BOOL){
                    fprintf(stderr,"Error line %d: La condition pour for est fausse\n",root->opr[1]->lineno);
                    exit(1);
                }
                break;

            case NODE_PRINT:
                printf_level(1,"NODE_PRINT\n");
                //retourner(root);
                break;

            
            /* Operateur */
            case NODE_PLUS:
                printf_level(1,"NODE_PLUS\n");
                analyse_exp(root);
                break;

            case NODE_MINUS:
                printf_level(1,"NODE_MINUS\n");
                analyse_exp(root);
                break;

            case NODE_MUL:
                printf_level(1,"NODE_MUL\n");
                analyse_exp(root);
                break;

            case NODE_DIV:
                printf_level(1,"NODE_DIV\n");
                analyse_exp(root);
                break;

            case NODE_MOD:
                printf_level(1,"NODE_MOD\n");
                analyse_exp(root);
                break;

            case NODE_UMINUS:
                printf_level(1,"NODE_UMINUS\n");
                analyse_exp(root);
                break;

            case NODE_LT:
                printf_level(1,"NODE_LT\n");
                analyse_exp(root);
                break;

            case NODE_GT:
                printf_level(1,"NODE_GT\n");
                analyse_exp(root);
                break;

            case NODE_LE:
                printf_level(1,"NODE_LE\n");
                analyse_exp(root);
                break;

            case NODE_GE:
                printf_level(1,"NODE_GE\n");
                analyse_exp(root);
                break;

            case NODE_EQ:
                printf_level(1,"NODE_EQ\n");
                analyse_exp(root);
                break;

            case NODE_NE:
                printf_level(1,"NODE_NE\n");
                analyse_exp(root);
                break;

            case NODE_AND:
                printf_level(1,"NODE_AND\n");
                analyse_exp(root);
                break;

            case NODE_OR:
                printf_level(1,"NODE_OR\n");
                analyse_exp(root);
                break;

            case NODE_BAND:
                printf_level(1,"NODE_BAND\n");
                analyse_exp(root);
                break;

            case NODE_BOR:
                printf_level(1,"NODE_BOR\n");
                analyse_exp(root);
                break;

            case NODE_BXOR:
                printf_level(1,"NODE_BXOR\n");
                analyse_exp(root);
                break;

            case NODE_SRL:
                printf_level(1,"NODE_SRL\n");
                analyse_exp(root);
                break;

            case NODE_SRA:
                printf_level(1,"NODE_SRA\n");
                analyse_exp(root);
                break;

            case NODE_NOT:
                printf_level(1,"NODE_NOT\n");
                analyse_exp(root);
                break;

            case NODE_BNOT:
                printf_level(1,"NODE_BNOT\n");
                analyse_exp(root);
                break;

            case NODE_AFFECT:
                printf_level(1,"NODE_AFFECT\n");
                analyse_exp(root->opr[1]);
                if(root->opr[1]->nature==NODE_AFFECT){
                    if(root->opr[0]->type!=root->opr[1]->opr[1]->type){
                        fprintf(stderr,"Error line %d: Type n'est pas coherent\n",root->opr[0]->lineno);
                    }

                }else{
                    if(root->opr[0]->type!=root->opr[1]->type){
                        fprintf(stderr,"Error line %d: Type n'est pas coherent\n",root->opr[1]->lineno);
                    }    
                }

                root->type=root->opr[0]->type;
                break;

            case NODE_INTVAL:
                printf_level(1,"NODE_INTVAL\n");
                printf_level(1, "%ld\n", root->value);
                break;
                
            case NODE_BOOLVAL:
                printf_level(1,"NODE_BOOLVAL\n");
                printf_level(1, "%ld\n", root->value);
                break;

            case NODE_STRINGVAL:
                printf_level(1,"NODE_STRINGVAL\n");
                printf("STRING: %s\n",root->str);
                root->offset = add_string(root->str);
                break;


            default:
                break;
        }
        for (int i=0; i<root->nops; i++){
            analyse_passe_1(root->opr[i]);
        }
        
    }

}

/*void retourner(node_t node){
    if ( node->nops > 0){
    	int i;
    	for (i = 0; i < node->nops; i++){
            analyse_passe_1(node->opr[i]);
        }
    }
}*/

void analyse_DECL(node_t node,node_type type){

    int offset_decl = env_add_element(node -> opr[0] -> ident, node);
    if(offset_decl<0){
        fprintf(stderr,"Erreur line %d: La variable du même nom existe déjà",node->lineno);
    }else{
        node->opr[0]->offset = offset_decl;
        node->opr[0]->decl_node = NULL; 
        node->opr[0]->type = type;
        printf("type.....................%s\n",node_type2string(node->opr[0]->type));
    }
    
}



void analyse_IDEN(node_t node){
    if(strcmp(node->ident,"main")!= 0){
        node_t tmp = (node_t)get_decl_node(node->ident);
    
        if(tmp == NULL){
            fprintf(stderr,"Error line %d: L'identification ne peut pas être null\n", node->lineno);
        }else{
            node->type = tmp->type;
        }
    }
}

void analyse_exp(node_t node){
    if(node->nature == NODE_IDENT){
        printf("analyse_exp %s\n", node->ident);
        node_t tmp = (node_t)get_decl_node(node->ident);
        if(tmp == NULL){
            fprintf(stderr,"Error line %d: La variable %s n'est pas declaree\n", node->lineno, node->ident);
        }else{
            printf("type:%s\n",node_type2string(tmp->type));
            node -> type = tmp->type;    
        }
    }else if(node->nops == 1){
        analyse_exp(node->opr[0]);
    }
    else if(node->nops == 2){
        analyse_exp(node->opr[0]);
        analyse_exp(node->opr[1]);
    }

    node_nature nature = node->nature;
    if(nature==NODE_PLUS || nature==NODE_MINUS || nature==NODE_MUL || 
    nature==NODE_DIV || nature==NODE_MOD || nature==NODE_BAND || nature==NODE_BOR || 
    nature==NODE_BXOR || nature==NODE_SLL || nature==NODE_SRL || nature==NODE_SRA){
        if(node->opr[0]->type==TYPE_INT && node->opr[1]->type==TYPE_INT){
            
            node->type =TYPE_INT;
        }
        else{
            fprintf(stderr,"Error line %d: Le type de l'opération n'est pas correcte\n",node->lineno);
            exit(1);
        }
    }

    else if(nature==NODE_EQ || nature==NODE_NE || nature==NODE_LT || 
    nature==NODE_GT || nature==NODE_LE || nature==NODE_GE){
        printf("***********************************%d\n",node->opr[0]->type);
        printf("***********************************%d\n",node->opr[1]->type);
        if(node->opr[0]->type==TYPE_INT && node->opr[1]->type==TYPE_INT){
            node->type =TYPE_BOOL;
        }
        else{
            fprintf(stderr,"Error line %d: Le type de l'opération n'est pas correcte\n",node->lineno);
            exit(1);
        }
    }

    else if(nature==NODE_AND || nature==NODE_OR || nature==NODE_EQ || nature==NODE_NE){
        if(node->opr[0]->type==TYPE_BOOL && node->opr[1]->type==TYPE_BOOL){
            node->type =TYPE_BOOL;
        }
        else{
            fprintf(stderr,"Error line %d: Le type de l'opération n'est pas correcte\n",node->lineno);
            exit(1);
        }
    }
    /*else if(nature==NODE_AFFECT){
        printf("-----------------------------------------%d\n",nature);
        if(node->opr[0]->type!=node->opr[1]->opr[1]->type){
            fprintf(stderr,"Error line %d: Type n'est pas coherent\n",node->opr[0]->lineno);
        }
        else if(node->opr[0]->type!=node->opr[1]->type){
            fprintf(stderr,"Error line %d: Type n'est pas coherent\n",node->opr[1]->lineno);
        }    
        node->type=node->opr[0]->type;
    }*/
}


