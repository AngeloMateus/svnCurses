#include "svn_api.h"

struct status_baton
{
  /* These fields all correspond to the ones in the
     svn_cl__print_status() interface. */
  const char *target_abspath;
  const char *target_path;
  svn_boolean_t suppress_externals_placeholders;
  svn_boolean_t detailed;
  svn_boolean_t show_last_committed;
  svn_boolean_t skip_unrecognized;
  svn_boolean_t repos_locks;

  apr_hash_t *cached_changelists;
  apr_pool_t *cl_pool;          /* where cached changelists are allocated */

  svn_boolean_t had_print_error;  /* To avoid printing lots of errors if we get
                                     errors while printing to stdout */
  svn_boolean_t xml_mode;

  /* Conflict stats. */
  unsigned int text_conflicts;
  unsigned int prop_conflicts;
  unsigned int tree_conflicts;

  svn_client_ctx_t *ctx;

  /*
  void api_get_svn_status(){
    svn_error_t *error;
    apr_pool_t *pool;
    apr_initialize();
    atexit(apr_terminate);
    apr_pool_create(&pool,NULL);
    apr_pool_destroy(pool);
    //svn_wc_walk_status;
  }
  */
};

