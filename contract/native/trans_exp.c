/**
 * @file    trans_exp.c
 * @copyright defined in aergo/LICENSE.txt
 */

#include "common.h"

#include "ast_id.h"
#include "ast_blk.h"
#include "ast_stmt.h"
#include "ir_bb.h"
#include "ir_fn.h"
#include "ir_sgmt.h"

#include "trans_exp.h"

static void
exp_trans_lit(trans_t *trans, ast_exp_t *exp)
{
    int addr;
    value_t *val = &exp->u_lit.val;
    ir_sgmt_t *sgmt = &trans->ir->sgmt;

    switch (val->type) {
    case TYPE_BOOL:
    case TYPE_UINT64:
    case TYPE_DOUBLE:
        break;

    case TYPE_STRING:
        addr = sgmt_add_raw(sgmt, val_ptr(val), val_size(val) + 1);
        value_set_i64(val, addr);
        break;

    case TYPE_OBJECT:
        if (is_null_val(val))
            addr = sgmt_add_raw(sgmt, "\0\0\0\0", 4);
        else
            addr = sgmt_add_raw(sgmt, val_ptr(val), val_size(val));
        value_set_i64(val, addr);
        break;

    default:
        ASSERT1(!"invalid value", val->type);
    }
}

static void
exp_trans_id(trans_t *trans, ast_exp_t *exp)
{
    ast_id_t *id = exp->id;

    ASSERT(id != NULL);

    if (is_var_id(id)) {
        if (is_global_id(id))
            /* The global variable always refers to the memory */
            exp_set_stack(exp, id->meta.base_idx, id->meta.rel_addr, id->meta.rel_offset);
        else if (is_out_param(id))
            /* The out parameters always refers to the memory */
            exp_set_stack(exp, id->idx, 0, 0);
        else
            exp_set_local(exp, id->idx);
    }
    else if (is_fn_id(id) || is_cont_id(id)) {
        /* In the case of a contract identifier, the "this" syntax is used */
        exp_set_local(exp, trans->fn->cont_idx);
        exp->u_local.type = TYPE_UINT32;
    }
}

static void
exp_trans_array(trans_t *trans, ast_exp_t *exp)
{
    ast_id_t *id = exp->id;
    ast_exp_t *id_exp = exp->u_arr.id_exp;
    ast_exp_t *idx_exp = exp->u_arr.idx_exp;

    exp_trans(trans, id_exp);
    exp_trans(trans, idx_exp);

    if (is_array_meta(&id->meta)) {
        uint32_t offset;

        /* In array expression, the offset is calculated as follows:
         *
         * Suppose that "int i[x][y][z]" is defined.
         *
         * First, when we access "i[a]", the formula for calculating the offset is
         * (a * y * z * sizeof(int)).
         *
         * Next, in the case of "i[a][b]",
         * (a * y * z * sizeof(int)) + (b * z * sizeof(int)).
         *
         * Finally, in the case of "i[a][b][c]",
         * (a * y * z * sizeof(int)) + (b * z * sizeof(int)) + (c * sizeof(int)). */

        /* If "id_exp" is a call expression, it can be a stack expression */
        ASSERT1(is_stack_exp(id_exp) || is_local_exp(id_exp), id_exp->kind);

        if (!is_lit_exp(idx_exp))
            /* We must dynamically determine the address and offset */
            return;

        /* The following meta_size() is stripped size of array */
        offset = val_i64(&idx_exp->u_lit.val) * meta_size(&exp->meta);

        if (is_stack_exp(id_exp))
            exp_set_stack(exp, id_exp->u_stk.base, id_exp->u_stk.addr,
                          id_exp->u_stk.offset + offset);
        else
            exp_set_stack(exp, id_exp->u_local.idx, 0, offset);

        if (is_array_meta(&exp->meta))
            exp->u_stk.type = TYPE_UINT32;
    }
    else {
        /* TODO
         * int addr = fn_add_stack_var(trans->fn);
         * ast_exp_t *call_exp = exp_new_call("$map_get", &exp->pos);
         *
         * bb_add_stmt(trans->bb, stmt_new_exp(call_exp, &exp->pos));
         *
         * return <return address of call>; */
    }
}

static void
exp_trans_cast(trans_t *trans, ast_exp_t *exp)
{
    exp_trans(trans, exp->u_cast.val_exp);

    if (is_string_meta(&exp->meta) || is_string_meta(&exp->u_cast.to_meta)) {
        /* TODO
         * int addr = fn_add_stack_var(trans->fn);
         * ast_exp_t *call_exp = exp_new_call("$concat", &exp->pos);
         *
         * bb_add_stmt(trans->bb, stmt_new_exp(call_exp, &exp->pos));
         *
         * return <return address of call>; */
    }
}

static void
exp_trans_unary(trans_t *trans, ast_exp_t *exp)
{
    ast_exp_t *val_exp = exp->u_un.val_exp;
    ast_exp_t *var_exp, *bi_exp, *lit_exp;

    switch (exp->u_un.kind) {
    case OP_INC:
    case OP_DEC:
        /* Clone value expression because we have to transform it to "x op 1" */
        var_exp = exp_clone(val_exp);

        exp_trans(trans, var_exp);
        exp_trans(trans, val_exp);

        lit_exp = exp_new_lit_i64(1, &exp->pos);
        meta_copy(&lit_exp->meta, &val_exp->meta);

        bi_exp = exp_new_binary(exp->u_un.kind == OP_INC ? OP_ADD : OP_SUB, val_exp,
                                lit_exp, &exp->pos);
        meta_copy(&bi_exp->meta, &val_exp->meta);

        if (exp->u_un.is_prefix)
            bb_add_stmt(trans->bb, stmt_new_assign(var_exp, bi_exp, &exp->pos));
        else
            /* The postfix operator is added as a piggybacked statement since it must
             * be executed after the current statement is executed */
            bb_add_piggyback(trans->bb, stmt_new_assign(var_exp, bi_exp, &exp->pos));

        *exp = *val_exp;
        break;

    case OP_NEG:
    case OP_NOT:
        exp_trans(trans, val_exp);
        break;

    default:
        ASSERT1(!"invalid operator", exp->u_un.kind);
    }
}

static void
exp_trans_binary(trans_t *trans, ast_exp_t *exp)
{
    exp_trans(trans, exp->u_bin.l_exp);
    exp_trans(trans, exp->u_bin.r_exp);

    if (exp->u_bin.kind == OP_ADD && is_string_meta(&exp->meta)) {
        /* TODO
         * int addr = fn_add_stack();
         * ast_exp_t *call exp = exp_new_call("$concat", &exp->pos);
         * bb_add_stmt(trans->bb, stmt_new_exp(call_exp, &exp->pos));
         * return exp_new_stack(addr, offset, &exp->pos);
        */
    }
}

static void
exp_trans_ternary(trans_t *trans, ast_exp_t *exp)
{
    exp_trans(trans, exp->u_tern.pre_exp);
    exp_trans(trans, exp->u_tern.in_exp);
    exp_trans(trans, exp->u_tern.post_exp);

    if (is_lit_exp(exp->u_tern.pre_exp)) {
        /* Maybe we should do this in optimizer (if exists) */
        meta_t meta = exp->meta;

        if (val_bool(&exp->u_tern.pre_exp->u_lit.val))
            *exp = *exp->u_tern.in_exp;
        else
            *exp = *exp->u_tern.post_exp;

        meta_copy(&exp->meta, &meta);
    }
}

static void
exp_trans_access(trans_t *trans, ast_exp_t *exp)
{
    ast_exp_t *qual_exp = exp->u_acc.qual_exp;
    //ast_id_t *qual_id = qual_exp->id;
    ast_id_t *fld_id = exp->id;

    exp_trans(trans, qual_exp);

    if (is_fn_id(fld_id)) {
        /* It may be a stack expression, in the case of an access expression to the
         * return value of a function */
        if (is_stack_exp(qual_exp)) {
            exp_set_local(exp, qual_exp->u_stk.base);
            exp->u_local.type = TYPE_UINT32;
        }
        return;
    }

    if (is_local_exp(qual_exp)) {
        exp_set_stack(exp, qual_exp->u_local.idx, 0, fld_id->meta.rel_offset);
    }
    else {
        ASSERT1(is_stack_exp(qual_exp), qual_exp->kind);

        exp_set_stack(exp, qual_exp->u_stk.base, qual_exp->u_stk.addr,
                      qual_exp->u_stk.offset + fld_id->meta.rel_offset);
    }
}

#if 0
static ast_exp_t *
make_return_exp(ast_id_t *ret_id, int stack_idx)
{
    ast_exp_t *addr_exp = exp_new_local(TYPE_UINT32, stack_idx);

    if (ret_id->meta.rel_addr > 0) {
        ast_exp_t *v_exp;

        v_exp = exp_new_lit_i64(ret_id->meta.rel_addr, &ret_id->pos);
        meta_set_int32(&v_exp->meta);

        addr_exp = exp_new_binary(OP_ADD, addr_exp, v_exp, &ret_id->pos);
        meta_set_int32(&addr_exp->meta);
    }

    return addr_exp;
}

static void
add_return_param(trans_t *trans, ast_exp_t *call_exp, ast_exp_t *ret_exp)
{
    ir_fn_t *fn = trans->fn;
    ast_id_t *ret_id = ret_exp->id->u_fn.ret_id;

    /* Since all return values of a function are treated as parameters, each return
     * value is added as a separate statement
     *
     * As a result, "x, y = f();" is tranformed to "f(); x = r1; y = r2;" in the
     * assignment statement */

    ASSERT(fn != NULL);

    /* The return value is always stored in stack memory */
    if (is_tuple_id(ret_id)) {
        int i;
        array_t *elem_exps = array_new();
        array_t *elem_ids = ret_id->u_tup.elem_ids;

        array_foreach(elem_ids, i) {
            ast_id_t *elem_id = array_get_id(elem_ids, i);

            ASSERT1(elem_id->meta.rel_offset == 0, elem_id->meta.rel_offset);

            fn_add_stack(fn, &elem_id->meta);

            array_add_last(call_exp->u_call.param_exps,
                           make_return_exp(elem_id, fn->stack_idx));

            array_add_last(elem_exps,
                           exp_new_stack(elem_id->meta.type, fn->stack_idx,
                                         elem_id->meta.rel_addr, 0));
        }

        ret_exp->kind = EXP_TUPLE;
        ret_exp->u_tup.elem_exps = elem_exps;
    }
    else {
        ASSERT1(ret_id->meta.rel_offset == 0, ret_id->meta.rel_offset);

        fn_add_stack(fn, &ret_id->meta);

        array_add_last(call_exp->u_call.param_exps,
                       make_return_exp(ret_id, fn->stack_idx));

        exp_set_stack(ret_exp, fn->stack_idx, ret_id->meta.rel_addr, 0);
    }
}
#endif

static void
exp_trans_call(trans_t *trans, ast_exp_t *exp)
{
    int i;
    ast_id_t *fn_id = exp->id;
    ast_exp_t *id_exp = exp->u_call.id_exp;
    ir_fn_t *fn = trans->fn;

    if (is_map_meta(&exp->meta))
        /* TODO */
        return;

    exp_trans(trans, id_exp);

    if (is_ctor_id(exp->id))
        /* The constructor does not have a parameter and returns a contract address */
        return;

    /* Since non-constructor functions are added the contract base address as a first
     * argument, we must also add the address as a call argument here */
    if (exp->u_call.param_exps == NULL)
        exp->u_call.param_exps = array_new();

    if (is_access_exp(id_exp)) {
        ast_exp_t *qual_exp = id_exp->u_acc.qual_exp;

        ASSERT1(is_object_meta(&qual_exp->meta), qual_exp->meta.type);

        /* If the call expression is of type "x.y()", pass "x" as the first argument */
        array_add_first(exp->u_call.param_exps, qual_exp);
    }
    else {
        ASSERT1(is_local_exp(id_exp), id_exp->kind);
        ASSERT(trans->fn->cont_idx == 0);

        /* If the call expression is of type "x()", pass my first parameter as the first
         * parameter of the target */
        array_add_first(exp->u_call.param_exps, exp_new_local(TYPE_UINT32, 0));
    }

    array_foreach(exp->u_call.param_exps, i) {
        exp_trans(trans, array_get_exp(exp->u_call.param_exps, i));
    }

    if (fn->usage > 0) {
        ast_exp_t *l_exp = exp_new_local(TYPE_UINT32, fn->stack_idx);
        ast_exp_t *v_exp = exp_new_lit_i64(ALIGN64(fn->usage), &exp->pos);
        ast_exp_t *r_exp = exp_new_binary(OP_SUB, l_exp, v_exp, &exp->pos);

        meta_set_int32(&v_exp->meta);

        bb_add_stmt(trans->bb,
                    stmt_new_assign(exp_new_global("stack$offset"), r_exp, &exp->pos));
    }

    if (fn_id->u_fn.ret_id != NULL) {
        ast_exp_t *var_exp;
        ast_id_t *tmp_id = id_new_tmp_var("temp$val");

        tmp_id->up = fn_id;
        meta_copy(&tmp_id->meta, &exp->meta);

        fn_add_local(fn, tmp_id);

        var_exp = exp_new_local(tmp_id->meta.type, tmp_id->idx);

        /* If there is a return value, we have to clone it because the call expression
         * itself is transformed */
        bb_add_stmt(trans->bb, stmt_new_assign(var_exp, exp_clone(exp), &exp->pos));

        exp_set_local(exp, tmp_id->idx);
    }
#if 0
    if (exp->id->u_fn.ret_id != NULL) {
        ast_exp_t *call_exp = exp_clone(exp);

        add_return_param(trans, call_exp, exp);

        /* If there is a return value, we have to clone it because the call expression
         * itself is transformed */
        bb_add_stmt(trans->bb, stmt_new_exp(call_exp, &exp->pos));
    }
    else {
        bb_add_stmt(trans->bb, stmt_new_exp(exp, &exp->pos));
    }
#endif
}

static void
exp_trans_sql(trans_t *trans, ast_exp_t *exp)
{
    /* TODO */
}

static void
exp_trans_tuple(trans_t *trans, ast_exp_t *exp)
{
    int i;
    array_t *elem_exps = exp->u_tup.elem_exps;

    array_foreach(elem_exps, i) {
        exp_trans(trans, array_get_exp(elem_exps, i));
    }
}

static void
exp_trans_init(trans_t *trans, ast_exp_t *exp)
{
    int i;
    bool is_aggr_val = true;
    array_t *elem_exps = exp->u_init.elem_exps;

    array_foreach(elem_exps, i) {
        ast_exp_t *elem_exp = array_get_exp(elem_exps, i);

        exp_trans(trans, elem_exp);

        if (!is_lit_exp(elem_exp))
            is_aggr_val = false;
    }

    if (is_aggr_val) {
        int offset = 0;
        uint32_t size = meta_size(&exp->meta);
        char *raw = xcalloc(size);

        array_foreach(elem_exps, i) {
            ast_exp_t *elem_exp = array_get_exp(elem_exps, i);
            value_t *elem_val = &elem_exp->u_lit.val;

            offset = ALIGN(offset, meta_align(&elem_exp->meta));

            memcpy(raw + offset, val_ptr(elem_val), val_size(elem_val));
            offset += meta_size(&elem_exp->meta);
        }

        ASSERT2(offset <= size, offset, size);

        exp_set_lit(exp, NULL);
        value_set_ptr(&exp->u_lit.val, raw, size);
    }
    else {
        fn_add_stack(trans->fn, &exp->meta);
    }
}

static void
exp_trans_alloc(trans_t *trans, ast_exp_t *exp)
{
    fn_add_stack(trans->fn, &exp->meta);
}

void
exp_trans(trans_t *trans, ast_exp_t *exp)
{
    ASSERT(exp != NULL);

    switch (exp->kind) {
    case EXP_NULL:
        break;

    case EXP_LIT:
        exp_trans_lit(trans, exp);
        break;

    case EXP_ID:
        exp_trans_id(trans, exp);
        break;

    case EXP_ARRAY:
        exp_trans_array(trans, exp);
        break;

    case EXP_CAST:
        exp_trans_cast(trans, exp);
        break;

    case EXP_UNARY:
        exp_trans_unary(trans, exp);
        break;

    case EXP_BINARY:
        exp_trans_binary(trans, exp);
        break;

    case EXP_TERNARY:
        exp_trans_ternary(trans, exp);
        break;

    case EXP_ACCESS:
        exp_trans_access(trans, exp);
        break;

    case EXP_CALL:
        exp_trans_call(trans, exp);
        break;

    case EXP_SQL:
        exp_trans_sql(trans, exp);
        break;

    case EXP_TUPLE:
        exp_trans_tuple(trans, exp);
        break;

    case EXP_INIT:
        exp_trans_init(trans, exp);
        break;

    case EXP_ALLOC:
        exp_trans_alloc(trans, exp);
        break;

    case EXP_GLOBAL:
    case EXP_LOCAL:
    case EXP_STACK:
        break;

    default:
        ASSERT1(!"invalid expression", exp->kind);
    }
}

/* end of trans_exp.c */
