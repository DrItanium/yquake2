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
; Rewrite of g_combat functions as rules
;--------------------------------------------------------------------
(defrule react-to-damage-no-exist
 (declare (salience 10))
 (stage combat-update $?)
 ?fct <- ({ env: quake2 
            action: react-to-damage
            target: ?target
            attacker: ?attacker 
            duration: ? })
 (or (not (exists (object (is-a QEdict) (pointer ?target))))
     (not (exists (object (is-a QEdict) (pointer ?attacker)))))
 =>
 (retract ?fct))
;--------------------------------------------------------------------
(defrule react-to-damage-already-dead
 (declare (salience 10))
 ?fct <- ({ env: quake2 
            action: react-to-damage
            target: ?target
            attacker: ?attacker 
            duration: ? })
 (object (is-a QEdict) (pointer ?target) (health ?h&:(<= ?h 0)))
 =>
 ;already dead
 (retract ?fct))

(defrule react-to-damage-attacker-not-client-not-monster
 (declare (salience 10))
 ?fct <- ({ env: quake2 
            action: react-to-damage
            target: ?target
            attacker: ?attacker 
            duration: ? })
 (object (is-a QEdict) (pointer ?target) (health ?h&:(> ?h 0)))
 (object (is-a QEdict) (pointer ?attacker) 
         (is-client FALSE) (
 
