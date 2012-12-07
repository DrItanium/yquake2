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
; Monster class - A wrapper over monsterinfo
;--------------------------------------------------------------------
(defclass Monster (is-a QEdict)
  (slot current-move)
  (slot ai-flags)
  (slot next-frame)
  (slot scale (type FLOAT))
  (slot function-stand (type INTEGER))
  (slot function-idle (type INTEGER))
  (slot function-search (type INTEGER))
  (slot function-walk (type INTEGER))
  (slot function-run (type INTEGER))
  (slot function-dodge (type INTEGER))
  (slot function-attack (type INTEGER))
  (slot function-melee (type INTEGER))
  (slot function-sight (type INTEGER))
  (slot function-check-attack (type INTEGER))
  (slot pause-time)
  (slot attack-finished)
  (multislot saved-goal)
  (slot search-time)
  (slot trail-time)
  (multislot last-sighting)
  (slot attack-state)
  (slot lefty)
  (slot idle-time (type FLOAT))
  (slot link-count)
  (slot power-armor-type)
  (slot power-armor-power))


(defmessage-handler Monster stand primary () )
(defmessage-handler Monster idle primary () )
(defmessage-handler Monster search primary () )
(defmessage-handler Monster walk primary () )
(defmessage-handler Monster run primary () )
(defmessage-handler Monster dodge primary () )
(defmessage-handler Monster attack primary () )
(defmessage-handler Monster melee primary () )
(defmessage-handler Monster sight primary (?other) )
(defmessage-handler Monster check-attack primary () )
(defmessage-handler Monster fidget primary () )
(defmessage-handler Monster die primary (?inflictor ?attacker ?damage ?point) )
(defmessage-handler Monster spawn primary () )
