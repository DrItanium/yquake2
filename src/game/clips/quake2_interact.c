/*
 * Copyright (C) 2012 Joshua Scoggins 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 */
#include "clips.h"
#include "libmaya.h"

#if QUAKE_EXTENSIONS
#include "../header/local.h"
static int FindQuake(void *,const char *);
static int ExitQuake(void *,int);
static int PrintQuake(void *,const char *, const char *);
#define QUAKE_CONSOLE "quake"
extern void QuakeExtensionFunctions(void* theEnv) {
   EnvAddRouter(theEnv, QUAKE_CONSOLE, 40, FindQuake, PrintQuake,
         NULL, 
		 NULL, //doesn't handle input
         ExitQuake);
   gi.dprintf("Created IO Routers for Quake Console\n");
}

static int FindQuake(void *theEnv, const char* logicalName) {

   if ( (strcmp(logicalName,"stdout") == 0) ||
         (strcmp(logicalName, QUAKE_CONSOLE) == 0) ||
         (strcmp(logicalName, WDISPLAY) == 0) ||
         (strcmp(logicalName,WERROR) == 0))
   { return(TRUE); }
   return(FALSE);
}

static int PrintQuake(void *theEnv, const char* logicalName, const char* str) {
   gi.dprintf("%s", str);  
   if((strcmp(logicalName,QUAKE_CONSOLE) != 0)) {
      EnvDeactivateRouter(theEnv,QUAKE_CONSOLE);
      EnvPrintRouter(theEnv,logicalName,str);
      EnvActivateRouter(theEnv, QUAKE_CONSOLE);
   }
   return (1);
}

static int ExitQuake(void* theEnv, int num) {
   gi.dprintf("Deactivating IO Router for Quake Console\n");

   return (1);
}
#endif // QUAKE_EXTENSIONS
