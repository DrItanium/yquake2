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
; Some test rules to verify that the code is working properly 
;--------------------------------------------------------------------
(defrule entity-mad-at-announce
 ?fct <- ({ env: quake2 
            action: anger 
            target: ?p 
            enemy: ?e
            target-classname: ?tc
            enemy-classname: ?ec })
 =>
 (retract ?fct)
 (printout quake "Entity: " ?p "(" ?tc ") is mad at " ?e "(" ?ec ")" crlf))

(defrule monster-fired-someting
 ?fct <- ({ env: quake2
            action: fire
            type: ?type
            from: ?ent
            class: ?class })
 =>
 (retract ?fct)
 (printout quake "Entity " ?ent "(" ?class ") fired a " ?type crlf))
