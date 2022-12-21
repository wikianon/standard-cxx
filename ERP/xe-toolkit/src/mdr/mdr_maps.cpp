/****************************************************************************
**
** Copyright (c) 2006-2007 Captive Metrics Software Corporation.
**                    All rights reserved.
**
** This file is part of the mdr library of Captivity.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE-GPL.txt contained within the
** same package as this file. This software is subject to a
** dual-licensing mechanism, the details of which are outlined in
** file LICENSE-DUAL.txt, also contained within this package. Be sure
** to use the correct license for your needs. To view the commercial
** license, read LICENSE-COMMERCIAL.txt also contained within this
** package.
**
** If you do not have access to these files or are unsure which license
** is appropriate for your use, please contact the sales department at
** sales@captivemetrics.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef _REENTRANT
# define _REENTRANT
#endif
#include <cstring>
#include <fstream>
#include <dirent.h>
#include "mdr.h"
#include "mdr_maps.h"
#include "mdr_exception.h"

using namespace mdr;

////////////////////////////////////////////////////////////////////
// line_map code
////////////////////////////////////////////////////////////////////

void line_map::assign(char *line, const char *delim)
{
  char *p;
  char *pp;

  local_storage = strdup(line);
  token_count = 0;
  for(p=strtok_r(local_storage, delim, &pp); p; p=strtok_r(0, delim, &pp))
    (*this)[token_count++].assign(p);
}

////////////////////////////////////////////////////////////////////
// file_map code
////////////////////////////////////////////////////////////////////

void file_map::assign(const char *file_name, const char *delim)
{
  std::ifstream input(file_name);
  char buf[BUFSIZ];

  if (!input) {
    file_exists = false;
    return;
  }

  file_exists = true;

  for(line_count=0; !input.getline(buf, sizeof buf).eof(); line_count++)
    (*this)[line_count].assign(buf, delim);
}

file_map::file_map(const char *file_name, const char *delim)
{
  assign(file_name, delim);
}

file_map::file_map(const char *file_name)
{
  assign(file_name, " \t");
}

line_map &file_map::operator[](const char *token0)
{
  for(int i=0; i<line_count; i++) {
    char *token = (*this)[i][0].token();

    if (token && (strcmp(token, token0) == 0))
      return (*this)[i];
  }

  throw mdr_exception_t(
    "file_map::operator[](const char *): %s not in file_map", token0);
}

line_map &file_map::find(const char *token0, int instance)
{
  int found = 0;

  for(int i=0; i<line_count; i++) {
    char *token = (*this)[i][0].token();

    if (token && (strcmp(token, token0) == 0))
      if (found++ == instance)
        return (*this)[i];
  }

  throw mdr_exception_t(
    "file_map::operator[](const char *): %s not in file_map", token0);
}

////////////////////////////////////////////////////////////////////
// dir_list_t code
////////////////////////////////////////////////////////////////////

dir_list_t::dir_list_t(const char *dir_name, bool include_dots)
{
  DIR *dirp;
  struct dirent *dp;

  dirp = opendir(dir_name);
  if (dirp == 0) {
    open_succeeded = false;
    return;
  }
  open_succeeded = true;
  while((dp = readdir(dirp)) != 0) {
    if (!include_dots &&
      ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0)))
      continue;
    insert((const char *) &dp->d_name[0]);
  }
  closedir(dirp);
}

bool dir_list_t::operator!(void)
{
  return !open_succeeded;
}

bool dir_list_t::operator()(void)
{
  return open_succeeded;
}
