/*
	main.h
*/

#ifndef _MAIN_H
#define _MAIN_H

#define VERSION	"0.1.76"

#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include "action.h"
#include "environment.h"
#include "session.h"

enum status_type {
   st_none,
   st_error,
   st_pending,
   st_done,
   st_found,
   st_nl
};

extern uid_t ruid;
extern uid_t euid;

extern gid_t rgid;
extern gid_t egid;

#endif
