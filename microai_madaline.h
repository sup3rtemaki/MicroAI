#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define STATE_BYTES 7
#define MULT 0x13B /* for STATE_BYTES==6 only */
#define MULT_LO (MULT & 255)
#define MULT_HI (MULT & 256)

void treinarRede(int amostras, int entradas, int numclasses, int targets, 
									int entrada[amostras][entradas], int target[numclasses][targets], float w[entradas][numclasses]);

void testarRede(int entradas, int numclasses, float w[entradas][numclasses],
									 int entrada_teste[entradas], float y[numclasses]);

uint8_t rand8(void);