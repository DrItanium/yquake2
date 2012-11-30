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
#include "quake2_interact.h" 
#include "router.h"
#include "../header/local.h"

static int FindQuake(void *,char *);
static int GetcQuake(void *,char *);
static int UngetcQuake(void *,int,char *);
static int ExitQuake(void *,int);
static int PrintQuake(void *,char *,char *);

extern void QuakeInteractionDefinitions(void* theEnv) {
   EnvAddRouter(theEnv, "quake", 40, FindQuake, PrintQuake,
         GetcQuake, UngetcQuake, ExitQuake);
   gi.dprintf("Created Quake IO Router\n");
}

static int FindQuake(void *theEnv, char* logicalName) {

   if ( (strcmp(logicalName,"stdout") == 0) ||
         (strcmp(logicalName, "quake") == 0) ||
         (strcmp(logicalName,WERROR) == 0))
   { return(TRUE); }
   return(FALSE);
}

static int PrintQuake(void *theEnv, char* logicalName, char* str) {
   gi.dprintf("%s", str);  
   if((strcmp(logicalName,"quake") != 0)) {
      EnvDeactivateRouter(theEnv,"quake");
      EnvPrintRouter(theEnv,logicalName,str);
      EnvActivateRouter(theEnv, "quake");
   }
   return (1);
}

static int GetcQuake(void* theEnv, char* logicalName) {
   int rv;

   if((strcmp(logicalName, "quake") != 0)) {
      EnvDeactivateRouter(theEnv, "quake");
      rv = EnvGetcRouter(theEnv, logicalName);
      EnvActivateRouter(theEnv, "quake");
   } 
   return rv;
}

static int UngetcQuake(void* theEnv, int ch, char* logicalName) {
   int rv;
   //do we need this? Don't think so
   EnvDeactivateRouter(theEnv, "quake");
   rv = EnvUngetcRouter(theEnv, ch, logicalName);
   EnvActivateRouter(theEnv, "quake");

   return rv;
}

static int ExitQuake(void* theEnv, int num) {
   gi.dprintf("Deactivating Quake IO Router\n");

   return (1);
}
