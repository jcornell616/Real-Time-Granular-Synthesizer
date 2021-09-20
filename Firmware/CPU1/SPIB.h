#ifndef SPIB_H_
#define SPIB_H_

void SPIB_Init(Uint16 baudRate);

Uint16 SPItransmit(Uint16 data);

Uint16 SPIreceive(void);

#endif
