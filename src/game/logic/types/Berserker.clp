;
;Copyright (C) 2012 Joshua Scoggins
;
;This program is free software; you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation; either version 2 of the License, or (at
;your option) any later version.
;
;This program is distributed in the hope that it will be useful, but
;WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
;
;See the GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License
;along with this program; if not, write to the Free Software
;Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
;02111-1307, USA.
;--------------------------------------------------------------------
; Berserker moster - CLIPS subtype of this class 
;--------------------------------------------------------------------

(defclass MonsterBerserker (is-a Monster))
 
(defmessage-handler MonsterBerserker sight (?other) 
 ;gi.sound(self, CHAN_VOICE, sound_sight, 1, ATTN_NORMAL, 0);
 )
(defmessage-handler MonsterBerserker search ()
 ;gi.sound(self, CHAN_VOICE, sound_search, 1, ATTN_NORMAL, 0);
 )

(defmessage-handler MonsterBerserker fidget ()
 ;TODO: translate code from berserk_fidget in berserker.c
)

(defmessage-handler MonsterBerserker stand ()
 ;TODO: translate code from berserk_stand in berserker.c
)

(defmessage-handler MonsterBerserker walk ()
 ;TODO: translate code from berserk_stand
)

