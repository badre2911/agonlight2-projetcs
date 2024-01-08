
#include <stdio.h>
#include <mos_api.h>
#include <stdint.h>
#include <stdbool.h>
 

bool isPresent(uint8_t i2c_address) {
	return (!mos_i2c_write(i2c_address, 0, 0));
}

void scanBus(void) {
	int n;

	printf("\r\nScanning I2C bus:\r\n");
	for(n = 0; n < 128; n++) {
		if(isPresent(n)) {
			printf("\r\nAddress 0x%02X\r\n", n);
		}
	}
}

int main()
{			
	mos_i2c_open(3);
	scanBus();
	mos_i2c_close();
	
	return 0;
}




