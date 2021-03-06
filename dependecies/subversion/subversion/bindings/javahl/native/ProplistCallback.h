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
 * @file ProplistCallback.h
 * @brief Interface of the class ProplistCallback
 */

#ifndef PROPLISTCALLBACK_H
#define PROPLISTCALLBACK_H

#include <jni.h>
#include "svn_client.h"

/**
 * This class holds a Java callback object, which will receive every
 * property from a SVNClientInterface.properties call.
 */
class ProplistCallback
{
 public:
  ProplistCallback(jobject jcallback, bool inherited);
  ~ProplistCallback();

  static svn_error_t *callback(void *baton,
                               const char *path,
                               apr_hash_t *prop_hash,
                               apr_array_header_t *inherited_props,
                               apr_pool_t *pool);

  bool inherited() const { return m_inherited; }

 protected:
  svn_error_t *singlePath(const char *path,
                          apr_hash_t *prop_hash,
                          apr_pool_t *pool);

  svn_error_t *singlePath(const char *path,
                          apr_hash_t *prop_hash,
                          apr_array_header_t *inherited_props,
                          apr_pool_t *pool);
 private:
  /**
   * This a local reference to the Java object.
   */
  jobject m_callback;
  bool m_inherited;
};


#endif  // PROPLISTCALLBACK_H
