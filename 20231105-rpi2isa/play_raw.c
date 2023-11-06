#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "isa.h"

void wr_adlib(unsigned char reg, unsigned char value) {
  outb(0x388, reg); usleep(3);
  outb(0x389, value), usleep(22);
  // 1 us is in the inb/outb
}

int main() {
  int i, j, k, x, y, z, song_register, song_data, song_wait, song_offset = 0;
  unsigned char buffer[64000];
  FILE *f;

  f = fopen("lemmings.dat", "rb");
  fread(buffer, 1, 57306, f);
  fclose(f);

  gpio = (unsigned int*)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, open("/dev/gpiomem", O_RDWR|O_SYNC), 0);
  setoutput();
  gpio[GPCLR0] = 0x0FFFFFFF;

  digitalWrite(IOW, 1);
  digitalWrite(IOR, 1);
  digitalWrite(MEMW, 1);
  digitalWrite(MEMR, 1);
  digitalWrite(RESET, 1); updatePins(); usleep(100000);
  digitalWrite(RESET, 0); updatePins(); usleep(100000);

  srand(time(0));
  j = rand()&255;
  for(i=0; i<16; i++) {
    memw(0xA0000+i, (i+j) & 255);
    printf("%.2x ", (i+j) & 255);
  }
  printf("\n");
  for(i=0; i<16; i++)
    printf("%.2x ", memr(0xA0000+i));
  printf("\n");
  for(i=0; i<16; i++)
    printf("%.2x ", memr(0xC0000+i));
  printf("\n");

  // PLAY THE LEMMINGS SONG USING ADLIB
  for(;;) {
    song_register = buffer[song_offset];
    song_offset++;
    if(song_register==0) {
      song_wait  = (buffer[song_offset+1] << 8) | buffer[song_offset+0];
      song_offset += 2;
      usleep(1786*song_wait);
    } else {
      song_data = buffer[song_offset]; song_offset++;
      song_wait = buffer[song_offset]; song_offset++;
      wr_adlib(song_register, song_data);
      usleep(1786*song_wait);
    }
    if(song_offset>57300) {
      for(i=0; i<256; i++)
        wr_adlib(i, 0);
      exit(0);
    }
  }

  return 0;
}
