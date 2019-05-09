/**
 * @file    syslib.c
 * @copyright defined in aergo/LICENSE.txt
 */

#include "common.h"

#include "binaryen-c.h"
#include "ast_id.h"
#include "ast_exp.h"
#include "parse.h"
#include "ir_abi.h"
#include "ir_md.h"
#include "gen_util.h"
#include "iobuf.h"

#include "syslib.h"

char *lib_src =
"library system {\n"
"    func abs32(int32 v) int32 : \"__abs32\";\n"
"    func abs64(int64 v) int64 : \"__abs64\";\n"
"    func abs128(int128 v) int128 : \"__mpz_abs\";\n"

"    func pow32(int32 x, int32 y) int32 : \"__pow32\";\n"
"    func pow64(int64 x, int32 y) int64 : \"__pow64\";\n"
"    func pow128(int128 x, int32 y) int128 : \"__mpz_pow\";\n"

"    func sign32(int32 v) int8 : \"__sign32\";\n"
"    func sign64(int64 v) int8 : \"__sign64\";\n"
"    func sign128(int128 v) int8 : \"__mpz_sign\";\n"

"    func lower(string v) string : \"__lower\";\n"
"    func upper(string v) string : \"__upper\";\n"
"}";

sys_fn_t sys_fntab_[FN_MAX] = {
    { "__udf", NULL, 0, { TYPE_NONE }, TYPE_NONE },
    { "__ctor", NULL, 0, { TYPE_NONE }, TYPE_NONE },
#undef fn_def
#define fn_def(kind, name, ...)                                                                    \
    { name, SYSLIB_MODULE"."name, __VA_ARGS__ },
#include "syslib.list"
};

void
syslib_load(ast_t *ast)
{
    flag_t flag = { FLAG_NONE, 0, 0 };
    iobuf_t src;

    iobuf_init(&src, "system_library");
    iobuf_set(&src, strlen(lib_src), lib_src);

    parse(&src, flag, ast);
}

ir_abi_t *
syslib_abi(sys_fn_t *sys_fn)
{
    int i;
    ir_abi_t *abi = xcalloc(sizeof(ir_abi_t));

    ASSERT(sys_fn != NULL);

    abi->module = SYSLIB_MODULE;
    abi->name = sys_fn->name;

    abi->param_cnt = sys_fn->param_cnt;
    abi->params = xmalloc(sizeof(BinaryenType) * abi->param_cnt);

    for (i = 0; i < abi->param_cnt; i++) {
        abi->params[i] = type_gen(sys_fn->params[i]);
    }

    abi->result = type_gen(sys_fn->result);

    return abi;
}

BinaryenExpressionRef
syslib_call_1(gen_t *gen, fn_kind_t kind, BinaryenExpressionRef argument)
{
    sys_fn_t *sys_fn = SYS_FN(kind);

    ASSERT2(sys_fn->param_cnt == 1, kind, sys_fn->param_cnt);

    md_add_abi(gen->md, syslib_abi(sys_fn));

    return BinaryenCall(gen->module, sys_fn->qname, &argument, 1, type_gen(sys_fn->result));
}

BinaryenExpressionRef
syslib_call_2(gen_t *gen, fn_kind_t kind, BinaryenExpressionRef argument1,
              BinaryenExpressionRef argument2)
{
    sys_fn_t *sys_fn = SYS_FN(kind);
    BinaryenExpressionRef arguments[2] = { argument1, argument2 };

    ASSERT2(sys_fn->param_cnt == 2, kind, sys_fn->param_cnt);

    md_add_abi(gen->md, syslib_abi(sys_fn));

    return BinaryenCall(gen->module, sys_fn->qname, arguments, 2, type_gen(sys_fn->result));
}

BinaryenExpressionRef
syslib_gen(gen_t *gen, fn_kind_t kind, int argc, ...)
{
    int i;
    va_list vargs;
    sys_fn_t *sys_fn = SYS_FN(kind);
    BinaryenExpressionRef arguments[SYSLIB_MAX_ARGS];

    ASSERT1(argc <= SYSLIB_MAX_ARGS, argc);
    ASSERT3(sys_fn->param_cnt == argc, kind, sys_fn->param_cnt, argc);

    va_start(vargs, argc);

    for (i = 0; i < argc; i++) {
        arguments[i] = va_arg(vargs, BinaryenExpressionRef);
    }

    va_end(vargs);

    md_add_abi(gen->md, syslib_abi(sys_fn));

    return BinaryenCall(gen->module, sys_fn->qname, arguments, argc, type_gen(sys_fn->result));
}

/* end of syslib.c */
