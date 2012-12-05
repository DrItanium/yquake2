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
#define IfNullEmpty(a) => ( a ) ? a : "" 
void CreateExpertSystemRepresentation(edict_t *ent, qboolean isMonster) {
   if(isMonster) {
      /* create the instance string once but keep the name field as blank to
       * allow it to be copied out as need be */
      char tmpRepresentation[2048];
      char privateMonsterInstance[2048];
      char publicMonsterInstance[2048];
      Com_sprintf(tmpRepresentation, sizeof(tmpRepresentation),
            "(%s of QEdict (pointer %llu)"
            "(move-type %u) (flags %u) (model \"%s\") (free-time %f)"
            "(message \"%s\") (classname %s) (spawn-flags %u) (time-stamp %f)"
            "(angle %f) (target %s) (target-name %s) (kill-target %s) (team %s)"
            "(path-target %s) (death-target %s) (combat-target %s) "
            "(target-entity %llu) (speed %f) (acceleration %f) "
            "(deceleration %f) (move-direction %d %d %d) (position1 %d %d %d) "
            "(position2 %d %d %d) (velocity %d %d %d) (avelocity %d %d %d) "
            "(mass %u) (air-finished %f) (gravity %d) (goal-entity %llu) "
            "(move-target %llu) (yaw-speed %f) (ideal-yaw %f) (next-think %f) "
            "(function-pre-think %llu) (function-think %llu) "
            "(function-blocked %llu) (function-touched %llu) (function-use %llu)"
            "(function-pain %llu) (function-die %llu) (touch-debounce-time %f) "
            "(pain-debounce-time %f) (damage-debounce-time %f) "
            "(fly-sound-debounce-time %f) (last-move-time %f) (health %d) "
            "(max-health %d) (gib-health %d) (is-dead FALSE) "
            "(power-armor-time %f) (map \"%s\") (view-height %d) "
            "(take-damage %d) (damage %d) (radius-damage %d) (damage-radius %f) (sounds %d) "
            "(count %d) (chain %llu) (enemy %llu) (oldenemy %llu) "
            "(activator %llu) (ground-entity %llu) (ground-entity-link-count %d) "
            "(team-chain %llu) (team-master %llu) (my-noise %llu) " 
            "(my-noise2 %llu) (noise-index %d) (noise-index2 %d) (volume %f) "
            "(attenuation %f) (wait %f) (delay %f) (random %f) " 
            "(teleport-time %f) (water-type %d) (water-level %d) "
            "(move-origin %d %d %d) (move-angles %d %d %d) (light-level %d) "
            "(style %d) (item %llu) (move-info %llu) (monster-info %llu))", 
         "%s", 
         ent, 
         ent->movetype, 
         ent->flags, 
         IfNullEmpty(ent->model),
         ent->freetime,
         IfNullEmpty(ent->message),
         IfNullEmpty(ent->classname),
         ent->spawnflags, 
         ent->timestamp, 
         ent->angle, 
         ent->target, 
         IfNullEmpty(ent->targetname),
         IfNullEmpty(ent->killtarget), 
         IfNullEmpty(ent->team),
         IfNullEmpty(ent->pathtarget), 
         IfNullEmpty(ent->deathtarget), 
         IfNullEmpty(ent->combattarget), 
         ent->target_ent,
         ent->speed, 
         ent-> accel, 
         ent->decel, 
         ent->movedir[0], ent->movedir[1], ent->movedir[2],
         ent->pos1[0], ent->pos1[1], ent->pos1[2],
         ent->pos2[0], ent->pos2[1], ent->pos2[2],
         ent->velocity[0], ent->velocity[1], ent->velocity[2],
         ent->avelocity[0], ent->avelocity[1], ent->avelocity[2],
         ent->mass, 
         ent->air_finished, 
         ent->gravity, 
         ent->goalentity,
         ent->movetarget, 
         ent->yaw_speed, 
         ent->ideal_yaw, 
         ent->nextthink,
         ent->prethink, 
         ent->think, 
         ent->blocked, 
         ent->touch, 
         ent->use, 
         ent->pain, 
         ent->die, 
         ent->touch_debounce_time, 
         ent->pain_debounce_time,
         ent->damage_debounce_time, 
         ent->fly_sound_debounce_time, 
         ent->last_move_time,
         ent->health, 
         ent->max_health, 
         ent->gib_health, 
         ent->powerarmor_time,
         IfNullEmpty(ent->map), 
         ent->viewheight, 
         ent->takedamage, 
         ent->dmg, 
         ent->radius_dmg,
         ent->dmg_radius, 
         ent->sounds, 
         ent->count, 
         ent->chain, 
         ent->enemy, 
         ent->oldenemy, 
         ent->activator, 
         ent->groundentity, 
         ent->groundentity_linkcount,
         ent->teamchain, 
         ent->teammaster, 
         ent->mynoise, 
         ent->mynoise2, 
         ent->noise_index, 
         ent->noise_index2, 
         ent->volume, 
         ent->attenuation,
         ent->wait, 
         ent->delay, 
         ent->random, 
         ent->teleport_time, 
         ent->watertype,
         ent->waterlevel, 
         ent->move_origin[0], ent->move_origin[1], ent->move_origin[2],
         ent->move_angles[0], ent->move_angles[1], ent->move_angles[2],
         ent->light_level, 
         ent->style, 
         ent->item, 
         ent->moveinfo, 
         ent->monsterinfo);
     Com_sprintf(publicMonsterInstance, sizeof(publicMonsterInstance), 
                 tmpRepresentation, "");
     Com_sprintf(privateMonsterInstance, sizeof(privateMonsterInstance),
           tmpRepresentation, "self");
     /*
      Com_sprintf(publicMonsterInstance, sizeof(publicMonsterInstance), 
            "( of QEdict (pointer %llu) (health %d) (gib-health %d) (max-health %d))",
            ent, ent->health, ent->gib_health, ent->health);
      Com_sprintf(privateMonsterInstance, sizeof(privateMonsterInstance), 
            "(self of QEdict (pointer %llu) (health %d) (gib-health %d) (max-health %d))",
            ent, ent->health, ent->gib_health, ent->health);
            */
      printf("%s\n",privateMonsterInstance);
      printf("%s\n",publicMonsterInstance);
      EnvMakeInstance(ent->privateEnv, privateMonsterInstance);
      EnvMakeInstance(ent->publicEnv, publicMonsterInstance);
   } else {
      /* don't do anything right now as the edict isn't being used for a 
       * monster 
       */
   }
}
#undef IfNullEmpty

void TryDeleteEnvironment(edict_t *ent) {
   if(ent->privateEnv) {
      /* If I had more knowledge of if this edict was going to be reused for
       * something that was a monster then I could act appropriately. 
       *
       * Unfortunately, at this point I have to take a safe approach until I
       * can figure out a safe way to determine if the edict is being reused or
       * not.
       *
       * Because if it is being reused for a monster then I can either call
       * EnvReset or just delete the self pointer. 
       */
      char fact[1024];
      Com_sprintf(fact, sizeof(fact), 
            "({ env: quake2 action: delete type: object env-ptr: %llu self-ptr: %llu duration: -1 })",
            ent->privateEnv, ent);
      DestroyEnvironment(ent->privateEnv);
      EnvAssertString(ent->publicEnv, fact);
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
