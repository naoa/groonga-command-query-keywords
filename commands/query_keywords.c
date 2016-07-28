#include <groonga/plugin.h>

#ifdef __GNUC__
# define GNUC_UNUSED __attribute__((__unused__))
#else
# define GNUC_UNUSED
#endif

static grn_obj *
command_query_keywords(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
                       grn_user_data *user_data)
{
  grn_obj *query;
  grn_obj *table_name;
  grn_obj *table = NULL, *default_column = NULL, *record = NULL, *expr = NULL;
  grn_expr_flags flags;
  grn_rc rc = GRN_SUCCESS;

  table_name = grn_plugin_proc_get_var(ctx, user_data, "table", -1);
  if (GRN_TEXT_LEN(table_name) == 0) {
    GRN_PLUGIN_ERROR(ctx, GRN_INVALID_ARGUMENT,
                     "[query_keywords] missing table_name");
    return NULL;
  }
  table = grn_ctx_get(ctx, GRN_TEXT_VALUE(table_name), GRN_TEXT_LEN(table_name));
  if (!table) {
    GRN_PLUGIN_ERROR(ctx, GRN_INVALID_ARGUMENT,
                     "[query_keywords] missing table");
    return NULL;
  }

  query = grn_plugin_proc_get_var(ctx, user_data, "query", -1);

  if (GRN_TEXT_LEN(query) == 0) {
    GRN_PLUGIN_ERROR(ctx, GRN_INVALID_ARGUMENT,
                     "[query_keywords] missing query");
    return NULL;
  }

  default_column = grn_obj_column(ctx, table,
                                  GRN_COLUMN_NAME_ID,
                                  GRN_COLUMN_NAME_ID_LEN);

  if (!default_column) {
     GRN_PLUGIN_ERROR(ctx, GRN_NO_MEMORY_AVAILABLE,
                      "[query_keywords] failed to create default column");
    goto exit;
  }

  GRN_EXPR_CREATE_FOR_QUERY(ctx, table, expr, record);
  flags = GRN_EXPR_SYNTAX_QUERY|GRN_EXPR_ALLOW_PRAGMA|GRN_EXPR_ALLOW_COLUMN;

  rc = grn_expr_parse(ctx,
                      expr,
                      GRN_TEXT_VALUE(query),
                      GRN_TEXT_LEN(query),
                      default_column,
                      GRN_OP_MATCH,
                      GRN_OP_OR,
                      flags);


  if (rc != GRN_SUCCESS) {
    GRN_PLUGIN_ERROR(ctx, GRN_NO_MEMORY_AVAILABLE,
                     "[query_keywords] failed to parse query");
    goto exit;
  }

  {
    size_t i, n_keywords;
    grn_obj current_keywords;
    GRN_PTR_INIT(&current_keywords, GRN_OBJ_VECTOR, GRN_ID_NIL);
    grn_expr_get_keywords(ctx, expr, &current_keywords);

    n_keywords = GRN_BULK_VSIZE(&current_keywords) / sizeof(grn_obj *);

    grn_ctx_output_array_open(ctx, "KEYWORDS", n_keywords);

    for (i = 0; i < n_keywords; i++) {
      grn_obj *keyword;
      keyword = GRN_PTR_VALUE_AT(&current_keywords, i);
      grn_ctx_output_str(ctx, GRN_TEXT_VALUE(keyword), GRN_TEXT_LEN(keyword));
    }

    grn_ctx_output_array_close(ctx);
    grn_obj_unlink(ctx, &current_keywords);
  }

exit :
  
  if (table) {
    grn_obj_unlink(ctx, table);
  }
  if (default_column) {
    grn_obj_unlink(ctx, default_column);
  }
  if (expr) {
    grn_obj_unlink(ctx, expr);
  }

  return NULL;
}

grn_rc
GRN_PLUGIN_INIT(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_expr_var vars[2];

  grn_plugin_expr_var_init(ctx, &vars[0], "table", -1);
  grn_plugin_expr_var_init(ctx, &vars[1], "query", -1);
  grn_plugin_command_create(ctx, "query_keywords", -1, command_query_keywords, 2, vars);

  return ctx->rc;
}

grn_rc
GRN_PLUGIN_FIN(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}
