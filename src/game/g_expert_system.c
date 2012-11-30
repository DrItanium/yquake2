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

void
Cmd_Ex_Eval_f(edict_t *ent) {
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
   if(Eval(name, &obj) == 0) {
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
void
Cmd_Ex_Build_f(edict_t *ent) {
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
   if(Build(name) == 0) {
      gi.dprintf("ERROR! Building %s failed\n", name);
   } 
}

void
Cmd_Ex_Run_f(edict_t *ent) {
   char *name;
   int result;
   name = gi.args();
   if(strlen(name) == 0) {
      Run(-1L);
   } else {
      result = atoi(name); 
      if(result <= 0) {
         Run(-1L);
      } else {
         Run(result);
      }
   }
}

void
Cmd_Ex_Batch_f(edict_t *ent) {
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
   } else if(BatchStar(name) == 0) {
      gi.dprintf("ERROR! Couldn't batch %s \n", name);
   } 
}
