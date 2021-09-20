/*
 * SRAMDriver.h
 *
 *  Created on: Feb 10, 2021
 *      Author: jacks
 */

#ifndef SRAMDRIVER_H_
#define SRAMDRIVER_H_

//Initialize system to read SRAM
void SRAM_Init(Uint16 baudRate);

//Read from SRAM
Uint16 SRAM_Read(Uint32 addr);
char SRAM_Read_Char(Uint32 addr);

//Write to SRAM
Uint16 SRAM_Write(Uint32 addr, Uint16 data);
Uint16 SRAM_Write_Char(Uint32 addr, char data);

#endif /* SRAMDRIVER_H_ */
