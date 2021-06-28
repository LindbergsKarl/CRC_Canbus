#include <stdio.h>
#include <stdint.h>

#define VAL_GODKAND 1
#define POLYNOMIAL 0xC599 // CAN protocol uses CRC-15 with this polynomial
#define SIZE_BIT 8
#define MSB 0x8000
#define LSB 0x0001
#define LSBPOS (sizeof(message)-1)
#define SECOND_PART 0x00ff



uint16_t get_crc15(uint8_t *data, uint8_t size)
{
	uint16_t remainder = 0;
	for(int i=0; i<size; i++){
		for(int x = 0; x<SIZE_BIT; x++ ){
			remainder = remainder << 1;

			if(data[i] & 1 << x){
				remainder = remainder | LSB;
			}
			if(remainder & MSB){
				remainder = remainder ^ POLYNOMIAL;
			}
		}
	}
	return remainder;
}

uint8_t getBitsReversed(uint8_t bits){
	uint8_t reversed=0;
		for(int i=0; i<SIZE_BIT;i++){
			reversed |= ((bits>>i) & 1)<<((SIZE_BIT-1)-i);
		}
	return reversed;
}

int main(void)
{
	uint8_t message[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 0};

	uint16_t crc = get_crc15(message, sizeof(message));
	(void)printf("The CRC of the message is: 0x%X\n", crc); // It shall be 0xB35

#if VAL_GODKAND

	message[LSBPOS-1] = getBitsReversed(crc >> SIZE_BIT);

	message[LSBPOS] = getBitsReversed(crc & SECOND_PART);


	if (0 == get_crc15(message, sizeof(message)))
	{
		printf("The data is OK\n");
	}
	else
	{
		printf("The data is not OK\n");
	}
#endif
}

