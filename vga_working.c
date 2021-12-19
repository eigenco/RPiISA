// gcc -O0 playvga.c -l wiringPi -o playvga
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>

#define LA 23
#define IOW 25
#define IOR 11
#define MEMR 9
#define MEMW 10
#define RESET 22

#define D0 27
#define D1 18
#define D2 17
#define D3 15
#define D4 14
#define D5 4
#define D6 3
#define D7 2

#define A0 21
#define A1 20
#define A2 26
#define A3 16
#define A4 19
#define A5 13
#define A6 12
#define A7 6
#define A8 5
#define A9 1
#define A10 0
#define A11 7

#define ATA0 0x300
#define ATA1 0x302
#define ATA2 0x304
#define ATA3 0x306
#define ATA4 0x308
#define ATA5 0x30A
#define ATA6 0x30C
#define ATA7 0x30E

void setaddr(unsigned short addr) {
  digitalWrite(A0, addr & 1); addr >>= 1;
  digitalWrite(A1, addr & 1); addr >>= 1;
  digitalWrite(A2, addr & 1); addr >>= 1;
  digitalWrite(A3, addr & 1); addr >>= 1;
  digitalWrite(A4, addr & 1); addr >>= 1;
  digitalWrite(A5, addr & 1); addr >>= 1;
  digitalWrite(A6, addr & 1); addr >>= 1;
  digitalWrite(A7, addr & 1); addr >>= 1;
  digitalWrite(A8, addr & 1); addr >>= 1;
  digitalWrite(A9, addr & 1); addr >>= 1;
  digitalWrite(A10, addr & 1); addr >>= 1;
  digitalWrite(A11, addr & 1);
}

void setdata(unsigned char data) {
  digitalWrite(D0, data & 1); data >>= 1;
  digitalWrite(D1, data & 1); data >>= 1;
  digitalWrite(D2, data & 1); data >>= 1;
  digitalWrite(D3, data & 1); data >>= 1;
  digitalWrite(D4, data & 1); data >>= 1;
  digitalWrite(D5, data & 1); data >>= 1;
  digitalWrite(D6, data & 1); data >>= 1;
  digitalWrite(D7, data & 1);
}

unsigned char getdata() {
  unsigned char data;

  data  = digitalRead(D7); data <<= 1;
  data |= digitalRead(D6); data <<= 1;
  data |= digitalRead(D5); data <<= 1;
  data |= digitalRead(D4); data <<= 1;
  data |= digitalRead(D3); data <<= 1;
  data |= digitalRead(D2); data <<= 1;
  data |= digitalRead(D1); data <<= 1;
  data |= digitalRead(D0);

  return data;
}

void setinput() {
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
}

void setoutput() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
}

unsigned char inb(unsigned short addr) {
  unsigned char data;

  setinput();
  setaddr(addr);
  digitalWrite(IOR, 0); usleep(1);
  data = getdata();
  digitalWrite(IOR, 1); usleep(1);

  return data;
}

void outb(unsigned short addr, unsigned char data) {
  setoutput();
  setaddr(addr);
  setdata(data);
  digitalWrite(IOW, 0); usleep(1);
  digitalWrite(IOW, 1); usleep(1);
}

void memw(unsigned int addr, unsigned char data) {
  int i;

  setoutput();
  setaddr((addr >> 12) & 0xFF);
  digitalWrite(LA, 1);
  digitalWrite(LA, 0);
  setaddr(addr & 0xFFF);
  setdata(data);
  digitalWrite(MEMW, 0); for(i=0; i<256; i++);
  digitalWrite(MEMW, 1); for(i=0; i<256; i++);
}

unsigned char memr(unsigned int addr) {
  unsigned char data, l;

  setinput();
  setaddr((addr >> 12) & 0xFF);
  digitalWrite(LA, 1);
  digitalWrite(LA, 0);
  setaddr(addr & 0xFFF);
  digitalWrite(MEMR, 0); usleep(1);
  data = getdata();
  digitalWrite(MEMR, 1); usleep(1);

  return data;
}


void wr_adlib(unsigned char reg, unsigned char value) {
  outb(0x388, reg);
  outb(0x389, value);
}

void init_vga(void) {
  unsigned int i;

  // ET4000: set key
  outb(0x3bf, 3);
  outb(0x3d8, 0xa0);
  usleep(100000);

  // ET4000: set clocks
  outb(0x3c4, 0x07);
  outb(0x3c5, 0xfc);
  usleep(100000);

  // MISC register at port 3c2
  outb(0x3c2, 0x63);

  // ATC registers at port 3c0
  for(i=0; i<0x10; i++) {
    inb(0x3da);
    outb(0x3c0, i);
    outb(0x3c0, i);
  }
  outb(0x3c0, 0x30); outb(0x3c0, 0x41);
  outb(0x3c0, 0x31); outb(0x3c0, 0x00);
  outb(0x3c0, 0x32); outb(0x3c0, 0x0f);
  outb(0x3c0, 0x33); outb(0x3c0, 0x00);
  outb(0x3c0, 0x34); outb(0x3c0, 0x00);

  // SEQ registers at port 3c4
  outb(0x3c4, 0x00); outb(0x3c5, 0x03);
  outb(0x3c4, 0x01); outb(0x3c5, 0x01);
  outb(0x3c4, 0x02); outb(0x3c5, 0x0f);
  outb(0x3c4, 0x03); outb(0x3c5, 0x00);
  outb(0x3c4, 0x04); outb(0x3c5, 0x0e);

  // GDC registers at port 3ce
  outb(0x3ce, 0x00); outb(0x3cf, 0x00);
  outb(0x3ce, 0x01); outb(0x3cf, 0x00);
  outb(0x3ce, 0x02); outb(0x3cf, 0x00);
  outb(0x3ce, 0x03); outb(0x3cf, 0x00);
  outb(0x3ce, 0x04); outb(0x3cf, 0x00);
  outb(0x3ce, 0x05); outb(0x3cf, 0x40);
  outb(0x3ce, 0x06); outb(0x3cf, 0x05);
  outb(0x3ce, 0x07); outb(0x3cf, 0x0f);
  outb(0x3ce, 0x08); outb(0x3cf, 0xff);

  // CRTC registers at port 3d4
  outb(0x3d4, 0x11); outb(0x3d5, 0x00);
  outb(0x3d4, 0x00); outb(0x3d5, 0x5f);
  outb(0x3d4, 0x01); outb(0x3d5, 0x4f);
  outb(0x3d4, 0x02); outb(0x3d5, 0x50);
  outb(0x3d4, 0x03); outb(0x3d5, 0x82);
  outb(0x3d4, 0x04); outb(0x3d5, 0x54);
  outb(0x3d4, 0x05); outb(0x3d5, 0x80);
  outb(0x3d4, 0x06); outb(0x3d5, 0xbf);
  outb(0x3d4, 0x07); outb(0x3d5, 0x1f);
  outb(0x3d4, 0x08); outb(0x3d5, 0x00);
  outb(0x3d4, 0x09); outb(0x3d5, 0x41);
  outb(0x3d4, 0x0a); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0b); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0c); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0d); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0e); outb(0x3d5, 0x00);
  outb(0x3d4, 0x0f); outb(0x3d5, 0x00);
  outb(0x3d4, 0x10); outb(0x3d5, 0x9c);
  outb(0x3d4, 0x11); outb(0x3d5, 0x8e);
  outb(0x3d4, 0x12); outb(0x3d5, 0x8f);
  outb(0x3d4, 0x13); outb(0x3d5, 0x28);
  outb(0x3d4, 0x14); outb(0x3d5, 0x40);
  outb(0x3d4, 0x15); outb(0x3d5, 0x96);
  outb(0x3d4, 0x16); outb(0x3d5, 0xb9);
  outb(0x3d4, 0x17); outb(0x3d5, 0xa3);
  outb(0x3d4, 0x18); outb(0x3d5, 0xff);

  /* Enable video */
  outb(0x3c0, 0x20);
  inb(0x3da);
  usleep(100000);
}

int main() {
  unsigned char buffer[57306];
  int song_register, song_data, song_offset = 0, song_wait;
  FILE *f;
  int i, j, k;
  int x, y, z;

  f = fopen("le.adl", "rb");
  fread(buffer, 1, 57306, f);
  fclose(f);

  wiringPiSetupGpio();

  pinMode(LA, OUTPUT);
  pinMode(IOW, OUTPUT);
  pinMode(IOR, OUTPUT);
  pinMode(MEMW, OUTPUT);
  pinMode(MEMR, OUTPUT);
  pinMode(RESET, OUTPUT);
  digitalWrite(IOW, 1);
  digitalWrite(IOR, 1);
  digitalWrite(MEMW, 1);
  digitalWrite(MEMR, 1);

  // CONFIGURE ADDRESS PINS
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);

  // CONFIGURE DATA PINS
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);

  // TEST MEM
  for(i=0; i<256; i++) memw(0xA0000+i, i);

  // RESET
  digitalWrite(RESET, 1); usleep(100000);
  digitalWrite(RESET, 0); usleep(500000);

  init_vga();

  /* set palette */
  outb(0x3c6, 0xff); // PEL mask
  outb(0x3c8, 0);    // Palette register
  for(i=0; i<64; i++) {
    outb(0x3c9, i & 63);
    outb(0x3c9, 0);
    outb(0x3c9, 0);
  }
  for(i=0; i<64; i++) {
    outb(0x3c9, 0);
    outb(0x3c9, i & 63);
    outb(0x3c9, 0);
  }
  for(i=0; i<64; i++) {
    outb(0x3c9, 0);
    outb(0x3c9, 0);
    outb(0x3c9, i & 63);
  }
  for(i=0; i<64; i++) {
    outb(0x3c9, i & 63);
    outb(0x3c9, i & 63);
    outb(0x3c9, i & 63);
  }

  printf("VMEM test:\n");
  i=0;
  srand(time(0));
  for(y=0; y<16; y++)
    for(x=0; x<16; x++)
      memw(0xA0000 + i++, rand()%256);
  i=0;
  for(y=0; y<16; y++) {
    for(x=0; x<16; x++)
      printf("%.2x ", memr(0xA0000 + i++));
    printf("\n");
  }
  printf("\n");

  for(y=0; y<150; y++)
    for(x=0; x<320; x++)
      memw(0xA0000 + x + y*320, (x & 63) + 64*((y >> 3)& 3));

  for(;;)
  for(y=150; y<200; y++)
    for(x=0; x<320; x++)
      memw(0xA0000 + x + y*320, rand()%256);

  return 0;
}
