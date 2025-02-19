#include "parserDef.h"
#include <stdio.h>
#include <stdlib.h>




int main (){
    RuleList* rl = readRules("final_grammar_index_clean.txt");
    for (int i=0; i<rl->num_rules; i++){
        printf("%d %d -> ", rl->rules[i].num_rhs, rl->rules[i].lhs);
        for (int j=0; j<rl->rules[i].num_rhs; j++){
            if (rl->rules[i].rhs[j].is_terminal){
                printf("%d ", rl->rules[i].rhs[j].symbol.t);
            }
            else{
                printf("%d ", rl->rules[i].rhs[j].symbol.nt);
            }
        }
        printf("\n");
    }
    return 0;
}