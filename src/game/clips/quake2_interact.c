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
#include "quake2_interact.h" 
#include "../header/local.h"

game_locals_t game;
level_locals_t level;
game_import_t gi;
game_export_t globals;
spawn_temp_t st;
static int FindQuake(void *,char *);
static int ExitQuake(void *,int);
static int PrintQuake(void *,char *,char *);

extern void QuakeInteractionDefinitions(void* theEnv) {
   EnvAddRouter(theEnv, "quake", 40, FindQuake, PrintQuake,
         NULL, NULL, //doesn't handle input
         ExitQuake);
   gi.dprintf("Created IO Router for Quake Console\n");
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

static int ExitQuake(void* theEnv, int num) {
   gi.dprintf("Deactivating IO Router for Quake Console\n");

   return (1);
}
