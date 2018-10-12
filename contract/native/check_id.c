/**
 * @file    check_id.c
 * @copyright defined in aergo/LICENSE.txt
 */

#include "common.h"

#include "array.h"
#include "ast_exp.h"

#include "check_exp.h"
#include "check_blk.h"

#include "check_id.h"

static int
id_var_check(check_t *check, ast_id_t *id)
{
    int arr_size = 0;
    ast_exp_t *type_exp;
    meta_t *type_meta;
    ast_exp_t *arr_exp;
    ast_exp_t *init_exp;

    ASSERT1(is_var_id(id), id->kind);
    ASSERT(id->u_var.type_exp != NULL);

    type_exp = id->u_var.type_exp;
    type_meta = &type_exp->meta;

    ASSERT1(is_type_exp(type_exp), type_exp->kind);

    CHECK(check_exp(check, type_exp));

    if (type_exp->u_type.is_public)
        flag_set(id->mod, MOD_PUBLIC);

    id->meta = *type_meta;

    arr_exp = id->u_var.arr_exp;
    init_exp = id->u_var.init_exp;

    if (arr_exp != NULL) {
        /* XXX: multi-dimensional array */
        CHECK(check_exp(check, arr_exp));

        id->meta.is_array = true;

        if (is_null_exp(arr_exp)) {
            if (init_exp == NULL)
                RETURN(ERROR_MISSING_ARR_SIZE, &arr_exp->pos);
        }
        else {
            meta_t *arr_meta = &arr_exp->meta;

            if (!is_integer_meta(arr_meta))
                RETURN(ERROR_INVALID_SIZE_TYPE, &arr_exp->pos, META_NAME(arr_meta));

            if (!is_untyped_meta(arr_meta))
                RETURN(ERROR_INVALID_SIZE_VAL, &arr_exp->pos);

            ASSERT1(is_val_exp(arr_exp), arr_exp->kind);
            ASSERT1(is_int_val(&arr_exp->u_val.val), arr_exp->u_val.val.kind);

            arr_size = (int)arr_exp->u_val.val.iv;
        }
    }

    if (init_exp != NULL) {
        /* TODO: named initializer */
        meta_t *init_meta = &init_exp->meta;

        if (arr_exp == NULL) {
            CHECK(check_exp(check, init_exp));

            if (is_tuple_meta(init_meta) &&
                !is_map_meta(type_meta) && !is_struct_meta(type_meta))
                RETURN(ERROR_NOT_ALLOWED_INIT, &init_exp->pos);

            /* XXX: need to check value overflow */
            if (is_map_meta(type_meta))
                CHECK(meta_cmp_array(type_meta, init_meta->u_tup.metas));
            else
                CHECK(meta_cmp(type_meta, init_meta));
        }
        else {
            array_t *elem_metas;

            CHECK(check_exp(check, init_exp));

            if (!is_tuple_meta(init_meta))
                RETURN(ERROR_MISSING_ARR_INIT, &init_exp->pos);

            if (type_exp->id != NULL &&
                !is_struct_meta(type_meta) && !is_map_meta(type_meta))
                RETURN(ERROR_NOT_ALLOWED_INIT, &init_exp->pos);

            elem_metas = init_meta->u_tup.metas;
            ASSERT(array_size(elem_metas) > 0);

            if (arr_size > 0 && arr_size != array_size(elem_metas))
                RETURN(ERROR_MISMATCHED_ELEM_CNT, &init_exp->pos, arr_size,
                       array_size(elem_metas));

            /* XXX: need to check value overflow */
            if (is_map_meta(type_meta)) {
                int i;

                for (i = 0; i < array_size(elem_metas); i++) {
                    meta_t *map_meta = array_item(elem_metas, i, meta_t);

                    ASSERT1(is_tuple_meta(map_meta), map_meta->type);
                    CHECK(meta_cmp_array(type_meta, map_meta->u_tup.metas));
                }
            }
            else{
                CHECK(meta_cmp_array(type_meta, elem_metas));
            }
        }
    }

    return NO_ERROR;
}

static int
id_struct_check(check_t *check, ast_id_t *id)
{
    int i;
    array_t *fld_ids;

    ASSERT1(is_struct_id(id), id->kind);

    fld_ids = id->u_st.fld_ids;
    ASSERT(fld_ids != NULL);

    for (i = 0; i < array_size(fld_ids); i++) {
        ast_id_t *fld_id = array_item(fld_ids, i, ast_id_t);

        CHECK(id_var_check(check, fld_id));
    }

    meta_set_struct(&id->meta, fld_ids);

    return NO_ERROR;
}

static int
id_param_check(check_t *check, ast_id_t *id)
{
    ast_exp_t *type_exp;
    meta_t *type_meta;

    ASSERT1(is_var_id(id), id->kind);
    ASSERT(id->u_var.type_exp != NULL);
    ASSERT(id->u_var.init_exp == NULL);

    type_exp = id->u_var.type_exp;
    type_meta = &type_exp->meta;

    ASSERT1(is_type_exp(type_exp), type_exp->kind);

    CHECK(check_exp(check, type_exp));

    id->meta = *type_meta;

    if (id->u_var.arr_exp != NULL) {
        ast_exp_t *arr_exp = id->u_var.arr_exp;
        meta_t *arr_meta = &arr_exp->meta;

        id->meta.is_array = true;

        CHECK(check_exp(check, arr_exp));

        if (!is_null_exp(arr_exp) && !is_integer_meta(arr_meta))
            RETURN(ERROR_NOT_ALLOWED_TYPE, &id->pos, META_NAME(arr_meta));
    }

    return NO_ERROR;
}

static int
id_func_check(check_t *check, ast_id_t *id)
{
    int i;
    array_t *param_ids;
    array_t *ret_exps;

    ASSERT1(is_func_id(id), id->kind);

    param_ids = id->u_func.param_ids;

    for (i = 0; i < array_size(param_ids); i++) {
        ast_id_t *param_id = array_item(param_ids, i, ast_id_t);

        id_param_check(check, param_id);
    }

    ret_exps = id->u_func.ret_exps;

    if (ret_exps != NULL) {
        for (i = 0; i < array_size(ret_exps); i++) {
            ast_exp_t *type_exp = array_item(ret_exps, i, ast_exp_t);

            ASSERT1(is_type_exp(type_exp), type_exp->kind);

            check_exp(check, type_exp);
        }

        meta_set_tuple(&id->meta, ret_exps);
    }
    else if (id->mod == MOD_CTOR) {
        meta_set_ref(&id->meta);
    }
    else {
        meta_set_void(&id->meta);
    }

    check->fn_id = id;

    if (id->u_func.blk != NULL)
        check_blk(check, id->u_func.blk);

    check->fn_id = NULL;

    return NO_ERROR;
}

static int
id_contract_check(check_t *check, ast_id_t *id)
{
    ASSERT1(is_contract_id(id), id->kind);

    if (id->u_cont.blk != NULL)
        check_blk(check, id->u_cont.blk);

    meta_set_ref(&id->meta);

    return NO_ERROR;
}

void
check_id(check_t *check, ast_id_t *id)
{
    ASSERT(id->name != NULL);

    switch (id->kind) {
    case ID_VAR:
        id_var_check(check, id);
        break;

    case ID_STRUCT:
        id_struct_check(check, id);
        break;

    case ID_FUNC:
        id_func_check(check, id);
        break;

    case ID_CONTRACT:
        id_contract_check(check, id);
        break;

    default:
        ASSERT1(!"invalid identifier", id->kind);
    }
}

/* end of check_id.c */