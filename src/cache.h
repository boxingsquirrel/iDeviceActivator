/*
 * cache.h
 * Load data from/add data to the activation info cache.
 *
 * Copyright (c) 2010 boxingsquirrel. All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <plist/plist.h>

#define BUFSIZE 0x200000

extern char* cachedir;
extern int use_cache;
extern int backup_to_cache;

extern int write_file(const char *filename, char data[BUFSIZE]);
extern int cache(const char *fname, const char *what);
extern int cache_plist(const char *fname, plist_t plist);
extern char* get_from_cache(const char *what);

extern void cache_warning();

extern int check_cache(lockdownd_client_t c);
