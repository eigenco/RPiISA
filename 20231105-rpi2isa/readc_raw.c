#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "isa.h"

int main() {
  int i, j, k, x, y, z;
  unsigned char buffer[32768];
  FILE *f;

  gpio = (unsigned int*)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, open("/dev/gpiomem", O_RDWR|O_SYNC), 0);
  digitalWrite(IOW, 1);
  digitalWrite(IOR, 1);
  digitalWrite(MEMW, 1);
  digitalWrite(MEMR, 1);
  digitalWrite(RESET, 1); updatePins(); usleep(100000);
  digitalWrite(RESET, 0); updatePins(); usleep(500000);

  for(i=0; i<32768; i++)
    buffer[i] = memr(0xC0000+i);

  f = fopen("vgarom.dat", "wb");
  fwrite(buffer, 1, 32768, f);
  fclose(f);

  return 0;
}
