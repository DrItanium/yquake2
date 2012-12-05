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
; CLIPS representation of the C representation of a CLIPS environment
;--------------------------------------------------------------------
(defclass Environment (is-a USER)
 (slot id (type SYMBOL))
 (slot target (type INTEGER))
 (slot pointer (type INTEGER)))

(defmessage-handler Environment init after ()
 (bind ?self:id (instance-name-to-symbol (instance-name ?self))))

(defmessage-handler Environment run ()
 (quake-env-run ?self:pointer))

(defmessage-handler Environment run-for (?count)
 (if (and (>= ?count 0) (integerp ?count)) then
 (quake-env-run ?self:pointer ?count)
 else
 (printout quake "ERROR: invalid number provided." crlf)))
 
(defmessage-handler Environment eval (?string)
 (if (stringp ?string) then
  (quake-env-eval ?self:pointer ?string)
  else
  (printout quake "ERROR: string expected" crlf)))
 
(defmessage-handler Environment build (?string)
 (if (stringp ?string) then
  (quake-env-build ?self:pointer ?string)
  else
  (printout quake "ERROR: string expected" crlf)))

(defmessage-handler Environment facts ()
 (quake-env-eval ?self:pointer "(facts)"))

(defmessage-handler Environment instances ()
 (quake-env-eval ?self:pointer "(instances)"))

(defmessage-handler Environment rules ()
 (quake-env-eval ?self:pointer "(rules)"))

(defmessage-handler Environment assert ($?elements)
 (quake-env-eval ?self:pointer 
  (format nil "(assert (%s))" (implode$ $?elements))))
