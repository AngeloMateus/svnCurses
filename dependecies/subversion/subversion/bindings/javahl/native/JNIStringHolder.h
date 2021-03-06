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
 * @file JNIStringHolder.h
 * @brief Interface of the class JNIStringHolder
 */

#ifndef JNISTRINGHOLDER_H
#define JNISTRINGHOLDER_H

#include <jni.h>
#include <apr_pools.h>

class JNIStringHolder
{
 public:
  JNIStringHolder(jstring jtext);
  ~JNIStringHolder();
  operator const char *() { return m_str; }
  const char* c_str() const { return m_str; }
  const char *pstrdup(apr_pool_t *pool);

 protected:
  const char *m_str;
  JNIEnv *m_env;
  jstring m_jtext;
};

#endif  // JNISTRINGHOLDER_H
