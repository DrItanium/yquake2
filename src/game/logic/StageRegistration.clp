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
; Rules that define the stages to be executed 
;--------------------------------------------------------------------
(defrule define-stages
 "Defines the stage control fact used to provide some semblance of ordering"
 (declare (salience 10000))
 ;this fact is asserted by quake2 each time G_RunFrame is called
 ?fct <- (define-stages)
 =>
 (retract ?fct)
 (assert (stage Init 
			       Actions 
					 Advance ; occurs before stale removal occurs
           Advance-Normalization
					 Stale)))

(defrule advance-stage
 "Move to the next stage"
 (declare (salience -10000))
 ?fct <- (stage ?curr $?rest)
 =>
 (retract ?fct)
 (if (> (length$ ?rest) 0) then
  (assert (stage $?rest))))

