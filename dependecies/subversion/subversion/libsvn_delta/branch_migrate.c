/*
 * migrate.c: Migrate history from non-move-tracking revisions
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

#include "svn_types.h"
#include "svn_error.h"
#include "svn_delta.h"
#include "svn_ra.h"

#include "private/svn_branch.h"
#include "private/svn_branch_compat.h"

#include "svn_private_config.h"


struct edit_baton
{
  svn_branch__txn_t *edit_txn;
  svn_ra_session_t *from_session;
  svn_revnum_t revision;
};

struct dir_baton
{
  struct edit_baton *edit_baton;
};

struct file_baton
{
  struct edit_baton *edit_baton;
};

static svn_error_t *
set_target_revision(void *edit_baton,
                    svn_revnum_t target_revision,
                    apr_pool_t *pool)
{
  /*struct edit_baton *eb = edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->set_target_revision(eb->wrapped_edit_baton,
                                                 target_revision,
                                                 pool));*/
  return SVN_NO_ERROR;
}

static svn_error_t *
open_root(void *edit_baton,
          svn_revnum_t base_revision,
          apr_pool_t *pool,
          void **root_baton)
{
  struct edit_baton *eb = edit_baton;
  struct dir_baton *dir_baton = apr_palloc(pool, sizeof(*dir_baton));

  /*SVN_ERR(eb->wrapped_editor->open_root(eb->wrapped_edit_baton,
                                        base_revision,
                                        pool,
                                        &dir_baton->wrapped_dir_baton));*/

  dir_baton->edit_baton = eb;

  *root_baton = dir_baton;

  return SVN_NO_ERROR;
}

static svn_error_t *
delete_entry(const char *path,
             svn_revnum_t base_revision,
             void *parent_baton,
             apr_pool_t *pool)
{
  /*struct dir_baton *pb = parent_baton;
  struct edit_baton *eb = pb->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->delete_entry(path,
                                          base_revision,
                                          pb->wrapped_dir_baton,
                                          pool);*/
  return SVN_NO_ERROR;
}

static svn_error_t *
add_directory(const char *path,
              void *parent_baton,
              const char *copyfrom_path,
              svn_revnum_t copyfrom_revision,
              apr_pool_t *pool,
              void **child_baton)
{
  struct dir_baton *pb = parent_baton;
  struct edit_baton *eb = pb->edit_baton;
  struct dir_baton *db = apr_palloc(pool, sizeof(*db));

  /*SVN_ERR(eb->wrapped_editor->add_directory(path,
                                            pb->wrapped_dir_baton,
                                            copyfrom_path,
                                            copyfrom_revision,
                                            pool,
                                            &db->wrapped_dir_baton));*/

  db->edit_baton = eb;
  *child_baton = db;

  return SVN_NO_ERROR;
}

static svn_error_t *
open_directory(const char *path,
               void *parent_baton,
               svn_revnum_t base_revision,
               apr_pool_t *pool,
               void **child_baton)
{
  struct dir_baton *pb = parent_baton;
  struct edit_baton *eb = pb->edit_baton;
  struct dir_baton *db = apr_palloc(pool, sizeof(*db));

  /*SVN_ERR(eb->wrapped_editor->open_directory(path,
                                             pb->wrapped_dir_baton,
                                             base_revision,
                                             pool,
                                             &db->wrapped_dir_baton));*/

  db->edit_baton = eb;
  *child_baton = db;

  return SVN_NO_ERROR;
}

static svn_error_t *
add_file(const char *path,
         void *parent_baton,
         const char *copyfrom_path,
         svn_revnum_t copyfrom_revision,
         apr_pool_t *pool,
         void **file_baton)
{
  struct dir_baton *pb = parent_baton;
  struct edit_baton *eb = pb->edit_baton;
  struct file_baton *fb = apr_palloc(pool, sizeof(*fb));

  /*SVN_ERR(eb->wrapped_editor->add_file(path,
                                       pb->wrapped_dir_baton,
                                       copyfrom_path,
                                       copyfrom_revision,
                                       pool,
                                       &fb->wrapped_file_baton));*/

  fb->edit_baton = eb;
  *file_baton = fb;

  return SVN_NO_ERROR;
}

static svn_error_t *
open_file(const char *path,
          void *parent_baton,
          svn_revnum_t base_revision,
          apr_pool_t *pool,
          void **file_baton)
{
  struct dir_baton *pb = parent_baton;
  struct edit_baton *eb = pb->edit_baton;
  struct file_baton *fb = apr_palloc(pool, sizeof(*fb));

  /*SVN_ERR(eb->wrapped_editor->open_file(path,
                                        pb->wrapped_dir_baton,
                                        base_revision,
                                        pool,
                                        &fb->wrapped_file_baton));*/

  fb->edit_baton = eb;
  *file_baton = fb;

  return SVN_NO_ERROR;
}

static svn_error_t *
apply_textdelta(void *file_baton,
                const char *base_checksum,
                apr_pool_t *pool,
                svn_txdelta_window_handler_t *handler,
                void **handler_baton)
{
  /*struct file_baton *fb = file_baton;
  struct edit_baton *eb = fb->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->apply_textdelta(fb->wrapped_file_baton,
                                              base_checksum,
                                              pool,
                                              handler,
                                              handler_baton));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
close_file(void *file_baton,
           const char *text_checksum,
           apr_pool_t *pool)
{
  /*struct file_baton *fb = file_baton;
  struct edit_baton *eb = fb->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->close_file(fb->wrapped_file_baton,
                                         text_checksum, pool));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
absent_file(const char *path,
            void *file_baton,
            apr_pool_t *pool)
{
  /*struct file_baton *fb = file_baton;
  struct edit_baton *eb = fb->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->absent_file(path, fb->wrapped_file_baton,
                                          pool));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
close_directory(void *dir_baton,
                apr_pool_t *pool)
{
  /*struct dir_baton *db = dir_baton;
  struct edit_baton *eb = db->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->close_directory(db->wrapped_dir_baton,
                                              pool));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
absent_directory(const char *path,
                 void *dir_baton,
                 apr_pool_t *pool)
{
  /*struct dir_baton *db = dir_baton;
  struct edit_baton *eb = db->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->absent_directory(path, db->wrapped_dir_baton,
                                               pool));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
change_file_prop(void *file_baton,
                 const char *name,
                 const svn_string_t *value,
                 apr_pool_t *pool)
{
  /*struct file_baton *fb = file_baton;
  struct edit_baton *eb = fb->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->change_file_prop(fb->wrapped_file_baton,
                                               name,
                                               value,
                                               pool));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
change_dir_prop(void *dir_baton,
                const char *name,
                const svn_string_t *value,
                apr_pool_t *pool)
{
  /*struct dir_baton *db = dir_baton;
  struct edit_baton *eb = db->edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->change_dir_prop(db->wrapped_dir_baton,
                                              name,
                                              value,
                                              pool));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
close_edit(void *edit_baton,
           apr_pool_t *pool)
{
  /*struct edit_baton *eb = edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->close_edit(eb->wrapped_edit_baton, pool));*/

  return SVN_NO_ERROR;
}

static svn_error_t *
abort_edit(void *edit_baton,
           apr_pool_t *pool)
{
  /*struct edit_baton *eb = edit_baton;*/

  /*SVN_ERR(eb->wrapped_editor->abort_edit(eb->wrapped_edit_baton, pool));*/

  return SVN_NO_ERROR;
}

svn_error_t *
svn_branch__compat_get_migration_editor(
                        const svn_delta_editor_t **old_editor,
                        void **old_edit_baton,
                        svn_branch__txn_t *edit_txn,
                        svn_ra_session_t *from_session,
                        svn_revnum_t revision,
                        apr_pool_t *result_pool)
{
  static const svn_delta_editor_t editor = {
    set_target_revision,
    open_root,
    delete_entry,
    add_directory,
    open_directory,
    change_dir_prop,
    close_directory,
    absent_directory,
    add_file,
    open_file,
    apply_textdelta,
    change_file_prop,
    close_file,
    absent_file,
    close_edit,
    abort_edit
  };
  struct edit_baton *eb = apr_palloc(result_pool, sizeof(*eb));

  eb->edit_txn = edit_txn;
  eb->from_session = from_session;
  eb->revision = revision;

  *old_editor = &editor;
  *old_edit_baton = eb;

  return SVN_NO_ERROR;
}

