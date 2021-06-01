
#include <stdio.h>
#include "passe_2.h"
bool global=true;
bool fin=false;
int act_offset=0;


void gen_code_passe_2(node_t root) {
    if(root){
        node_suivant(root);
    }
    create_inst_addiu(29, 29, act_offset);
    create_inst_ori(2, get_r0(), 10);
	create_inst_syscall();
    
}


void debut_fin(node_t root){
    if(fin==false && root!=NULL){
        switch (root->nature){
        case NODE_PROGRAM:
            
            create_inst_data_sec();
            break;

        case NODE_LIST: break;   

        case NODE_DECL:
            gen_code_decl(root);
            break;

        case NODE_FUNC:
            create_inst_text_sec();
            set_temporary_start_offset(root->offset);
        	set_max_registers(get_num_registers());
            int32_t strnum= get_global_strings_number();
            for (int i = 0; i <strnum; i++){	
				create_asciiz_inst(NULL, get_global_string(i));
			}
            create_inst_label_str("main");
            create_inst_addiu(29, 29, -root->offset);
            global=false;
            break;

        case NODE_TYPE: break;

        case NODE_IDENT:break;
            
        case NODE_PRINT:
            gen_code_print(root);
            break;

        case NODE_INTVAL:
            break;

        case NODE_STRINGVAL:
            create_inst_lui(4, 0x1001);
            create_inst_ori(4, 4, root->offset);
            create_inst_ori(2, get_r0(), 4);
            create_inst_syscall();
            break;
          
         case NODE_AFFECT:
            if(fin == false && global == false){   
                create_inst_ori(get_current_reg(), get_r0(), root->opr[1]->value);
                create_inst_sw(get_current_reg(), root->opr[0]->offset, 29);
            }
            break;

        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_UMINUS:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
        case NODE_LT:
        case NODE_GT:
        case NODE_LE:
        case NODE_GE:
        case NODE_EQ:
        case NODE_NE:
        case NODE_AND:
        case NODE_OR:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SRL:
        case NODE_SRA:
        case NODE_NOT:
        case NODE_BNOT:
            gen_code_ope(root);
            break;
        default:
            break;
        
      }
    }
}

void node_suivant(node_t root){

    for(int i=0 ; i< root->nops; i++){
        if(root->opr[i] != NULL){      
                    fin= false;
                    debut_fin(root->opr[i]);
                    node_suivant(root->opr[i]);
                    fin= true;
                    debut_fin(root->opr[i]);  

        }
    }

}

void gen_code_decl(node_t root){
    if(fin == false){

        if(global==true && root->opr[0]->nature ==NODE_IDENT){
                create_inst_word(root->opr[0]->ident, root->opr[1]->value);
                
            

        }else if(global==true && root->opr[1]->nature ==NODE_IDENT){
                create_inst_word(root->opr[1]->ident, root->opr[0]->value);
               
            
        }
        else if(global == false && root->opr[0]->nature == NODE_IDENT){

                act_offset+=4;
                create_inst_ori(get_current_reg(), get_r0(), root->opr[1]->value);
                create_inst_sw(get_current_reg(), root->opr[0]->offset, 29);

        }else if(global == false &&  root->opr[1]->nature == NODE_IDENT){

                act_offset+=4;
                create_inst_ori(get_current_reg(), get_r0(), root->opr[0]->value);
                create_inst_sw(get_current_reg(), root->opr[1]->offset, 29);
                
        }
    }
}

void gen_code_print(node_t root){

    if(fin==false){
        
        create_inst_lui(4,0x1001);
      	create_inst_ori(4, 4, root->offset);
      	create_inst_ori(2, get_r0(), 4);
      	create_inst_syscall();
    }
}

void gen_code_ope(node_t root){
    if(fin == false && root->opr[0]->nature == NODE_INTVAL){
            int32_t reg0=get_current_reg();
            create_inst_ori(reg0, get_r0(), root->opr[0]->value);
            if(root->opr[1]->nature == NODE_INTVAL){

                allocate_reg();
                int32_t reg1=get_current_reg();
                create_inst_ori(reg1, get_r0(), root->opr[1]->value);
                switch(root->nature){
                    case NODE_PLUS:
                        create_inst_addu(reg0, reg0, reg1);
                        break;
                    case NODE_DIV:
                        create_inst_div(reg0, reg1);
                        create_inst_teq(reg1, 0);
                        create_inst_mflo(reg0);
                        break;

                    case NODE_MINUS:
                        create_inst_subu(reg0, reg0, reg1);
                        break;

                    case NODE_MUL:
                        create_inst_mult(reg0, reg1);

                        break;
                    case NODE_MOD:
                        create_inst_div(reg0, reg1);
                        create_inst_teq(reg1, 0);
                        create_inst_mfhi(reg0);
                        break;

                    case NODE_BXOR:
                        create_inst_xor(reg0, reg0, reg1);
                        break;    
                    
                    case NODE_BAND:
                        create_inst_and(reg0, reg0, reg1);
                        break;  
                    case NODE_BOR:
                        create_inst_or(reg0, reg0, reg1);
                        break;    

                    case NODE_SRL:
                        create_inst_srlv(reg0, reg0, reg1);
                        break;

                    case NODE_SRA:
                        create_inst_srav(reg0, reg0, reg1);
                        break;   

                    case NODE_SLL:
                        create_inst_sllv(reg0, reg0, reg1);
                        break;       



                }
                release_reg();
                push_temporary(reg0);
        }
    }
}

