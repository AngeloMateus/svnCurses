/**
 * @copyright
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
 * @endcopyright
 *
 * @file ImportFilterCallback.h
 * @brief Interface of the class ImportFilterCallback
 */

#ifndef IMPORT_FILTER_CALLBACK_H
#define IMPORT_FILTER_CALLBACK_H

#include <jni.h>
#include "svn_client.h"

/**
 * This class holds a Java callback object, which will receive every
 * directory entry for which the callback information is requested.
 */
class ImportFilterCallback
{
public:
  ImportFilterCallback(jobject jcallback);
  ~ImportFilterCallback();

  static svn_error_t *callback(void *baton,
                               svn_boolean_t *filtered,
                               const char *local_abspath,
                               const svn_io_dirent2_t *dirent,
                               apr_pool_t *scratch_pool);

protected:
  svn_error_t *doImportFilter(svn_boolean_t *filtered,
                              const char *local_abspath,
                              const svn_io_dirent2_t *dirent,
                              apr_pool_t *pool);

private:
  /**
   * This a local reference to the Java object.
   */
  jobject m_callback;
};

#endif // IMPORT_FILTER_CALLBACK_H
