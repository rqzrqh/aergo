/**
 * @file    check.h
 * @copyright defined in aergo/LICENSE.txt
 */

#ifndef _CHECK_H
#define _CHECK_H

#include "common.h"

#include "ast.h"

#ifndef _AST_BLK_T
#define _AST_BLK_T
typedef struct ast_blk_s ast_blk_t;
#endif /* ! _AST_BLK_T */

#ifndef _AST_ID_T
#define _AST_ID_T
typedef struct ast_id_s ast_id_t;
#endif /* ! _AST_ID_T */

typedef struct check_s {
    ast_t *ast;
    ast_blk_t *root;

    flag_t flag;

    /* temporary context */
    ast_blk_t *blk;         /* current block */

    ast_id_t *cont_id;      /* current contract */
    ast_id_t *qual_id;      /* current access qualifier */
    ast_id_t *func_id;      /* current function */

    int mem_addr;           /* current address of linear memory */
    int var_idx;            /* current index of local variable */
} check_t;

void check(ast_t *ast, flag_t flag);

#endif /* ! _CHECK_H */
