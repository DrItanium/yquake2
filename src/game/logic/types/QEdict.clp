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
; Edict class
;--------------------------------------------------------------------
(defclass QEdict (is-a QEntity)
  (slot move-type)
  (slot flags)
  (slot model (type STRING))
  (slot free-time (type FLOAT))
  (slot spawn-flags)
  (slot time-stamp (type FLOAT))
  (slot angle (type FLOAT))
  (slot target)
  (slot target-name)
  (slot kill-target)
  (slot team)
  (slot path-target)
  (slot death-target)
  (slot combat-target)
  (slot target-entity)
  (slot speed (type FLOAT))
  (slot acceleration (type FLOAT))
  (slot deceleration (type FLOAT))
  (slot move-direction)
  (slot position1)
  (slot position2)
  (slot velocity)
  (slot avelocity)
  (slot mass)
  (slot air-finished (type FLOAT))
  (slot gravity)
  (slot goal-entity)
  (slot move-target)
  (slot yaw-speed)
  (slot ideal-yaw)
  (slot next-think)
  (slot function-pre-think)
  (slot function-think)
  (slot function-blocked)
  (slot function-touched)
  (slot function-use)
  (slot function-pain)
  (slot function-die)
  (slot touch-debounce-time (type FLOAT))
  (slot pain-debounce-time (type FLOAT))
  (slot damage-debounce-time (type FLOAT))
  (slot fly-sound-debounce-time (type FLOAT))
  (slot last-move-time(type FLOAT))
  (slot health)
  (slot max-health)
  (slot gib-health)
  (slot is-dead (type SYMBOL) (allowed-symbols FALSE TRUE))
  (slot show-hostile (type SYMBOL) (allowed-symbols FALSE TRUE))
  (slot power-armor-time (type FLOAT))
  (slot Map)
  (slot view-height)
  (slot take-damage)
  (slot damage)
  (slot damage-radius)
  (slot sounds)
  (slot count)
  (slot chain)
  (slot enemy)
  (slot old-enemy)
  (slot activator)
  (slot ground-entity)
  (slot ground-entity-link-count)
  (slot team-chain)
  (slot team-master)
  (slot my-noise)
  (slot my-noise2)
  (slot noise-index)
  (slot noise-index2)
  (slot volume)
  (slot attenuation)
  (slot wait)
  (slot delay)
  (slot random)
  (slot teleport-time)
  (slot water-type)
  (slot water-level)
  (slot light-level)
  (slot style)
  (slot item)
  (slot move-info)
  (slot monster-info))
