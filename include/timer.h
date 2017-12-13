/*
 * This file is part of the firmware for yogurt maker project
 * (https://github.com/mister-grumbler/yogurt-maker).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TIMER_H
#define TIMER_H

#ifndef bool
#define bool    _Bool
#define true    1
#define false   0
#endif

void initTimer();
void startFTimer();
void stopFTimer();
void resetUptime();
bool isFTimer();
unsigned long getUptime();
unsigned int getUptimeTicks();
unsigned char getUptimeSeconds();
unsigned char getUptimeMinutes();
unsigned char getUptimeHours();
unsigned char getUptimeDays();
void uptimeToString (unsigned char*, const unsigned char*);
void TIM4_UPD_handler() __interrupt (23);

#endif
