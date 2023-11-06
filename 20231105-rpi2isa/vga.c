// gcc -O2 vga.c -l wiringPi -o vga
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
#define READY 24

#define D0 27
#define D1 18
#define D2 17
#define D3 15
#define D4 14
#define D5 4
#define D6 3
#define D7 2

#define A00 21
#define A01 20
#define A02 26
#define A03 16
#define A04 19
#define A05 13
#define A06 12
#define A07 6
#define A08 5
#define A09 1
#define A10 0
#define A11 7

volatile int mode = 0;
volatile int oldaddr = 0;

void setaddr(int addr) {
  digitalWrite(A00, addr & 1); addr >>= 1;
  digitalWrite(A01, addr & 1); addr >>= 1;
  digitalWrite(A02, addr & 1); addr >>= 1;
  digitalWrite(A03, addr & 1); addr >>= 1;
  digitalWrite(A04, addr & 1); addr >>= 1;
  digitalWrite(A05, addr & 1); addr >>= 1;
  digitalWrite(A06, addr & 1); addr >>= 1;
  digitalWrite(A07, addr & 1); addr >>= 1;
  digitalWrite(A08, addr & 1); addr >>= 1;
  digitalWrite(A09, addr & 1); addr >>= 1;
  digitalWrite(A10, addr & 1); addr >>= 1;
  digitalWrite(A11, addr & 1);
}

void setdata(int data) {
  digitalWrite(D0, data & 1); data >>= 1;
  digitalWrite(D1, data & 1); data >>= 1;
  digitalWrite(D2, data & 1); data >>= 1;
  digitalWrite(D3, data & 1); data >>= 1;
  digitalWrite(D4, data & 1); data >>= 1;
  digitalWrite(D5, data & 1); data >>= 1;
  digitalWrite(D6, data & 1); data >>= 1;
  digitalWrite(D7, data & 1);
}

int getdata() {
  int data;

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
  mode = 0;
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
  mode = 1;
}

int inb(int addr) {
  int data;

  if(mode==1) setinput();
  if( ((addr >> 12) & 0xFF) != ((oldaddr >> 12) & 0xFF) ) {
    setaddr((addr >> 12) & 0xFF);
    digitalWrite(LA, 1);
    digitalWrite(LA, 0);
    oldaddr = addr;
  }
  setaddr(addr & 0xFFF);
  digitalWrite(IOR, 0);
  usleep(1);
  data = getdata();
  digitalWrite(IOR, 1);

  return data;
}

void outb(int addr, int data) {
  if(mode==0) setoutput();
  if( ((addr >> 12) & 0xFF) != ((oldaddr >> 12) & 0xFF) ) {
    setaddr((addr >> 12) & 0xFF);
    digitalWrite(LA, 1);
    digitalWrite(LA, 0);
    oldaddr = addr;
  }
  setaddr(addr & 0xFFF);
  setdata(data);
  digitalWrite(IOW, 0);
  usleep(1);
  digitalWrite(IOW, 1);
}

void memw(int addr, int data) {
  if(mode==0) setoutput();
  if( ((addr >> 12) & 0xFF) != ((oldaddr >> 12) & 0xFF) ) {
    setaddr((addr >> 12) & 0xFF);
    digitalWrite(LA, 1);
    digitalWrite(LA, 0);
    oldaddr = addr;
  }
  setaddr(addr & 0xFFF);
  setdata(data);
  digitalWrite(MEMW, 0);
  while(!digitalRead(READY)) { printf("*"); fflush(stdout); }
  digitalWrite(MEMW, 1);
}

int memr(int addr) {
  int data;

  if(mode==1) setinput();
  if( ((addr >> 12) & 0xFF) != ((oldaddr >> 12) & 0xFF)) {
    setaddr((addr >> 12) & 0xFF);
    digitalWrite(LA, 1);
    digitalWrite(LA, 0);
    oldaddr = addr;
  }
  setaddr(addr & 0xFFF);
  digitalWrite(MEMR, 0);
  while(!digitalRead(READY)) { printf("*"); fflush(stdout); }
  data = getdata();
  digitalWrite(MEMR, 1);

  return data;
}

void init_vga(void) {
  for(int i=0; i<256; i++) memw(0xA0000+i, i);

  // RESET BUS
  digitalWrite(RESET, 1); usleep(100000);
  digitalWrite(RESET, 0); usleep(100000);

  // ET4000: set key
  outb(0x3bf, 3);
  outb(0x3d8, 0xa0);
  //usleep(100000);

  // ET4000: set clocks
  outb(0x3c4, 0x07);
  outb(0x3c5, 0xfc);

  // MISC register at port 3c2
  outb(0x3c2, 0x63);

  // ATC registers at port 3c0
  for(int i=0; i<0x10; i++) {
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
  int j = inb(0x3da);

  for(int i=0; i<8000; i++) memw(0xA0000+i, i);

  /* Set palette */
  outb(0x3c6, 0xff); // PEL mask
  for(;;) {
    outb(0x3c8, 0);
    outb(0x3c9, rand() % 64);
    outb(0x3c9, rand() % 64);
    outb(0x3c9, rand() % 64);
    usleep(100000);
  }
/*
  outb(0x3c8, 0);    // Palette register
  for(int i=0; i<64; i++) {
    outb(0x3c9, i & 63);
    outb(0x3c9, 0);
    outb(0x3c9, 0);
  }
  for(int i=0; i<64; i++) {
    outb(0x3c9, 0);
    outb(0x3c9, i & 63);
    outb(0x3c9, 0);
  }
  for(int i=0; i<64; i++) {
    outb(0x3c9, 0);
    outb(0x3c9, 0);
    outb(0x3c9, i & 63);
  }
  for(int i=0; i<64; i++) {
    outb(0x3c9, i & 63);
    outb(0x3c9, i & 63);
    outb(0x3c9, i & 63);
  }
*/
}

int main() {
  int i, j, k, x, y, z;

  wiringPiSetupGpio();

  pinMode(READY, INPUT);
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
  pinMode(A00, OUTPUT);
  pinMode(A01, OUTPUT);
  pinMode(A02, OUTPUT);
  pinMode(A03, OUTPUT);
  pinMode(A04, OUTPUT);
  pinMode(A05, OUTPUT);
  pinMode(A06, OUTPUT);
  pinMode(A07, OUTPUT);
  pinMode(A08, OUTPUT);
  pinMode(A09, OUTPUT);
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

  init_vga();
/*
  for(i=0; i<70; i++) {
    for(y=0; y<200; y++)
      for(x=0; x<320; x++)
        memw(0xA0000 + x + y*320, rand()&255);
  }

  for(;;) {
    outb(0x3c6, 0xff);
    outb(0x3c8, 0);
    for(i=0; i<256; i++) {
      outb(0x3c9, rand()&63);
      outb(0x3c9, rand()&63);
      outb(0x3c9, rand()&63);
    }
    usleep(100000);
  }
*/
  return 0;
}
