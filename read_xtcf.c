#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#define IOW 0
#define IOR 1
#define AEN 22

#define ATA0 0x300
#define ATA1 0x302
#define ATA2 0x304
#define ATA3 0x306
#define ATA4 0x308
#define ATA5 0x30A
#define ATA6 0x30C
#define ATA7 0x30E

void setaddr(unsigned short addr) {
  digitalWrite(0x0C, addr & 1); addr >>= 1;
  digitalWrite(0x0D, addr & 1); addr >>= 1;
  digitalWrite(0x0E, addr & 1); addr >>= 1;
  digitalWrite(0x0F, addr & 1); addr >>= 1;
  digitalWrite(0x10, addr & 1); addr >>= 1;
  digitalWrite(0x11, addr & 1); addr >>= 1;
  digitalWrite(0x12, addr & 1); addr >>= 1;
  digitalWrite(0x13, addr & 1); addr >>= 1;
  digitalWrite(0x14, addr & 1); addr >>= 1;
  digitalWrite(0x15, addr & 1);
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
  for(i=0; i<100; i++)
  data = getdata();
  digitalWrite(IOR, 1);
  for(i=0; i<100; i++)

  return data;
}

void outb(unsigned short addr, unsigned char data) {
  int i;
  setoutput();
  setaddr(addr);
  setdata(data);
  digitalWrite(IOW, 0);
  for(i=0; i<100; i++);
  digitalWrite(IOW, 1);
  for(i=0; i<100; i++);
}

int main() {
  int i;

  wiringPiSetupGpio();

  pinMode(IOW, OUTPUT);
  pinMode(IOR, OUTPUT);
  pinMode(AEN, OUTPUT);
  digitalWrite(IOW, 1);
  digitalWrite(IOR, 1);
  digitalWrite(AEN, 0);

  // CONFIGURE ADDRESS PINS
  for(i=0; i<0x0A; i++) pinMode(i+12, OUTPUT);
  for(i=0; i<0x0A; i++) pullUpDnControl(i+12, PUD_DOWN);

  // CONFIGURE DATA PINS
  for(i=0; i<0x08; i++) pinMode(i+4, INPUT);
  for(i=0; i<0x08; i++) pullUpDnControl(i+4, PUD_DOWN);

  // CONFIGURE CF 8-BIT MODE
  outb(ATA1, 0x01);
  outb(ATA7, 0xEF);
  while((inb(ATA7)&0x80)!=0);

  // REQUEST DATA
  outb(ATA6, 0xA0|0x00); // HEAD
  outb(ATA2, 1);         // SECTORS TO READ
  outb(ATA3, 1);         // SECTOR (1-)
  outb(ATA4, 0);         // lo(CYLINDER)
  outb(ATA5, 0);         // hi(CYLINDER)
  outb(ATA7, 0x20);      // READ
  while((inb(ATA7)&8)==0);

  // LOAD DATA
  for(i=0; i<512; i++) {
    printf("%.2x ", inb(ATA0));
    if(i%24==23) printf("\n");
  }
  printf("\n");

  return 0;
}
