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
; Automated stale fact remover. 
;--------------------------------------------------------------------
(defrule retract-stale-facts 
			"Retracts facts that have a duration of 0"
			(stage Stale $?)
			?fct <- ({ env: quake2 $? duration: 0 })
			=>
			(retract ?fct))
;--------------------------------------------------------------------
(defrule advance-facts
			"Decreases the duration of a given fact if it is possible to mark it 
			as stale"
			(stage Advance $?)
			?fct <- ({ env: quake2 
						  $?contents 
						  duration: ?d&:(and (numberp ?d) (> ?d 0)) 
						  }) ; facts marked as continuous or -1 are skipped 
			=>
			(retract ?fct)
			; this will cause the agenda to recompute this fact. This allows the
			; removal of stale facts to occur on the same pass as the decrementing
			; to zero.
			(assert ({ env: quake2 $?contents duration: (- ?d 1) })))
;--------------------------------------------------------------------

