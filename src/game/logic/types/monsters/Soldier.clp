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
; Soldier moster - CLIPS subtype of this class 
;--------------------------------------------------------------------
;base class
(defclass MonsterSoldier (is-a WalkMonster)
 (role abstract)
 (slot sound-idle)
 (slot sound-sight1)
 (slot sound-sight2)
 (slot sound-pain)
 (slot sound-death)
 (slot sound-cock))

(defmessage-handler MonsterSolider init after () 
 (bind ?self:mass 100)
)
 
(defmessage-handler MonsterSoldier sight primary (?other) 
 )
(defmessage-handler MonsterSoldier search primary ()
 )

(defmessage-handler MonsterSoldier fidget primary ()
)

(defmessage-handler MonsterSoldier stand primary ()
)

(defmessage-handler MonsterSoldier walk primary ()
)

(defmessage-handler MonsterSoldier cock primary ())
