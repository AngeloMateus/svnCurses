/*
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
 *
 * core.i: SWIG module interface file for libsvn_subr, a few pieces of
 *   APR functionality, and anything else that does not fit into any
 *   of the more specific module files.
 */

%include svn_global.swg

#if defined(SWIGPYTHON)
%module(package="libsvn", moduleimport=SVN_PYTHON_MODULEIMPORT) core
#elif defined(SWIGPERL)
%module "SVN::_Core"
#elif defined(SWIGRUBY)
%module "svn::ext::core"
#endif

%{
#include <apr.h>
#include <apr_general.h>

#include <apr_md5.h>
#include "svn_diff.h"
#include "svn_private_config.h"
%}

#ifdef SWIGRUBY
%{
#include <apu.h>
#include <apr_xlate.h>
%}
#endif

/* ### for now, let's ignore this thing. */
%ignore svn_prop_t;

/* -----------------------------------------------------------------------
   The following struct members have to be read-only because otherwise
   strings assigned to then would never be freed, resulting in memory
   leaks. This prevents the swig warning "Warning(451): Setting const
   char * member may leak memory."
*/
%immutable svn_log_changed_path_t::copyfrom_path;
%immutable svn_dirent_t::last_author;
%immutable svn_error_t::message;
%immutable svn_error_t::file;

/* -----------------------------------------------------------------------
   completely ignore a number of functions. the presumption is that the
   scripting language already has facilities for these things (or they
   are relatively trivial).
*/

/* svn_io.h: We cherry-pick certain functions from this file. To aid in this,
 * EVERY function in the file is listed in the order it appears, and is either
 * %ignore-d, or present as a comment, explicitly documenting that we wrap it.
 */

%ignore svn_io_check_path;
%ignore svn_io_check_special_path;
%ignore svn_io_check_resolved_path;
/* This is useful for implementing svn_ra_callbacks_t->open_tmp_file */
// svn_io_open_unique_file2
// svn_io_open_unique_file
%ignore svn_io_create_unique_link;
%ignore svn_io_read_link;
%ignore svn_io_temp_dir;
%ignore svn_io_copy_file;
%ignore svn_io_copy_link;
%ignore svn_io_copy_dir_recursively;
%ignore svn_io_make_dir_recursively;
%ignore svn_io_dir_empty;
%ignore svn_io_append_file;
%ignore svn_io_set_file_read_only;
%ignore svn_io_set_file_read_write;
%ignore svn_io_set_file_read_write_carefully;
%ignore svn_io_set_file_executable;
%ignore svn_io_is_file_executable;
%ignore svn_io_read_length_line;
%ignore svn_io_file_affected_time;
%ignore svn_io_set_file_affected_time;
%ignore svn_io_filesizes_different_p;
// svn_io_file_checksum
// svn_io_files_contents_same_p
%ignore svn_io_file_create;
%ignore svn_io_file_lock;
%ignore svn_io_file_lock2;
%ignore svn_io_file_flush_to_disk;
%ignore svn_io_dir_file_copy;

/* Not useful from scripting languages. Custom streams should be achieved
 * by passing a scripting language native stream into a svn_stream_t *
 * parameter, and letting a typemap using svn_swig_xx_make_stream() take
 * care of the details. */
%ignore svn_stream_create;
%ignore svn_stream_set_baton;
%ignore svn_stream_set_read;
%ignore svn_stream_set_write;
%ignore svn_stream_set_close;

/* The permitted svn_stream and svn_stringbuf functions could possibly
 * be used by a script, in conjunction with other APIs which return or
 * accept streams. This requires that the relevant language's custom
 * svn_stream_t wrapping code does not obstruct this usage. */
// svn_stream_empty
// svn_stream_disown
// svn_stream_from_aprfile2
// svn_stream_from_aprfile
// svn_stream_for_stdout
// svn_stream_from_stringbuf
// svn_stream_compressed
/* svn_stream_checksummed would require special attention to wrap, because
 * of the read_digest and write_digest parameters. */
%ignore svn_stream_checksummed;
// svn_stream_read
// svn_stream_write
// svn_stream_close

/* Scripts can do the printf, then write to a stream.
 * We can't really handle the variadic, so ignore it. */
%ignore svn_stream_printf;
%ignore svn_stream_printf_from_utf8;

// svn_stream_readline
// svn_stream_copy
// svn_stream_contents_same
// svn_stringbuf_from_file
// svn_stringbuf_from_aprfile

#ifndef SWIGPYTHON
/* These functions are useful in Python, because they allow you to
 * easily delete files which are marked as read-only on Windows. */
%ignore svn_io_remove_file;
%ignore svn_io_remove_dir;
#endif
%ignore svn_io_get_dir_filenames;
%ignore svn_io_get_dirents2;
%ignore svn_io_get_dirents;
%ignore svn_io_dir_walk;
%ignore svn_io_start_cmd;
%ignore svn_io_wait_for_cmd;
%ignore svn_io_run_cmd;
%ignore svn_io_run_diff;
%ignore svn_io_run_diff3_2;
%ignore svn_io_run_diff3;
// svn_io_detect_mimetype
%ignore svn_io_file_open;
%ignore svn_io_file_close;
%ignore svn_io_file_getc;
%ignore svn_io_file_info_get;
%ignore svn_io_file_read;
%ignore svn_io_file_read_full;
%ignore svn_io_file_seek;
%ignore svn_io_file_write;
%ignore svn_io_file_write_full;
%ignore svn_io_stat;
%ignore svn_io_file_rename;
%ignore svn_io_file_move;
%ignore svn_io_dir_make;
%ignore svn_io_dir_make_hidden;
%ignore svn_io_dir_make_sgid;
%ignore svn_io_dir_open;
%ignore svn_io_dir_remove_nonrecursive;
%ignore svn_io_dir_read;
%ignore svn_io_read_version_file;
%ignore svn_io_write_version_file;

/* svn_path.h: We cherry-pick certain functions from this file. To aid in this,
 * EVERY function in the file is listed in the order it appears, and is either
 * %ignore-d, or present as a comment, explicitly documenting that we wrap it.
 */
// svn_path_internal_style;
// svn_path_local_style;
%ignore svn_path_join;
%ignore svn_path_join_many;
%ignore svn_path_basename;
%ignore svn_path_dirname;
%ignore svn_path_component_count;
%ignore svn_path_add_component;
%ignore svn_path_remove_component;
%ignore svn_path_remove_components;
%ignore svn_path_split;
// svn_path_is_empty;
// svn_path_canonicalize;
// svn_path_compare_paths;
// svn_path_get_longest_ancestor;
%ignore svn_path_get_absolute;
%ignore svn_path_split_if_file;
%ignore svn_path_condense_targets;
%ignore svn_path_remove_redundancies;
%ignore svn_path_decompose;
%ignore svn_path_compose;
%ignore svn_path_is_single_path_component;
%ignore svn_path_is_backpath_present;
%ignore svn_path_is_child;
%ignore svn_path_is_ancestor;
%ignore svn_path_check_valid;
// svn_path_is_url;
// svn_path_is_uri_safe;
%ignore svn_path_uri_encode;
%ignore svn_path_uri_decode;
%ignore svn_path_url_add_component;
%ignore svn_path_uri_from_iri;
%ignore svn_path_uri_autoescape;
%ignore svn_path_cstring_from_utf8;
%ignore svn_path_cstring_to_utf8;

/* svn_dirent_uri.h: SWIG can't digest these functions yet, so ignore them
 * for now. TODO: make them work.
 */
%ignore svn_dirent_join_many;
%ignore svn_dirent_condense_targets;
%ignore svn_uri_condense_targets;
%ignore svn_dirent_is_under_root;

/* Other files */

/* Ignore platform-specific auth functions */
%ignore svn_auth_get_keychain_simple_provider;
%ignore svn_auth_get_keychain_ssl_client_cert_pw_provider;
%ignore svn_auth_get_windows_simple_provider;
%ignore svn_auth_get_windows_ssl_server_trust_provider;
%ignore svn_auth_gnome_keyring_version;
%ignore svn_auth_get_gnome_keyring_simple_provider;
%ignore svn_auth_get_gnome_keyring_ssl_client_cert_pw_provider;
%ignore svn_auth_kwallet_version;
%ignore svn_auth_get_kwallet_simple_provider;
%ignore svn_auth_get_kwallet_ssl_client_cert_pw_provider;
%ignore svn_auth_get_gpg_agent_simple_provider;

/* bad pool convention */
%ignore svn_opt_print_generic_help;

%ignore svn_opt_args_to_target_array;

/* svn_cmdline.h */
%ignore svn_cmdline_auth_plaintext_passphrase_prompt;
%ignore svn_cmdline_auth_plaintext_prompt;
%ignore svn_cmdline_auth_simple_prompt;
%ignore svn_cmdline_auth_ssl_client_cert_prompt;
%ignore svn_cmdline_auth_ssl_client_cert_pw_prompt;
%ignore svn_cmdline_auth_ssl_server_trust_prompt;
%ignore svn_cmdline_auth_username_prompt;
%ignore svn_cmdline_cstring_from_utf8;
%ignore svn_cmdline_cstring_from_utf8_fuzzy;
%ignore svn_cmdline_cstring_to_utf8;
%ignore svn_cmdline_fflush;
%ignore svn_cmdline_fprintf;
%ignore svn_cmdline_fputs;
%ignore svn_cmdline_handle_exit_error;
%ignore svn_cmdline_output_encoding;
%ignore svn_cmdline_path_local_style_from_utf8;
%ignore svn_cmdline_printf;
%ignore svn_cmdline_prompt_baton2_t;
%ignore svn_cmdline_prompt_baton_t;
%ignore svn_cmdline_prompt_user2;
%ignore svn_cmdline_prompt_user;
%ignore svn_cmdline_setup_auth_baton;

/* Ugliness because the constants are typedefed and SWIG ignores them
   as a result. */
%constant svn_revnum_t SWIG_SVN_INVALID_REVNUM = -1;
%constant svn_revnum_t SWIG_SVN_IGNORED_REVNUM = -1;

/* -----------------------------------------------------------------------
   input rangelist
*/
%apply svn_rangelist_t *RANGELIST {
  svn_rangelist_t *rangeinput,
  const svn_rangelist_t *rangelist,
  svn_rangelist_t *from,
  svn_rangelist_t *to,
  svn_rangelist_t *changes,
  svn_rangelist_t *eraser,
  svn_rangelist_t *whiteboard,
  svn_rangelist_t *rangelist1,
  svn_rangelist_t *rangelist2
}

/* -----------------------------------------------------------------------
   output rangelist
*/
%apply svn_rangelist_t **RANGELIST {
  svn_rangelist_t **rangelist,
  svn_rangelist_t **inheritable_rangelist,
  svn_rangelist_t **deleted,
  svn_rangelist_t **added,
  svn_rangelist_t **output
}

/* -----------------------------------------------------------------------
   input and output rangelist
*/
%apply svn_rangelist_t **RANGELIST_INOUT {
  svn_rangelist_t **rangelist_inout
}

/* -----------------------------------------------------------------------
   input mergeinfo hash
*/
%apply apr_hash_t *MERGEINFO {
   apr_hash_t *mergefrom,
   apr_hash_t *mergeto,
   apr_hash_t *mergein1,
   apr_hash_t *mergein2,
   apr_hash_t *mergeinfo,
   apr_hash_t *mergeinput,
   apr_hash_t *eraser,
   apr_hash_t *whiteboard,
   apr_hash_t *changes
}

/* -----------------------------------------------------------------------
   output mergeinfo
*/

#if defined(SWIGPYTHON) || defined(SWIGRUBY)
%apply apr_hash_t **MERGEINFO_INOUT {
    apr_hash_t **mergeinfo_inout
}

%apply apr_hash_t **MERGEINFO {
    apr_hash_t **mergeinfo,
    apr_hash_t **inheritable_mergeinfo,
    apr_hash_t **deleted,
    apr_hash_t **added
}
#endif

/* -----------------------------------------------------------------------
   output mergeinfo hash
*/

#if defined(SWIGRUBY) || defined(SWIGPYTHON)
%apply apr_hash_t **MERGEINFO_CATALOG {
  apr_hash_t **catalog
};
#endif

/* -----------------------------------------------------------------------
   allowable null values
*/
%apply const char *MAY_BE_NULL {
    /* svn_config_get */
    const char *default_value,
    /* svn_config_read_auth_data */
    const char *config_dir,
    /* svn_config_get_user_config_path */
    const char *fname,
    /* svn_diff_file_output_merge */
    const char *conflict_original,
    const char *conflict_modified,
    const char *conflict_latest,
    const char *conflict_separator,
    /* svn_cmdline_create_auth_baton */
    const char *username,
    const char *password
};

/* -----------------------------------------------------------------------
   fix up the svn_stream_read() ptr/len arguments
*/
#ifdef SWIGPYTHON
%typemap(in) (char *buffer, apr_size_t *len) ($*2_type temp) {
    if (PyLong_Check($input)) {
        temp = PyLong_AsUnsignedLong($input);
    }
%#if IS_PY3 != 1
    else if (PyInt_Check($input)) {
        /* wish there was a PyInt_AsUnsignedLong but there isn't
           the mask version doesn't do bounds checking for us.
           I can't see a good way to do the bounds checking ourselves
           so just stick our head in the sand.  With Python3 this
           problem goes away because PyInt is gone anyway. */
        temp = PyInt_AsUnsignedLongMask($input);
    }
%#endif
    else {
        PyErr_SetString(PyExc_TypeError,
                        "expecting an integer for the buffer size");
        SWIG_fail;
    }
    $1 = malloc(temp);
    $2 = ($2_ltype)&temp;
}
#endif
#ifdef SWIGPERL
%typemap(in) (char *buffer, apr_size_t *len) ($*2_type temp) {
    temp = SvIV($input);
    $1 = malloc(temp);
    $2 = ($2_ltype)&temp;
}
#endif
#ifdef SWIGRUBY
%typemap(in) (char *buffer, apr_size_t *len) ($*2_type temp) {
    temp = NUM2LONG($input);
    $1 = malloc(temp);
    $2 = ($2_ltype)&temp;
}
#endif

/* ### need to use freearg or somesuch to ensure the string is freed.
   ### watch out for 'return' anywhere in the binding code. */

#ifdef SWIGPYTHON
%typemap(argout) (char *buffer, apr_size_t *len) {
  %append_output(PyBytes_FromStringAndSize($1, *$2));
  free($1);
}
#endif
#ifdef SWIGPERL
%typemap(argout) (char *buffer, apr_size_t *len) {
  %append_output(sv_2mortal(newSVpvn($1, *$2)));
  free($1);
}
#endif
#ifdef SWIGRUBY
%typemap(argout) (char *buffer, apr_size_t *len) {
  %append_output(*$2 == 0 ? Qnil : rb_str_new($1, *$2));
  free($1);
}
#endif

/* -----------------------------------------------------------------------
   fix up the svn_stream_write() ptr/len arguments
*/
#ifdef SWIGPYTHON
%typemap(in) (const char *data, apr_size_t *len) ($*2_type temp) {
    Py_ssize_t length;
    if (PyBytes_Check($input)) {
        if (PyBytes_AsStringAndSize($input, (char **)&$1, &length) == -1) {
            SWIG_fail;
        }
    }
    else if (PyUnicode_Check($input)) {
        $1 = (char *)PyStr_AsUTF8AndSize($input, &length);
        if (PyErr_Occurred()) {
            SWIG_fail;
        }
    }
    else {
        PyErr_SetString(PyExc_TypeError,
                        "expecting a bytes or str object for the buffer");
        SWIG_fail;
    }
    temp = ($*2_type)length;
    $2 = ($2_ltype)&temp;
}
#endif
#ifdef SWIGPERL
%typemap(in) (const char *data, apr_size_t *len) ($*2_type temp) {
    $1 = SvPV($input, temp);
    $2 = ($2_ltype)&temp;
}
#endif
#ifdef SWIGRUBY
%typemap(in) (const char *data, apr_size_t *len) ($*2_type temp)
{
  $1 = StringValuePtr($input);
  temp = RSTRING_LEN($input);
  $2 = ($2_ltype)&temp;
}
#endif

#ifdef SWIGPYTHON
%typemap(argout) (const char *data, apr_size_t *len) {
  %append_output(PyInt_FromLong(*$2));
}
#endif

#ifdef SWIGPERL
%typemap(argout) (const char *data, apr_size_t *len) {
  %append_output(sv_2mortal(newSViv(*$2)));
}
#endif

#ifdef SWIGRUBY
%typemap(argout) (const char *data, apr_size_t *len) {
  %append_output(LONG2NUM(*$2));
}
#endif

/* -----------------------------------------------------------------------
   auth parameter set/get
*/

/* set */
#ifdef SWIGPYTHON
%typemap(in) const void *value 
  (apr_pool_t *_global_pool = NULL, PyObject *_global_py_pool = NULL)
{
    if (_global_pool == NULL)
    {
       if (svn_swig_py_get_parent_pool(args, $descriptor(apr_pool_t *),
                                     &_global_py_pool, &_global_pool))
       SWIG_fail;
    }

    if (PyBytes_Check($input)) {
        const char *value = PyBytes_AsString($input);
        $1 = apr_pstrdup(_global_pool, value);
    }
    else if (PyLong_Check($input)) {
        $1 = apr_palloc(_global_pool, sizeof(apr_uint32_t));
        *((apr_uint32_t *)$1) = PyLong_AsLong($input);
    }
    else if (PyInt_Check($input)) {
        $1 = apr_palloc(_global_pool, sizeof(apr_uint32_t));
        *((apr_uint32_t *)$1) = PyInt_AsLong($input);
    }
    else if ($input == Py_None) {
        $1 = NULL;
    }
    else if (svn_swig_py_convert_ptr($input, (void **)&$1,
                                     $descriptor(svn_auth_ssl_server_cert_info_t *)) == 0) {
    }
    else {
        PyErr_SetString(PyExc_TypeError, "not a known type");
        SWIG_fail;
    }
}
#endif

#ifdef SWIGPERL
%typemap(in) const void *value 
  (apr_pool_t *_global_pool = NULL)
{
    if (!SvOK($input) || $input == &PL_sv_undef) {
        $1 = NULL;
    }
    else if (SvPOK($input)) {
        if (_global_pool == NULL) {
            _global_pool = svn_swig_pl_make_pool((SV *)NULL);
            SPAGAIN;
        }
        $1 = apr_pstrdup(_global_pool, SvPV_nolen($input));
    }
    else {
        croak("Value is not a string (or undef)");
    }
}
#endif

/*
  - all values are converted to char*
  - assume the first argument is Ruby object for svn_auth_baton_t*
*/
#ifdef SWIGRUBY
%typemap(in) const void *value
{
  if (NIL_P($input)) {
    $1 = (void *)NULL;
  } else {
    VALUE _rb_pool;
    apr_pool_t *_global_pool;
    char *value = StringValuePtr($input);

    svn_swig_rb_get_pool(1, argv, Qnil, &_rb_pool, &_global_pool);
    $1 = (void *)apr_pstrdup(_global_pool, value);
  }
}
#endif

/* get */
/* assume the value is char* */
#ifdef SWIGRUBY
%typemap(out) const void *
{
  char *value = $1;
  if (value) {
    $result = rb_str_new2(value);
  } else {
    $result = Qnil;
  }
}
#endif

#ifndef SWIGRUBY
%ignore svn_auth_get_parameter;
#endif

/* -----------------------------------------------------------------------
   svn_io_parse_mimetypes_file()
*/

#ifdef SWIGRUBY
%apply apr_hash_t **HASH_CSTRING {
    apr_hash_t **type_map
}
#endif

/* -----------------------------------------------------------------------
   svn_io_detect_mimetype2()
*/

%apply apr_hash_t *HASH_CSTRING {
    apr_hash_t *mimetype_map
}

/* -----------------------------------------------------------------------
   describe how to pass a FILE* as a parameter (svn_stream_from_stdio)
*/
#ifdef SWIGPYTHON
%typemap(in) FILE * {
    $1 = svn_swig_py_as_file($input);
    if ($1 == NULL) {
        PyErr_SetString(PyExc_ValueError, "Must pass in a valid file object");
        SWIG_fail;
    }
}
#endif
#ifdef SWIGPERL
%typemap(in) FILE * {
    $1 = PerlIO_exportFILE (IoIFP (sv_2io ($input)), NULL);
}
#endif

/* -----------------------------------------------------------------------
   wrap some specific APR functionality
*/

apr_status_t apr_initialize(void);
void apr_terminate(void);

apr_status_t apr_time_ansi_put(apr_time_t *result, time_t input);

void apr_pool_destroy(apr_pool_t *p);
void apr_pool_clear(apr_pool_t *p);

apr_status_t apr_file_open_stdout (apr_file_t **out, apr_pool_t *pool);
apr_status_t apr_file_open_stderr (apr_file_t **out, apr_pool_t *pool);

/* Allow parsing of apr_errno.h without parsing apr.h. */
#define APR_DECLARE(x) x
/* Not wrapped, use svn_strerror instead. */
%ignore apr_strerror;
/* Wrap the APR status and error codes. */
/* Sigh, or not. This would mean actually having access to apr_errno.h at
   wrapper generation time, which, when rolling tarballs, the include paths
   are not currently set up to give us. FIXME. So, instead, we replicate
   one important typedef here instead.
%include apr_errno.h
*/
typedef int apr_status_t;

/* Make possible to parse the SVN_VER_NUM definition. */
#define APR_STRINGIFY_HELPER(n) #n
#define APR_STRINGIFY(n) APR_STRINGIFY_HELPER(n)

/* -----------------------------------------------------------------------
   pool functions renaming since swig doesn't take care of the #define's
*/
%rename (svn_pool_create) svn_pool_create_ex;
%ignore svn_pool_create_ex_debug;
%typemap(default) apr_allocator_t *allocator {
    $1 = NULL;
}

/* -----------------------------------------------------------------------
   Default pool handling for perl.
*/
#ifdef SWIGPERL
apr_pool_t *current_pool;

%{

/* ### Eventually this should go away. This is not thread safe and a very
   ### good example on HOW NOT TO USE pools */
static apr_pool_t *current_pool = 0;

static apr_pool_t *
core_get_current_pool (void)
{
  return current_pool;
}

static void
core_set_current_pool (apr_pool_t *pool)
{
  current_pool = pool;
}

%}

#endif

/* -----------------------------------------------------------------------
   wrap config functions
*/

#ifdef SWIGPERL
%callback_typemap(svn_config_enumerator_t callback, void *baton,
                  ,
                  svn_swig_pl_thunk_config_enumerator,
                  )
#endif

#ifndef SWIGPERL
%callback_typemap(svn_config_enumerator2_t callback, void *baton,
                  svn_swig_py_config_enumerator2,
                  ,
                  svn_swig_rb_config_enumerator)

%callback_typemap(svn_config_section_enumerator2_t callback, void *baton,
                  svn_swig_py_config_section_enumerator2,
                  ,
                  svn_swig_rb_config_section_enumerator)
#endif

/* -----------------------------------------------------------------------
  thunk the various authentication prompt functions.
  PERL NOTE: store the inputed SV in _global_callback for use in the
             later argout typemap
*/
#ifdef SWIGPERL
%define %authprompt_callback_typemap(AuthType)
%typemap(in) (svn_auth_ ## AuthType ## _prompt_func_t prompt_func,
              void *prompt_baton) {
  $1 = svn_swig_pl_thunk_ ## AuthType ## _prompt;
  $2 = $input;
  _global_callback = $input;
}
%enddef
#else
%define %authprompt_callback_typemap(AuthType)
%callback_typemap(svn_auth_ ## AuthType ## _prompt_func_t prompt_func,
                  void *prompt_baton,
                  svn_swig_py_auth_ ## AuthType ## _prompt_func,,
                  svn_swig_rb_auth_ ## AuthType ## _prompt_func)
%enddef
#endif

%authprompt_callback_typemap(simple)
%authprompt_callback_typemap(username)
%authprompt_callback_typemap(ssl_server_trust)
%authprompt_callback_typemap(ssl_client_cert)
%authprompt_callback_typemap(ssl_client_cert_pw)
%authprompt_callback_typemap(gnome_keyring_unlock)

#ifdef SWIGPYTHON
/* pl and rb aren't yet implemented */
%callback_typemap_maybenull(svn_config_auth_walk_func_t walk_func,
                            void *walk_baton,
                            svn_swig_py_config_auth_walk_func,
                            svn_swig_pl_config_auth_walk_func,
                            svn_swig_rb_config_auth_walk_func)
#endif

/* -----------------------------------------------------------------------
 * For all the various functions that set a callback baton create a reference
 * for the baton (which in this case is an SV pointing to the callback)
 * and make that a return from the function.  The perl side should
 * then store the return in the object the baton is attached to.
 * If the function already returns a value then this value is follows that
 * function.  In the case of the prompt functions auth_open_helper in Core.pm
 * is used to split up these values.
*/
#ifdef SWIGPERL
%typemap(argout) void *CALLBACK_BATON (SV * _global_callback) {
  /* callback baton */
  %append_output(sv_2mortal(newRV_inc(_global_callback)));
}

%typemap(in) void *CALLBACK_BATON (SV * _global_callback) {
  _global_callback = $input;
  $1 = (void *) _global_callback;
}

%apply void *CALLBACK_BATON {
  void *prompt_baton
};
#endif


/* -----------------------------------------------------------------------
   These APIs take an "inout" parameter that necessitates more careful
   definition.
*/
%ignore svn_mergeinfo_merge;
%ignore svn_mergeinfo_sort;
%ignore svn_rangelist_merge;
%ignore svn_rangelist_reverse;

#ifdef SWIGRUBY
%ignore svn_auth_open;
%ignore svn_diff_file_options_create;
%ignore svn_create_commit_info;
%ignore svn_commit_info_dup;

%ignore svn_opt_args_to_target_array2;
%ignore svn_opt_args_to_target_array3;
%ignore svn_opt_parse_num_args;
%ignore svn_opt_parse_all_args;
#endif

#ifdef SWIGPYTHON
/* The auth baton depends on the providers, so we preserve a
   reference to them inside the wrapper. This way, if all external
   references to the providers are gone, they will still be alive,
   keeping the baton valid.
 */
%feature("pythonappend") svn_auth_open %{
  val.__dict__["_deps"] = list(args[0])
%}
#endif

/* ----------------------------------------------------------------------- */

%include svn_error_codes_h.swg
%include svn_time_h.swg
%include svn_types_impl_h.swg
%include svn_types_h.swg
%include svn_pools_h.swg
%include svn_version_h.swg

/* The constant SVN_PROP_REVISION_ALL_PROPS is a C fragment, not a single
   data value, so the SWIG parser will raise a 305 warning if we don't
   suppress it. */
#pragma SWIG nowarn=305
%include svn_props_h.swg
#pragma SWIG nowarn=+305

%include svn_opt_impl_h.swg
%include svn_opt_h.swg
%include svn_cmdline_h.swg
%include svn_auth_h.swg
%include svn_config_h.swg
%include svn_utf_h.swg
%include svn_nls_h.swg
%include svn_path_h.swg
%include svn_dirent_uri_h.swg
%include svn_mergeinfo_h.swg
%include svn_io_h.swg
%include svn_checksum_h.swg
%include svn_cache_config_h.swg



%inline %{
/* Helper function to set the gnome-keyring unlock prompt function. This
 * C function accepts an auth baton, a function and a prompt baton, but
 * the below callback_typemap uses both the function and the prompt
 * baton, so the resulting binding has just two arguments: The auth
 * baton and the prompt function.
 * The prompt function should again have two arguments: The keyring name
 * (string) and a pool (except for the ruby version, which doesn't have
 * the pool argument). It should return the entered password (string).
 * This binding generated for this function generates a reference to the
 * prompt function that was passed into this. The caller should store
 * that reference somewhere, to prevent the function from being garbage
 * collected...
 */
static void svn_auth_set_gnome_keyring_unlock_prompt_func(svn_auth_baton_t *ab,
                                                          svn_auth_gnome_keyring_unlock_prompt_func_t prompt_func,
                                                          void *prompt_baton) {
    svn_auth_set_parameter(ab, SVN_AUTH_PARAM_GNOME_KEYRING_UNLOCK_PROMPT_FUNC,
                           prompt_func);
    svn_auth_set_parameter(ab, SVN_AUTH_PARAM_GNOME_KEYRING_UNLOCK_PROMPT_BATON,
                           prompt_baton);
}
%}

#if defined(SWIGPERL) || defined(SWIGRUBY)
%include svn_md5_h.swg
#endif

#ifdef SWIGPERL
/* The apr_file_t* 'in' typemap can't cope with struct members, and there
   is no reason to change this one. */
%immutable svn_patch_t::patch_file;

%include svn_diff_h.swg
%include svn_error_h.swg

%{
#include "svn_private_config.h"
%}
%init %{
  svn_swig_pl__bind_current_pool_fns(&core_get_current_pool,
                                     &core_set_current_pool);
%}
#endif

#ifdef SWIGPYTHON

void svn_swig_py_set_application_pool(PyObject *py_pool, apr_pool_t *pool);
void svn_swig_py_clear_application_pool();

%init %{
/* Theoretically, we should be checking for errors here,
   but I do not know of any useful way to signal an error to Python
   from within a module initialization function. */
svn_swig_py_initialize();
%}

/* Proxy classes for APR classes */
%include proxy_apr.swg

#endif

#ifdef SWIGRUBY
%init %{
  svn_swig_rb_initialize();

  rb_define_const(mCore, "SVN_ALLOCATOR_MAX_FREE_UNLIMITED",
                  UINT2NUM(APR_ALLOCATOR_MAX_FREE_UNLIMITED));
%}

%header %{
static void apr_pool_wrapper_destroy(apr_pool_wrapper_t *self);
static void apr_pool_wrapper_destroy_children(apr_pool_wrapper_t *self);
static void apr_pool_wrapper_remove_from_parent(apr_pool_wrapper_t *self);
%}

/* Dummy declaration */
struct apr_pool_wrapper_t
{
};

/* Leave memory administration to ruby's GC */
%extend apr_pool_wrapper_t
{
  static void destroy(VALUE object) {
    svn_swig_rb_destroy_internal_pool(object);
  }

  static void set_default_max_free_size(apr_size_t size) {
    apr_allocator_max_free_set(svn_swig_rb_allocator(), size);
  }

  apr_pool_wrapper_t(apr_pool_wrapper_t *parent) {
    apr_pool_wrapper_t *self;
    apr_pool_t *parent_pool;

    self = ALLOC(apr_pool_wrapper_t);
    if (parent) {
      parent_pool = parent->pool;
      APR_ARRAY_PUSH(parent->children, apr_pool_wrapper_t *) = self;
    } else {
      parent_pool = svn_swig_rb_pool();
    }
    self->pool = svn_pool_create_ex(parent_pool, NULL);
    self->destroyed = FALSE;
    self->parent = parent;
    self->children = apr_array_make(self->pool, 0,
                                    sizeof(apr_pool_wrapper_t *));
    return self;
  }

  ~apr_pool_wrapper_t() {
    apr_pool_wrapper_destroy(self);
    xfree(self);
  }

  void set_max_free_size(apr_size_t size) {
    apr_allocator_t *allocator;

    allocator = apr_pool_allocator_get(self->pool);
    apr_allocator_max_free_set(allocator, size);
  }

  void _destroy(void) {
    apr_pool_wrapper_destroy(self);
  }
};

%ignore apr_pool_wrapper_destroy;
%ignore apr_pool_wrapper_destroy_children;
%ignore apr_pool_wrapper_remove_from_parent;
%inline %{
static void
apr_pool_wrapper_destroy(apr_pool_wrapper_t *self)
{
  if (!self->destroyed) {
    self->destroyed = TRUE;
    apr_pool_wrapper_destroy_children(self);
    apr_pool_wrapper_remove_from_parent(self);
    apr_pool_destroy(self->pool);
  }
}

static void
apr_pool_wrapper_destroy_children(apr_pool_wrapper_t *self)
{
  apr_pool_wrapper_t **child;

  while ((child = apr_array_pop(self->children))) {
    if (*child) {
      apr_pool_wrapper_destroy(*child);
    }
  }
}

static void
apr_pool_wrapper_remove_from_parent(apr_pool_wrapper_t *self)
{
  if (self->parent) {
    apr_pool_wrapper_t *child;
    int i, len;

    len = self->parent->children->nelts;
    for (i = 0; i < len; i++) {
      child = APR_ARRAY_IDX(self->parent->children, i, apr_pool_wrapper_t *);
      if (child == self) {
        APR_ARRAY_IDX(self->parent->children, i, apr_pool_wrapper_t *) = NULL;
        self->parent = NULL;
        break;
      }
    }
  }
}
%}

/* Dummy declaration */
struct svn_stream_t
{
};

%extend svn_stream_t
{
  svn_stream_t(VALUE io) {
    return svn_swig_rb_make_stream(io);
  };

  ~svn_stream_t() {
  };
}

/* Dummy declaration */
struct svn_auth_baton_t
{
};

%extend svn_auth_baton_t
{
  svn_auth_baton_t(apr_array_header_t *providers, apr_pool_t *pool) {
    svn_auth_baton_t *self;
    svn_auth_open(&self, providers, pool);
    return self;
  };

  ~svn_auth_baton_t() {
  };
}

%extend svn_diff_file_options_t
{
  svn_diff_file_options_t(apr_pool_t *pool) {
    return svn_diff_file_options_create(pool);
  };

  ~svn_diff_file_options_t() {
  };
}

%extend svn_commit_info_t
{
  svn_commit_info_t(apr_pool_t *pool) {
    return svn_create_commit_info(pool);
  };

  ~svn_commit_info_t() {
  };

  svn_commit_info_t *dup(apr_pool_t *pool) {
    return svn_commit_info_dup(self, pool);
  };
}

%extend svn_merge_range_t
{
  svn_merge_range_t(svn_revnum_t start, svn_revnum_t end,
                    svn_boolean_t inheritable, apr_pool_t *pool) {
    svn_merge_range_t *self;
    self = apr_palloc(pool, sizeof(svn_merge_range_t));
    self->start = start;
    self->end = end;
    self->inheritable = inheritable;
    return self;
  };

  ~svn_merge_range_t() {
  };

  svn_merge_range_t *dup(apr_pool_t *pool) {
    return svn_merge_range_dup(self, pool);
  };
}

%include svn_diff_h.swg

%inline %{
static VALUE
svn_default_charset(void)
{
  return PTR2NUM(APR_DEFAULT_CHARSET);
}

static VALUE
svn_locale_charset(void)
{
  return PTR2NUM(APR_LOCALE_CHARSET);
}

/* prompt providers return baton for protecting GC */
static VALUE
svn_swig_rb_auth_get_simple_prompt_provider(
  svn_auth_provider_object_t **provider,
  svn_auth_simple_prompt_func_t prompt_func,
  void *prompt_baton,
  int retry_limit,
  apr_pool_t *pool)
{
  svn_auth_get_simple_prompt_provider(provider, prompt_func, prompt_baton,
                                      retry_limit, pool);
  return rb_ary_new3(1, (VALUE)prompt_baton);
}

static VALUE
svn_swig_rb_auth_get_ssl_client_cert_prompt_provider(
  svn_auth_provider_object_t **provider,
  svn_auth_ssl_client_cert_prompt_func_t prompt_func,
  void *prompt_baton,
  int retry_limit,
  apr_pool_t *pool)
{
  svn_auth_get_ssl_client_cert_prompt_provider(provider, prompt_func,
                                               prompt_baton, retry_limit,
                                               pool);
  return rb_ary_new3(1, (VALUE)prompt_baton);
}

static VALUE
svn_swig_rb_auth_get_ssl_client_cert_pw_prompt_provider(
  svn_auth_provider_object_t **provider,
  svn_auth_ssl_client_cert_pw_prompt_func_t prompt_func,
  void *prompt_baton,
  int retry_limit,
  apr_pool_t *pool)
{
  svn_auth_get_ssl_client_cert_pw_prompt_provider(provider, prompt_func,
                                                  prompt_baton, retry_limit,
                                                  pool);
  return rb_ary_new3(1, (VALUE)prompt_baton);
}

static VALUE
svn_swig_rb_auth_get_ssl_server_trust_prompt_provider(
  svn_auth_provider_object_t **provider,
  svn_auth_ssl_server_trust_prompt_func_t prompt_func,
  void *prompt_baton,
  apr_pool_t *pool)
{
  svn_auth_get_ssl_server_trust_prompt_provider(provider, prompt_func,
                                                prompt_baton, pool);
  return rb_ary_new3(1, (VALUE)prompt_baton);
}

static VALUE
svn_swig_rb_auth_get_username_prompt_provider(
  svn_auth_provider_object_t **provider,
  svn_auth_username_prompt_func_t prompt_func,
  void *prompt_baton,
  int retry_limit,
  apr_pool_t *pool)
{
  svn_auth_get_username_prompt_provider(provider, prompt_func, prompt_baton,
                                        retry_limit, pool);
  return rb_ary_new3(1, (VALUE)prompt_baton);
}
%}
#endif

#if defined(SWIGPYTHON) || defined(SWIGRUBY)
%inline %{
static svn_error_t *
svn_swig_mergeinfo_merge(apr_hash_t **mergeinfo_inout,
                         apr_hash_t *changes,
                         apr_pool_t *pool)
{
  return svn_mergeinfo_merge(*mergeinfo_inout, changes, pool);
}

static svn_error_t *
svn_swig_mergeinfo_sort(apr_hash_t **mergeinfo_inout, apr_pool_t *pool)
{
  return svn_mergeinfo_sort(*mergeinfo_inout, pool);
}

static svn_error_t *
svn_swig_rangelist_merge(svn_rangelist_t **rangelist_inout,
                         svn_rangelist_t *changes,
                         apr_pool_t *pool)
{
  return svn_rangelist_merge(rangelist_inout, changes, pool);
}

static svn_error_t *
svn_swig_rangelist_reverse(svn_rangelist_t **rangelist_inout,
                           apr_pool_t *pool)
{
  return svn_rangelist_reverse(*rangelist_inout, pool);
}
%}
#endif
