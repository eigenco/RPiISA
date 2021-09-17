#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <wiringPi.h>
#include <unistd.h>

#define FW 100
#define IOW 0
#define IOR 1
#define MEMW 2
#define MEMR 3
#define AEN 22
#define A11 23
#define A10 24
#define LA 25
#define RESET 27

void setaddr(unsigned int addr) {
  digitalWrite(0x0C, addr & 1); addr >>= 1;
  digitalWrite(0x0D, addr & 1); addr >>= 1;
  digitalWrite(0x0E, addr & 1); addr >>= 1;
  digitalWrite(0x0F, addr & 1); addr >>= 1;
  digitalWrite(0x10, addr & 1); addr >>= 1;
  digitalWrite(0x11, addr & 1); addr >>= 1;
  digitalWrite(0x12, addr & 1); addr >>= 1;
  digitalWrite(0x13, addr & 1); addr >>= 1;
  digitalWrite(0x14, addr & 1); addr >>= 1;
  digitalWrite(0x15, addr & 1); addr >>= 1;
  digitalWrite(A10, addr & 1); addr >>= 1;
  digitalWrite(A11, addr & 1);
}

void setdata(unsigned char data) {
  digitalWrite(0x04, data & 1); data >>= 1;
  digitalWrite(0x05, data & 1); data >>= 1;
  digitalWrite(0x06, data & 1); data >>= 1;
  digitalWrite(0x07, data & 1); data >>= 1;
  digitalWrite(0x08, data & 1); data >>= 1;
  digitalWrite(0x09, data & 1); data >>= 1;
  digitalWrite(0x0A, data & 1); data >>= 1;
  digitalWrite(0x0B, data & 1);
}

unsigned char getdata() {
  unsigned char data;

  data  = digitalRead(0x0B); data <<= 1;
  data |= digitalRead(0x0A); data <<= 1;
  data |= digitalRead(0x09); data <<= 1;
  data |= digitalRead(0x08); data <<= 1;
  data |= digitalRead(0x07); data <<= 1;
  data |= digitalRead(0x06); data <<= 1;
  data |= digitalRead(0x05); data <<= 1;
  data |= digitalRead(0x04);

  return data;
}

void setinput() {
  unsigned char i;
  for(i=0; i<8; i++)
    pinMode(i+4, INPUT);
}

void setoutput() {
  unsigned char i;
  for(i=0; i<8; i++)
    pinMode(i+4, OUTPUT);
}

unsigned char inb(unsigned short addr) {
  unsigned char data;
  int i;

  setinput();
  setaddr(addr);
  digitalWrite(IOR, 0);
  for(i=0; i<FW; i++);
  data = getdata();
  digitalWrite(IOR, 1);

  return data;
}

void outb(unsigned short addr, unsigned char data) {
  int i;
  setoutput();
  setaddr(addr);
  setdata(data);
  digitalWrite(IOW, 0);
  for(i=0; i<FW; i++);
  digitalWrite(IOW, 1);
}

void memw(unsigned int addr, unsigned char data) {
  int i;
  setoutput();
  setaddr((addr >> 12) & 0xFF);
  digitalWrite(LA, 1);
  digitalWrite(LA, 0);
  setaddr(addr & 0xFFF);
  setdata(data);
  digitalWrite(MEMW, 0);
  for(i=0; i<FW; i++);
  digitalWrite(MEMW, 1);
}

unsigned char memr(unsigned int addr) {
  unsigned char data;
  int i;

  setinput();
  setaddr((addr >> 12) & 0xFF);
  digitalWrite(LA, 1);
  digitalWrite(LA, 0);
  setaddr(addr & 0xFFF);
  digitalWrite(MEMR, 0);
  for(i=0; i<FW; i++);
  data = getdata();
  digitalWrite(MEMR, 1);

  return data;
}

int main() {
  unsigned char j, m = 1;
  int i, k;
  int x, y, z;

  srand(time(0));
  wiringPiSetupGpio();

  pinMode(RESET, OUTPUT);
  pinMode(AEN, OUTPUT);
  pinMode(IOW, OUTPUT);
  pinMode(IOR, OUTPUT);
  pinMode(LA, OUTPUT);
  pinMode(MEMW, OUTPUT);
  pinMode(MEMR, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);
  digitalWrite(IOW, 1);
  digitalWrite(IOR, 1);
  digitalWrite(MEMW, 1);
  digitalWrite(MEMR, 1);
  digitalWrite(AEN, 0);

  // CONFIGURE ADDRESS PINS
  for(i=0; i<0x0A; i++) pinMode(i+12, OUTPUT);
  for(i=0; i<0x0A; i++) pullUpDnControl(i+12, PUD_DOWN);

  // CONFIGURE DATA PINS
  for(i=0; i<0x08; i++) pinMode(i+4, INPUT);
  for(i=0; i<0x08; i++) pullUpDnControl(i+4, PUD_DOWN);

  digitalWrite(RESET, 1); usleep(100);
  digitalWrite(RESET, 0); usleep(100);

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
  usleep(100000);

  // ATC registers at port 3c0
  for(j=0; j<0x10; j++) {
    m = inb(0x3da);
    outb(0x3c0, j);
    outb(0x3c0, j);
  }
  outb(0x3c0, 0x30); outb(0x3c0, 0x41);
  outb(0x3c0, 0x31); outb(0x3c0, 0x00);
  outb(0x3c0, 0x32); outb(0x3c0, 0x0f);
  outb(0x3c0, 0x33); outb(0x3c0, 0x00);
  outb(0x3c0, 0x34); outb(0x3c0, 0x00);
  usleep(100000);

  // SEQ registers at port 3c4
  outb(0x3c4, 0x00); outb(0x3c5, 0x03);
  outb(0x3c4, 0x01); outb(0x3c5, 0x01);
  outb(0x3c4, 0x02); outb(0x3c5, 0x0f);
  outb(0x3c4, 0x03); outb(0x3c5, 0x00);
  outb(0x3c4, 0x04); outb(0x3c5, 0x0e);
  usleep(100000);

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
  usleep(100000);

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
  usleep(100000);

  /* Enable video */
  outb(0x3c0, 0x20);
  inb(0x3da);
  usleep(100000);

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

  printf("RAM test:\n");
  for(i=0; i<256; i++) memw(i, i);
  i=0;
  for(y=0; y<16; y++) {
    for(x=0; x<16; x++)
      printf("%.2x ", memr(i++));
    printf("\n");
  }
  printf("\n");

  for(y=0; y<200; y++)
    for(x=0; x<320; x++)
      memw(0xA0000 + x + y*320, (x & 63) + 64*((y >> 3)& 3));

  for(y=0; y<64; y++)
  for(x=32000; x<64000; x++)
    memw(0xA0000 + x, rand() & 255);

  printf("VMEM test:\n");
  i=0;
  for(y=0; y<16; y++) {
    for(x=0; x<16; x++)
      printf("%.2x ", memr(0xA0000 + i++));
    printf("\n");
  }
  printf("\n");

  return 0;
}
