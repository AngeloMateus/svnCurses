/*
 * atomic-ra-revprop-change.c :  wrapper around svn_ra_change_rev_prop2()
 *
 * ====================================================================
 *    Licensed to the Apache Software Foundation (ASF) under one
 *    or more contributor license agreements.  See the NOTICE file
 *    distributed with this work for additional information
 *    regarding copyright ownership.  The ASF licenses this file
 *    to you under the Apache License, Version 2.0 (the
 *    "License"); you may not use this file except in compliance
 *    with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing,
 *    software distributed under the License is distributed on an
 *    "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *    KIND, either express or implied.  See the License for the
 *    specific language governing permissions and limitations
 *    under the License.
 * ====================================================================
 */

#include <stdlib.h>
#include <stdio.h>

#include <apr_pools.h>
#include <apr_general.h>

#include "svn_types.h"
#include "svn_pools.h"
#include "svn_dirent_uri.h"
#include "svn_ra.h"
#include "svn_cmdline.h"

#include "private/svn_skel.h"

#include "svn_private_config.h"


#define KEY_OLD_PROPVAL "old_value_p"
#define KEY_NEW_PROPVAL "value"

#define USAGE_MSG \
  "Usage: %s URL REVISION PROPNAME VALUES_SKEL WANT_ERROR CONFIG_DIR\n" \
  "\n" \
  "VALUES_SKEL is a proplist skel containing pseudo-properties '%s' \n" \
  "and '%s'.  A pseudo-property missing from the skel is interpreted \n" \
  "as unset.\n" \
  "\n" \
  "WANT_ERROR is 1 if the propchange is expected to fail due to the atomicity,"\
  "and 0 if it is expected to succeed.  If the expectation matches reality," \
  "the exit code shall be zero.\n"


static svn_error_t *
construct_auth_baton(svn_auth_baton_t **auth_baton_p,
                     const char *config_dir,
                     apr_pool_t *pool)
{
  SVN_ERR(svn_cmdline_create_auth_baton2(auth_baton_p,
                                         TRUE  /* non_interactive */,
                                         "jrandom", "rayjandom",
                                         config_dir,
                                         TRUE  /* no_auth_cache */,
                                         FALSE /* trust_server_cert */,
                                         FALSE, FALSE, FALSE, FALSE,
                                         NULL, NULL, NULL, pool));
  return SVN_NO_ERROR;
}

static svn_error_t *
construct_config(apr_hash_t **config_p,
                 const char *config_dir,
                 apr_pool_t *pool)
{
  SVN_ERR(svn_config_get_config(config_p, config_dir, pool));

  return SVN_NO_ERROR;
}

static svn_error_t *
change_rev_prop(const char *url,
                svn_revnum_t revision,
                const char *propname,
                const svn_string_t *propval,
                const svn_string_t *old_value,
                svn_boolean_t want_error,
                const char *config_dir,
                apr_pool_t *pool)
{
  svn_ra_callbacks2_t *callbacks;
  svn_ra_session_t *sess;
  apr_hash_t *config;
  svn_boolean_t capable;
  svn_error_t *err;

  SVN_ERR(svn_ra_create_callbacks(&callbacks, pool));
  SVN_ERR(construct_auth_baton(&callbacks->auth_baton, config_dir, pool));
  SVN_ERR(construct_config(&config, config_dir, pool));

  SVN_ERR(svn_ra_open5(&sess, NULL, NULL, url, NULL,
                       callbacks, NULL /* baton */, config, pool));

  SVN_ERR(svn_ra_has_capability(sess, &capable,
                                SVN_RA_CAPABILITY_ATOMIC_REVPROPS,
                                pool));
  if (capable)
    {
      err = svn_ra_change_rev_prop2(sess, revision, propname,
                                    &old_value, propval, pool);

      if (want_error && err
          && svn_error_find_cause(err, SVN_ERR_FS_PROP_BASEVALUE_MISMATCH))
        {
          /* Expectation was matched.  Get out. */
          svn_error_clear(err);
          return SVN_NO_ERROR;
        }
      else if (! want_error && ! err)
        /* Expectation was matched.  Get out. */
        return SVN_NO_ERROR;
      else if (want_error && ! err)
        return svn_error_create(SVN_ERR_TEST_FAILED, NULL,
                                "An error was expected but not seen");
      else
        /* A real (non-SVN_ERR_FS_PROP_BASEVALUE_MISMATCH) error. */
        return svn_error_trace(err);
    }
  else
    /* Running under --server-minor-version? */
    return svn_error_create(SVN_ERR_TEST_FAILED, NULL,
                            "Server doesn't advertise "
                            "SVN_RA_CAPABILITY_ATOMIC_REVPROPS");
}

/* Parse SKEL_CSTR according to the description in USAGE_MSG. */
static svn_error_t *
extract_values_from_skel(svn_string_t **old_propval_p,
                         svn_string_t **propval_p,
                         const char *skel_cstr,
                         apr_pool_t *pool)
{
  apr_hash_t *proplist;
  svn_skel_t *skel;

  skel = svn_skel__parse(skel_cstr, strlen(skel_cstr), pool);
  SVN_ERR(svn_skel__parse_proplist(&proplist, skel, pool));
  *old_propval_p = apr_hash_get(proplist, KEY_OLD_PROPVAL, APR_HASH_KEY_STRING);
  *propval_p = apr_hash_get(proplist, KEY_NEW_PROPVAL, APR_HASH_KEY_STRING);

  return SVN_NO_ERROR;
}

int
main(int argc, const char *argv[])
{
  apr_pool_t *pool;
  int exit_code = EXIT_SUCCESS;
  svn_error_t *err;
  const char *url;
  svn_revnum_t revision;
  const char *propname;
  svn_string_t *propval;
  svn_string_t *old_propval;
  char *digits_end = NULL;
  svn_boolean_t want_error;
  const char *config_dir;

  if (argc != 7)
    {
      fprintf(stderr, USAGE_MSG, argv[0], KEY_OLD_PROPVAL, KEY_NEW_PROPVAL);
      exit(1);
    }

  if (apr_initialize() != APR_SUCCESS)
    {
      fprintf(stderr, "apr_initialize() failed.\n");
      exit(1);
    }

  /* set up the global pool */
  pool = svn_pool_create(NULL);

  /* Parse argv. */
  url = svn_uri_canonicalize(argv[1], pool);
  revision = strtol(argv[2], &digits_end, 10);
  propname = argv[3];
  SVN_INT_ERR(extract_values_from_skel(&old_propval, &propval, argv[4], pool));
  want_error = !strcmp(argv[5], "1");
  config_dir = svn_dirent_canonicalize(argv[6], pool);


  if ((! SVN_IS_VALID_REVNUM(revision)) || (! digits_end) || *digits_end)
    SVN_INT_ERR(svn_error_create(SVN_ERR_CL_ARG_PARSING_ERROR, NULL,
                                 _("Invalid revision number supplied")));

  /* Do something. */
  err = change_rev_prop(url, revision, propname, propval, old_propval,
                        want_error, config_dir, pool);
  if (err)
    {
      svn_handle_error2(err, stderr, FALSE, "atomic-ra-revprop-change: ");
      svn_error_clear(err);
      exit_code = EXIT_FAILURE;
    }

  /* Clean up, and get outta here */
  svn_pool_destroy(pool);
  apr_terminate();

  return exit_code;
}
