#define GPSET0    7
#define GPCLR0   10
#define GPLEV0   13
#define GPFSEL0   0 //  0- 9
#define GPFSEL1   1 // 10-19
#define GPFSEL2   2 // 20-29

#define INPUT     0
#define OUTPUT    1

#define LA       23
#define IOW      25
#define IOR      11
#define MEMR      9
#define MEMW     10
#define RESET    22
#define READY    24

#define D0       27
#define D1       18
#define D2       17
#define D3       15
#define D4       14
#define D5        4
#define D6        3
#define D7        2

#define A0       21
#define A1       20
#define A2       26
#define A3       16
#define A4       19
#define A5       13
#define A6       12
#define A7        6
#define A8        5
#define A9        1
#define A10       0
#define A11       7

volatile int wcnt = 0;
volatile int *gpio;
volatile int gpio_clear = 0;
volatile int gpio_set = 0;
volatile int mode = 0;
volatile int oldaddr = 0;

void setinput(void) {
  gpio[GPFSEL0] = 0x08248009;
  gpio[GPFSEL1] = 0x08040249;
  gpio[GPFSEL2] = 0x00048249;
  mode = 0;
}

void setoutput(void) {
  gpio[GPFSEL0] = 0x08249249;
  gpio[GPFSEL1] = 0x09249249;
  gpio[GPFSEL2] = 0x00248249;
  mode = 1;
}

void digitalWrite(int pin, int state) {
  if(state==0) gpio_clear |= (1<<pin);
  if(state==1) gpio_set |= (1<<pin);
}

int digitalRead(int pin) {
  return (gpio[GPLEV0] >> pin) & 1;
}

void updatePins(void) {
  gpio[GPCLR0] = gpio_clear;
  gpio[GPCLR0] = gpio_clear;
  gpio[GPSET0] = gpio_set;
  gpio[GPSET0] = gpio_set;
  gpio_clear = 0;
  gpio_set = 0;
}

/****/

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

/****/

void outb(unsigned short addr, unsigned char data) {
  //if(mode==0)
  setoutput();
  setaddr(addr);
  setdata(data); updatePins();
  digitalWrite(IOW, 0); updatePins();
  usleep(1);
  digitalWrite(IOW, 1); updatePins();
  usleep(1);
}

unsigned char inb(unsigned short addr) {
  unsigned char data;

  //if(mode==1)
  setinput();
  setaddr(addr); updatePins();
  digitalWrite(IOR, 0); updatePins();
  usleep(1);
  data = getdata();
  digitalWrite(IOR, 1); updatePins();

  return data;
}

void memw(unsigned int addr, unsigned char data) {
  //if(mode==0)
  setoutput();
  if( ((addr >> 12) & 0xFF) != ((oldaddr >> 12) & 0xFF) ) {
    setaddr((addr >> 12) & 0xFF);
    digitalWrite(LA, 1); updatePins();
    digitalWrite(LA, 0); updatePins();
    oldaddr = addr;
  }
  setaddr(addr & 0xFFF); updatePins();
  setdata(data); updatePins();
  digitalWrite(MEMW, 0); updatePins();
  while(!digitalRead(READY));
  digitalWrite(MEMW, 1); updatePins();
}

unsigned char memr(unsigned int addr) {
  unsigned char data;

  //if(mode==1)
  setinput();
  if( ((addr >> 12) & 0xFF) != ((oldaddr >> 12) & 0xFF)) {
    setaddr((addr >> 12) & 0xFF);
    digitalWrite(LA, 1); updatePins();
    digitalWrite(LA, 0); updatePins();
    oldaddr = addr;
  }
  setaddr(addr & 0xFFF); updatePins();
  digitalWrite(MEMR, 0); updatePins();
  while(!digitalRead(READY));
  data = getdata();
  digitalWrite(MEMR, 1); updatePins();

  return data;
}
