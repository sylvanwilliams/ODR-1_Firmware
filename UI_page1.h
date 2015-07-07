/*******************************************************************************
* Header file for UI_page1.c
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*******************************************************************************/

#ifndef __UI_PAGE1_H
#define __UI_PAGE1_H

#define	int8 signed char                // AKA int8_t
#define	uint8 unsigned char             // AKA uint8_t
#define	int16 signed int                // AKA int16_t
#define	uint16 unsigned int             // AKA uint16_t
#define	int32 signed long int           // AKA int32_t
#define	uint32 unsigned long int        // AKA uint32_t
#define	int64 signed long long int      // AKA int64_t
#define	uint64 unsigned long long int   // AKA uint64_t


//Defines to make code easier to read	



// Functions
void Refresh_page1();        // Refresh page 0
void Page1_pointer1_update();
void Refresh_Page1_Item(int16 t);
void Update_Page1_Item();
void Save_Page1_Item();

#endif