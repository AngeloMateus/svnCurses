@ECHO off
REM ================================================================
REM   Licensed to the Apache Software Foundation (ASF) under one
REM   or more contributor license agreements.  See the NOTICE file
REM   distributed with this work for additional information
REM   regarding copyright ownership.  The ASF licenses this file
REM   to you under the Apache License, Version 2.0 (the
REM   "License"); you may not use this file except in compliance
REM   with the License.  You may obtain a copy of the License at
REM
REM     http://www.apache.org/licenses/LICENSE-2.0
REM
REM   Unless required by applicable law or agreed to in writing,
REM   software distributed under the License is distributed on an
REM   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
REM   KIND, either express or implied.  See the License for the
REM   specific language governing permissions and limitations
REM   under the License.
REM ================================================================

SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

CALL ..\svn-config.cmd
IF ERRORLEVEL 1 EXIT /B 1


SET MODE=-d
SET PARALLEL=
SET ARGS=

SET FSFS=
SET LOCAL=
SET RC=0
:next

IF "%1" == "-r" (
    SET MODE=-r
    SHIFT
) ELSE IF "%1" == "-d" (
    SET MODE=-d
    SHIFT
) ELSE IF "%1" == "-p" (
    SET PARALLEL=-p
    SHIFT
) ELSE IF "%1" == "fsfs" (
    SET FSFS=1
    SHIFT
) ELSE IF "%1" == "local" (
    SET LOCAL=1
    SHIFT
) ELSE IF "%1" == "svn" (
    SET SVN=1
    SHIFT
) ELSE IF "%1" == "serf" (
    SET DAV=1
    SHIFT
) ELSE IF "%1" == "dav" (
    SET DAV=1
    SHIFT
) ELSE (
    SET ARGS=!ARGS! -t %1
    SHIFT
)

IF NOT "%1" == "" GOTO next

taskkill /im svnserve.exe httpd.exe /f 2> nul:

IF "%SVN_BRANCH%" LSS "1.9." (
  IF NOT EXIST "%TESTDIR%\bin" MKDIR "%TESTDIR%\bin"
  xcopy /y /i ..\deps\release\bin\*.dll "%TESTDIR%\bin"

  PATH %TESTDIR%\bin;!PATH!
)

IF "%LOCAL%+%FSFS%" == "1+1" (
  echo win-tests.py -c %PARALLEL% %MODE% -f fsfs %ARGS% "%TESTDIR%\tests"
  win-tests.py -c %PARALLEL% %MODE% -f fsfs %ARGS% "%TESTDIR%\tests"
  IF ERRORLEVEL 1 SET RC=1
)

IF "%SVN%+%FSFS%" == "1+1" (
  echo win-tests.py -c %PARALLEL% %MODE% -f fsfs -u svn://127.0.0.1 %ARGS% "%TESTDIR%\tests"
  win-tests.py -c %PARALLEL% %MODE% -f fsfs -u svn://127.0.0.1 %ARGS% "%TESTDIR%\tests"
  IF ERRORLEVEL 1 SET RC=1
)

IF "%DAV%+%FSFS%" == "1+1" (
  echo win-tests.py -c %PARALLEL% %MODE% -f fsfs --httpd-no-log --httpd-dir "%CD%\..\deps\release\httpd" --httpd-port %TESTPORT% -u http://127.0.0.1:%TESTPORT% %ARGS% "%TESTDIR%\tests"
  win-tests.py -c %PARALLEL% %MODE% -f fsfs --httpd-no-log --httpd-dir "%CD%\..\deps\release\httpd" --httpd-port %TESTPORT% -u http://127.0.0.1:%TESTPORT% %ARGS% "%TESTDIR%\tests"
  IF ERRORLEVEL 1 SET RC=1
)

IF EXIST "%TEMP%\svn-*" (
  echo "SVN Files left:"
  dir "%TEMP%"
)
EXIT /B %RC%
