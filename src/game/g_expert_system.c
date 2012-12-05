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
 * =======================================================================
 *
 * Commands that allow interaction with the expert system
 *
 * =======================================================================
 */

#include "header/local.h"
#include "clips/clips.h"

game_locals_t game;
level_locals_t level;
game_import_t gi;
game_export_t globals;
spawn_temp_t st;

void Cmd_Ex_Eval_f(edict_t *ent) {
   int max;
   char* name;
   char* tmpStr;
   long tmpLong;
   double tmpDouble;
   int type;
   DATA_OBJECT obj;
   name = gi.args();
   //we need to strip out the quotation marks
   max = strlen(name);
   if(max > 0) {
      if(name[0] == '\"') {
         name[0] = ' ';
      } 
      if(name[max - 1] == '\"') {
         name[max - 1] = ' ';
      }
   }
   if(EnvEval(game.rhizome, name, &obj) == 0) {
      gi.dprintf("ERROR! Evaluating %s failed\n", name);
      return;
   } else {
      type = GetType(obj);
      if(type == INTEGER) {
         tmpLong = DOToLong(obj);
         gi.dprintf("%d\n", tmpLong);
         return;
      } else if(type == FLOAT) {
         tmpDouble = DOToDouble(obj);
         gi.dprintf("%f\n", tmpDouble);
         return;
      } else if(type == MULTIFIELD) {
         gi.dprintf("multifield\n");
         return;
      } else if(type == INSTANCE_ADDRESS) {
         gi.dprintf("instance-address\n");
         return;
      } else if(type == EXTERNAL_ADDRESS) {
         gi.dprintf("external-address\n");
         return;
      } else if (type == FACT_ADDRESS) {
         gi.dprintf("fact-address\n");
         return;
      } else {
         //STRING, SYMBOL, or INSTANCE_NAME
         tmpStr = DOToString(obj); 
         if(Q_stricmp(tmpStr, "FALSE") != 0) {
            gi.dprintf("%s\n", tmpStr);
         }
      }
   }
}
void Cmd_Ex_Build_f(edict_t *ent) {
   int max;
   char *name;
   name = gi.args();
   max = strlen(name);
   if(max > 0) {
      if(name[0] == '\"') {
         name[0] = ' ';
      } 
      if(name[max - 1] == '\"') {
         name[max - 1] = ' ';
      }
   }
   if(EnvBuild(game.rhizome, name) == 0) {
      gi.dprintf("ERROR! Building %s failed\n", name);
   } 
}

void Cmd_Ex_Run_f(edict_t *ent) {
   char *name;
   int result;
   name = gi.args();
   if(strlen(name) == 0) {
      EnvRun(game.rhizome, -1L);
   } else {
      result = atoi(name); 
      if(result <= 0) {
         EnvRun(game.rhizome, -1L);
      } else {
         EnvRun(game.rhizome, result);
      }
   }
}

void Cmd_Ex_Batch_f(edict_t *ent) {
   int max;
   char* name;
   name = gi.args();
   max = strlen(name);
   if(max > 0) {
      if(name[0] == '\"') {
         name[0] = ' ';
      } 
      if(name[max - 1] == '\"') {
         name[max - 1] = ' ';
      }
   }
   if(max == 0) {
      gi.dprintf("A file is needed for this to load properly\n");
      return;
   } else if(EnvBatchStar(game.rhizome, name) == 0) {
      gi.dprintf("ERROR! Couldn't batch %s \n", name);
   } 
}

void Cmd_Ex_Facts_f(edict_t *ent) {
   EnvFacts(game.rhizome, "quake", NULL, -1L, -1L, -1L);
}
void Cmd_Ex_Instances_f(edict_t *ent) {
   EnvInstances(game.rhizome, "quake", NULL, NULL, 1);
}

void SetupEnvironment(edict_t *ent) {
   char instance[512];
   char privateInstance[512];
   char instanceRhizome[512];
   ent->privateEnv = CreateEnvironment();
   ent->publicEnv = game.rhizome;
   EnvBatchStar(ent->privateEnv, "expert/logic/Init.clp");
   Com_sprintf(instance, sizeof(instance), "( of Environment (pointer %llu) (target %llu))",
         ent->privateEnv, ent);
   Com_sprintf(privateInstance, sizeof(privateInstance), "(env of Environment (pointer %llu) (target %llu))",
         ent->privateEnv, ent);
   Com_sprintf(instanceRhizome, sizeof(instanceRhizome), "(rhizome of Environment (pointer %llu))",
         ent->publicEnv);
   EnvMakeInstance(ent->privateEnv, instanceRhizome);
   EnvMakeInstance(ent->privateEnv, privateInstance);
   EnvMakeInstance(ent->publicEnv, instance);
}

void CreateExpertSystemRepresentation(edict_t *ent, qboolean isMonster) {
   if(isMonster) {
      char privateMonsterInstance[2048];
      char publicMonsterInstance[2048];
      Com_sprintf(publicMonsterInstance, sizeof(publicMonsterInstance), 
            "( of QEdict (pointer %llu) (health %d) (gib-health %d) (max-health %d))",
            ent, ent->health, ent->gib_health, ent->health);
      Com_sprintf(privateMonsterInstance, sizeof(privateMonsterInstance), 
            "(self of QEdict (pointer %llu) (health %d) (gib-health %d) (max-health %d))",
            ent, ent->health, ent->gib_health, ent->health);
      EnvMakeInstance(ent->privateEnv, privateMonsterInstance);
      EnvMakeInstance(ent->publicEnv, publicMonsterInstance);
   } else {
      //don't do anything right now 
   }
}
typedef long long PointerAddress;
extern void Quake_Env_Eval(void* theEnv) {
   DATA_OBJECT arg0;
   DATA_OBJECT arg1;
   DATA_OBJECT result;
   void* targetEnv;
   char* str;
   if((EnvArgCountCheck(theEnv, "quake-env-eval", EXACTLY, 2) == -1)) {
      return;
   } 
   if((EnvArgTypeCheck(theEnv, "quake-env-eval", 1, INTEGER, &arg0) == 0)) {
      return;
   }

   if((EnvArgTypeCheck(theEnv, "quake-env-eval", 2, STRING, &arg1) == 0)) {
      return;
   }
   targetEnv = (void*)(PointerAddress)DOToLong(arg0);
   str = DOToString(arg1);
   EnvEval(targetEnv, str, &result);
}

extern void Quake_Env_Build(void* theEnv) {
   DATA_OBJECT arg0;
   DATA_OBJECT arg1;
   void* targetEnv;
   char* str;
   if((EnvArgCountCheck(theEnv, "quake-env-build", NO_MORE_THAN, 2) == -1)) {
      return;
   }  
   if((EnvArgTypeCheck(theEnv, "quake-env-build", 1, INTEGER, &arg0) == 0)) {
      return;
   }
   if((EnvArgTypeCheck(theEnv, "quake-env-build", 2, STRING, &arg1) == 0)) {
      return;
   }
   targetEnv = (void*)(PointerAddress)DOToLong(arg0);
   str = DOToString(arg1);
   EnvBuild(targetEnv, str);
}

extern void Quake_Env_Run(void* theEnv) {
   DATA_OBJECT arg0;
   DATA_OBJECT arg1;
   void* targetEnv;
   PointerAddress duration;
   if((EnvArgCountCheck(theEnv, "quake-env-run", NO_MORE_THAN, 2) == -1)) {
      if((EnvArgCountCheck(theEnv, "quake-env-run", EXACTLY, 2) == -1)) {
         if((EnvArgTypeCheck(theEnv, "quake-env-run", 1, INTEGER, &arg0) == 0)) {
            return;
         }

         if((EnvArgTypeCheck(theEnv, "quake-env-run", 2, INTEGER, &arg1) == 0)) {
            return;
         }
         targetEnv = (void*)(PointerAddress)DOToLong(arg0);
         duration = (PointerAddress)DOToLong(arg1);
         EnvRun(targetEnv, duration);
      } else if((EnvArgCountCheck(theEnv, "quake-env-run", EXACTLY, 1) == -1)) {
         if((EnvArgTypeCheck(theEnv, "quake-env-run", 1, INTEGER, &arg0) == 0)) {
            return;
         } 
         targetEnv = (void*)(PointerAddress)DOToLong(arg0);
         EnvRun(targetEnv, -1L);
      }
   } else {
      return; 
   }
}

/* Add declarations of clips => quake2 functions here */
extern void SetupQuakeEnvFunctions(void* theEnv) {
   EnvDefineFunction2(theEnv, "quake-env-run", 'v', PTIEF Quake_Env_Run, "Quake_Env_Run" , "12i");
   EnvDefineFunction2(theEnv, "quake-env-build", 'v', PTIEF Quake_Env_Build, "Quake_Env_Build" , "22iis");
   EnvDefineFunction2(theEnv, "quake-env-eval", 'v', PTIEF Quake_Env_Eval, "Quake_Env_Eval" , "22iis");
}
