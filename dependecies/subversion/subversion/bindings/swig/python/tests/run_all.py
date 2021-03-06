#
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#
#
import sys
import unittest, setup_path
import mergeinfo, core, client, delta, checksum, pool, fs, ra, wc, repository, \
       auth, trac.versioncontrol.tests, typemap
from svn.core import svn_cache_config_get, svn_cache_config_set

# Run all tests

def suite():
  """Run all tests"""
  settings = svn_cache_config_get()
  if sys.hexversion < 0x3000000:
    settings.cache_size = long(1024*1024*32) ### Need explicit long
  else:
    settings.cache_size = 1024*1024*32
  svn_cache_config_set(settings)
  s = unittest.TestSuite()
  s.addTest(core.suite())
  s.addTest(checksum.suite())
  s.addTest(mergeinfo.suite())
  s.addTest(client.suite())
  s.addTest(delta.suite())
  s.addTest(pool.suite())
  s.addTest(fs.suite())
  s.addTest(ra.suite())
  s.addTest(wc.suite())
  s.addTest(repository.suite())
  s.addTest(auth.suite())
  s.addTest(trac.versioncontrol.tests.suite())
  s.addTest(typemap.suite())
  return s

if __name__ == '__main__':
  unittest.main(defaultTest='suite')
