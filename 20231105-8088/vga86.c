#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "cputable.h"
#include <sys/time.h>
#include "isa.h"
#include "vga.h"

#define BACKSPACE  8
#define TAB        9
#define ENTER     13
#define ESC       27

unsigned char k_normal_tab[] = {
0,
ESC,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE,
TAB,  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER, 0,
      'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',  0, 0,
'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0,  0, 0,
' '
};

unsigned char k_shift_tab[] = {
0,
ESC, '!', '@', '#', '$', '%', 0x5e, '&', '*', '(', ')', '_', '+', BACKSPACE,
TAB, 'Q', 'W', 'E', 'R', 'T',  'Y', 'U', 'I', 'O', 'P', '{', '}', ENTER, 0,
     'A', 'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L', ':', '\"',  0, 0,
'|', 'Z', 'X', 'C', 'V', 'B',  'N', 'M', '<', '>', '?',    0,  0, 0,
' '
};

unsigned char k_ctrl_tab[] = {
0,
0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0,
0, 17, 23, 5, 18, 20, 25, 21,  9, 15, 16, 0, 0, 0, 0,
    1, 19, 4,  6,  7,  8, 10, 11, 12,  0, 0, 0, 0,
0, 26, 24, 3, 22,  2, 14, 13,  0,  0,  0, 0, 0, 0,
0
};

unsigned char vgapal[] = {
	0x00, 0x00, 0x00,
	0x00, 0x00, 0x2a,
	0x00, 0x2a, 0x00,
	0x00, 0x2a, 0x2a,
	0x2a, 0x00, 0x00,
	0x2a, 0x00, 0x2a,
	0x2a, 0x15, 0x00,
	0x2a, 0x2a, 0x2a,
	0x15, 0x15, 0x15,
	0x15, 0x15, 0x3f,
	0x15, 0x3f, 0x15,
	0x15, 0x3f, 0x3f,
	0x3f, 0x15, 0x15,
	0x3f, 0x15, 0x3f,
	0x3f, 0x3f, 0x15,
	0x3f, 0x3f, 0x3f};

#define REGS_BASE 1024*1024-8192

#define REG_AX 0
#define REG_CX 1
#define REG_DX 2
#define REG_BX 3
#define REG_SP 4
#define REG_BP 5
#define REG_SI 6
#define REG_DI 7
#define REG_ES 8
#define REG_CS 9
#define REG_SS 10
#define REG_DS 11
#define REG_ZERO 12
#define REG_SCRATCH 13
#define REG_AL 0
#define REG_AH 1
#define REG_CL 2
#define REG_CH 3
#define REG_DL 4
#define REG_DH 5
#define REG_BL 6
#define REG_BH 7
#define FLAG_CF 40
#define FLAG_PF 41
#define FLAG_AF 42
#define FLAG_ZF 43
#define FLAG_SF 44
#define FLAG_IF 46
#define FLAG_DF 47
#define FLAG_OF 48

#define TABLE_XLAT_OPCODE 8
#define TABLE_XLAT_SUBFUNCTION 9
#define TABLE_STD_FLAGS 10
#define TABLE_PARITY_FLAG 11
#define TABLE_BASE_INST_SIZE 12
#define TABLE_I_W_SIZE 13
#define TABLE_I_MOD_SIZE 14
#define TABLE_COND_JUMP_DECODE_A 15
#define TABLE_COND_JUMP_DECODE_B 16
#define TABLE_COND_JUMP_DECODE_C 17
#define TABLE_COND_JUMP_DECODE_D 18
#define TABLE_FLAGS_BITFIELDS 19

#define FLAGS_UPDATE_SZP 1
#define FLAGS_UPDATE_AO_ARITH 2
#define FLAGS_UPDATE_OC_LOGIC 4

#define DECODE_RM_REG scratch2_uint = 4 * !i_mod, op_to_addr = rm_addr = i_mod < 3 ? (16 * regs16[seg_override_en ? seg_override : instructions[((scratch2_uint + 3)<<8)+i_rm]] + (unsigned short)( regs16[instructions[((scratch2_uint + 1)<<8)+i_rm]] + instructions[((scratch2_uint + 2)<<8)+i_rm] * i_data1 + regs16[instructions[(scratch2_uint<<8)+i_rm]])) : GET_REG_ADDR(i_rm), op_from_addr = GET_REG_ADDR(i_reg), i_d && (scratch_uint = op_from_addr, op_from_addr = rm_addr, op_to_addr = scratch_uint)
#define GET_REG_ADDR(reg_id) (REGS_BASE + (i_w ? 2 * reg_id : 2 * reg_id + (reg_id >> 2) & 7))
#define TOP_BIT 8*(i_w + 1)
#define SIGN_OF(a) (1 & (i_w ? *(short*)&a : a) >> (TOP_BIT - 1))
#define DAA_DAS(op1,op2,mask,min) set_AF((((scratch2_uint = regs8[REG_AL]) & 0x0F) > 9) || regs8[FLAG_AF]) && (op_result = regs8[REG_AL] op1 6, set_CF(regs8[FLAG_CF] || (regs8[REG_AL] op2 scratch2_uint))), set_CF((((mask & 1 ? scratch2_uint : regs8[REG_AL]) & mask) > min) || regs8[FLAG_CF]) && (op_result = regs8[REG_AL] op1 0x60)

unsigned char mem[1024*1024], io_ports[1024], *opcode_stream, *regs8, i_rm, i_w, i_reg, i_mod, i_mod_size, i_d, i_reg4bit, raw_opcode_id, xlat_opcode_id, extra, rep_mode, seg_override_en, rep_override_en, trap_flag, scratch_uchar, io_hi_lo;
unsigned short *regs16, reg_ip, seg_override;
unsigned int op_source, op_dest, rm_addr, op_to_addr, op_from_addr, i_data0, i_data1, i_data2, scratch_uint, scratch2_uint, inst_counter, set_flags_type;
int op_result, scratch_int;

char set_CF(int new_CF) {
	return regs8[FLAG_CF] = (new_CF!=0);
}

char set_AF(int new_AF) {
	return regs8[FLAG_AF] = (new_AF!=0);
}

char set_OF(int new_OF) {
	return regs8[FLAG_OF] = (new_OF!=0);
}

char set_AF_OF_arith() {
	op_source ^= op_dest ^ op_result;
	set_AF(op_source & 0x10);
	if(op_result == op_dest)
		return set_OF(0);
	else
		return set_OF(1 & (regs8[FLAG_CF] ^ op_source >> (TOP_BIT - 1)));
}

void make_flags() {
	int i;
	scratch_uint = 0xF002;
	for(i=9; i--;)
		scratch_uint += regs8[FLAG_CF + i] << instructions[(TABLE_FLAGS_BITFIELDS<<8)+i];
}

void set_flags(int new_flags) {
	int i;
	for(i=9; i--;)
		regs8[FLAG_CF + i] = ((1 << instructions[(TABLE_FLAGS_BITFIELDS<<8)+i] & new_flags)!=0);
}

void set_opcode(unsigned char opcode) {
	xlat_opcode_id = instructions[(TABLE_XLAT_OPCODE<<8)+(raw_opcode_id = opcode)];
	extra = instructions[(TABLE_XLAT_SUBFUNCTION<<8)+opcode];
	i_mod_size = instructions[(TABLE_I_MOD_SIZE<<8)+opcode];
	set_flags_type = instructions[(TABLE_STD_FLAGS<<8)+opcode];
}

char pc_interrupt(unsigned char interrupt_num) {
	set_opcode(0xCD);
	make_flags();
	i_w = 1;

	regs16[REG_SP]-=2; *(unsigned short*)&mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = scratch_uint;
	regs16[REG_SP]-=2; *(unsigned short*)&mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_CS];
	regs16[REG_SP]-=2; *(unsigned short*)&mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = reg_ip;

	*(unsigned short*)&mem[REGS_BASE+2*REG_CS] = *(unsigned short*)&mem[4*interrupt_num+2];
	reg_ip = *(unsigned short*)&mem[4*interrupt_num];

	return regs8[FLAG_IF] = 0;
}

int AAA_AAS(char which_operation) {
	return (regs16[REG_AX] += 262 * which_operation*set_AF(set_CF(((regs8[REG_AL] & 0x0F) > 9) || regs8[FLAG_AF])), regs8[REG_AL] &= 0x0F);
}

unsigned int palet[256];

int main(int argc, char *argv[]) {
	SDL_Window *window;
	//unsigned int *screen;
	SDL_Event sdl_event;

	int x, y, z;
	unsigned char *disk;
	unsigned char k_shift = 0, k_ctrl = 0;
	struct timeval now, was;
	float pit_tick = 65536.0/1193.18, pit_elapsed = 0.0;
	unsigned char pit, skipx, running=1, palidx, palc, areg, adet;
	unsigned int i, j, k, c, h, s, tnew, told, a, b, d, g;
	FILE *f;

	unsigned int *mem32 = (unsigned int *)mem;
	unsigned short *mem16 = (unsigned short *)mem;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("8088", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 200, 0);
    //SDL_GL_CreateContext(window);
    //SDL_GL_SetSwapInterval(1);
    //glBindTexture(GL_TEXTURE_2D, texture);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glEnable(GL_TEXTURE_2D);
	//screen = (unsigned int *)calloc(320*200, 4);

	gpio = (unsigned int*)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, open("/dev/gpiomem", O_RDWR|O_SYNC), 0);
	setoutput();
	gpio[GPCLR0] = 0x0FFFFFFF;
	digitalWrite(IOW, 1);
	digitalWrite(IOR, 1);
	digitalWrite(MEMW, 1);
	digitalWrite(MEMR, 1);
	digitalWrite(RESET, 1); updatePins(); usleep(200000);
	digitalWrite(RESET, 0); updatePins(); usleep(300000);
	for(i=0; i<256; i++) memw(i, 0);

	printf("Reading VGABIOS..."); fflush(stdout);
	for(i=0xC0000; i<0xD0000; i++)
		mem[i] = memr(i);
	printf("ok.\n"); fflush(stdout);
	init_vga();
	for(y=0; y<200; y++)
		for(x=0; x<320; x++)
			memw(0xA0000 + x + y*320, (x & 63) + 64*((y >> 3)& 3));
	for(x=0; x<32000; x++)
		memw(0xA0000 + x, rand() & 255);

	// setup interrupt table (initially everything points to iret at the beginning of the BIOS ROM)
	for(i=0; i<256; i++)
		mem32[i] = 0xf000f000;

	// load BIOS ROM
	f = fopen("vga86.bin", "rb");
	fread(mem + 0xFF000, 4096, 1, f);
	fclose(f);

	// setup keyboard (BDA)
	mem[16*0x40+0x1a] = 0x1e;
	mem[16*0x40+0x1c] = 0x1e;

/*
	for(i=0; i<256; i++) memw(0xA0000+i, i);
	init_vga();
	for(y=0; y<100; y++)
		for(x=0; x<320; x++)
			memw(0xA0000 + x + y*320, (x & 63) + 64*((y >> 3)& 3));
*/

	// load HDD image
	printf("Reading HDD..."); fflush(stdout);
        disk = (unsigned char *)calloc(263725056, 1);
        f = fopen("harddisk.img", "rb");
        fread(disk, 263725056, 1, f);
        fclose(f);
	printf("ok.\n"); fflush(stdout);

        // load the boot sector
        for(i=0; i<512; i++)
                mem[0x7c00+i] = disk[i];

        // setup default EGA/CGA palette
        for(i=0; i<256; i++) {
                if(i<16) palet[i] = (vgapal[3*i] << 2) | (vgapal[3*i+1] << 10) | (vgapal[3*i+2] << 18);
		outb(0x3c8, i);
		if(i<16) {
			outb(0x3c9, vgapal[3*i+0] & 63);
			outb(0x3c9, vgapal[3*i+1] & 63);
			outb(0x3c9, vgapal[3*i+2] & 63);
		} else {
			outb(0x3c9, 0);
			outb(0x3c9, 0);
			outb(0x3c9, 0);
		}
	}

	// jump to BIOS ROM
        regs8 = mem + REGS_BASE;
        regs16 = (unsigned short *)regs8;
	regs16[REG_CS] = 0xF000;
        reg_ip = 0xF001;

	while(running)	{
		opcode_stream = mem + 16 * regs16[REG_CS] + reg_ip;
		set_opcode(*opcode_stream);
		i_reg4bit = raw_opcode_id & 7;
		i_w = i_reg4bit & 1;
		i_d = (i_reg4bit >> 1) & 1;
		i_data0 = *(short*)&opcode_stream[1];
		i_data1 = *(short*)&opcode_stream[2];
		i_data2 = *(short*)&opcode_stream[3];
		if(seg_override_en) seg_override_en--;
		if(rep_override_en) rep_override_en--;
		if(i_mod_size) {
			i_mod = (i_data0 & 0xFF) >> 6;
			i_rm = i_data0 & 7;
			i_reg = (i_data0 >> 3) & 7;

			if((!i_mod && i_rm == 6) || (i_mod == 2)) i_data2 = *(short*)&opcode_stream[4];
			else if(i_mod != 1)	i_data2 = i_data1;
			else i_data1 = (char)i_data1;

			DECODE_RM_REG;
		}

		skipx = 0;

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// // // // //

		switch(raw_opcode_id) {
			case 0x0F:
				if(opcode_stream[1]==9) {
					if(k_shift) mem[16*0x40+0x17] = 2;
					else if(k_ctrl) mem[16*0x40+0x17] = 4;
					else mem[16*0x40+0x17] = 0;
					if(regs8[REG_AL] < 0x80) {
						i = mem[16*0x40+0x1c];
						mem[16*0x40+i+1] = regs8[REG_AL];
						mem[16*0x40+i] = k_normal_tab[regs8[REG_AL]];
						if(k_shift) mem[16*0x40+i] = k_shift_tab[regs8[REG_AL]];
						else if(k_ctrl) mem[16*0x40+i] = k_ctrl_tab[regs8[REG_AL]];
						if(regs8[REG_AL]>0x39) mem[16*0x40+i] = 0;
						i = i + 2;
						if(i==0x3e) mem[16*0x40+0x1c] = i - 32;
						else mem[16*0x40+0x1c] = i;
					}
				}
				reg_ip = reg_ip + 2;
				break;
                        case 0xCD: // INT imm8
				reg_ip = reg_ip + 2;
				if((unsigned char)i_data0==0x10 && regs8[REG_AH]==0x0e) {
					if(regs8[REG_AH]==0x0e) {
						printf("%c", regs8[REG_AL]);
						fflush(stdout);
					}
				} else
                                switch((unsigned char)i_data0) {
                                        case 0x10: // VIDEO
                                                if(regs8[REG_AH]==0);
						else if(regs8[REG_AH]==0x10) {
                                                        if(regs8[REG_AL]==0x10) {
								outb(0x3c8, regs16[REG_BX]);
								outb(0x3c9, regs8[REG_DH]);
								outb(0x3c9, regs8[REG_CH]);
								outb(0x3c9, regs8[REG_CL]);
                                                                palet[regs16[REG_BX]] = (regs8[REG_DH] << 2) | (regs8[REG_CH] << 10) | (regs8[REG_CL] << 18);
                                                        }
                                                        if(regs8[REG_AL]==0x15) {
								regs8[REG_DH] = (palet[regs16[REG_BX]] >> 2) & 63;
								regs8[REG_CH] = (palet[regs16[REG_BX]] >> 10) & 63;
								regs8[REG_CL] = (palet[regs16[REG_BX]] >> 18) & 63;
                                                        }
                                                } else {
                                                        regs8[REG_AL] = 0x1A;
                                                        regs16[REG_BX] = 8;
                                                }
                                                break;
					case 0x12:
						regs16[REG_AX] = 640;
						break;
                                        case 0x13: // STORAGE
                                                regs8[FLAG_CF] = 0;
                                                if(regs8[REG_AH]==2) {
                                                        c = ((regs8[REG_CL]&0xC0)<<2)|regs8[REG_CH];
                                                        h = regs8[REG_DH];
                                                        s = regs8[REG_CL]&63;
                                                        j = 512*(63*(16*c+h)+s-1);
                                                        for(i=0; i<512*regs8[REG_AL]; i++)
                                                                mem[(regs16[REG_ES] << 4) + regs16[REG_BX] + i] = disk[j + i];
                                                } else
                                                if(regs8[REG_AH]==3) {
                                                        c = ((regs8[REG_CL]&0xC0)<<2)|regs8[REG_CH];
                                                        h = regs8[REG_DH];
                                                        s = regs8[REG_CL]&63;
                                                        j = 512*(63*(16*c+h)+s-1);
                                                        for(i=0; i<512*regs8[REG_AL]; i++)
                                                                disk[j + i] = mem[(regs16[REG_ES] << 4) + regs16[REG_BX] + i];
                                                } else
                                                if(regs8[REG_AH]==8) { // CHS: 511/16/63
                                                        regs16[REG_CX] = 0xfd7f;
                                                        regs16[REG_DX] = 0x0f01;
                                                }
                                                break;
					case 0x16: // KEYBOARD (SW)
						if(regs8[REG_AH]==0) {
							i = mem[16*0x40+0x1a];
							regs8[REG_AL] = mem[16*0x40+i];
							regs8[REG_AH] = mem[16*0x40+i+1];
							i = i + 2;
							if(i==0x3e) mem[16*0x40+0x1a] = i - 32;
							else mem[16*0x40+0x1a] = i;
							break;
						} else
						if(regs8[REG_AH]==1) {
							if(mem[16*0x40+0x1a]!=mem[16*0x40+0x1c])
								regs8[FLAG_ZF] = 0;
							else
								regs8[FLAG_ZF] = 1;
						} else regs16[REG_AX] = 0;
						break;
                                        default:
						//i = (unsigned char)i_data0;
						//if(i==0x21) { }
                                                //pc_interrupt((unsigned char)i_data0);

						scratch_uint = 0xF002;
						for(i=9; i--;)
							scratch_uint += regs8[FLAG_CF + i] << instructions[(TABLE_FLAGS_BITFIELDS<<8)+i];
                                                regs16[REG_SP] = regs16[REG_SP] - 2;
                                                mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = 4;
                                                mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = 2;
                                                regs16[REG_SP] = regs16[REG_SP] - 2;
                                                mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = regs16[REG_CS] & 255;
                                                mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_CS] >> 8) & 255;
                                                regs16[REG_SP] = regs16[REG_SP] - 2;
                                                mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = reg_ip & 255;
                                                mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (reg_ip >> 8) & 255;
                                                regs16[REG_CS] = mem16[2*(unsigned char)i_data0+1];
                                                reg_ip = mem16[2*(unsigned char)i_data0];
						regs8[FLAG_IF] = 0;

                                                break;
				}
				break;
			case 0x06: // PUSH ES
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = regs16[REG_ES] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_ES] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x0E: // PUSH CS
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = regs16[REG_CS] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_CS] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x16: // PUSH SS
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = regs16[REG_SS] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_SS] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x1E: // PUSH DS
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = regs16[REG_DS] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_DS] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x07: // POP ES
				regs16[REG_SP] += 2;
				regs16[REG_ES] = (mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]+1]<<8)|mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x17: // POP SS
				regs16[REG_SP] += 2;
				regs16[REG_SS] = (mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]+1]<<8)|mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x1F: // POP DS
				regs16[REG_SP] += 2;
				regs16[REG_DS] = (mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]+1]<<8)|mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x26: // ES:
				seg_override_en = 2;
				seg_override = 8;
				if(rep_override_en) rep_override_en = rep_override_en + 1;
				reg_ip = reg_ip + 1;
				break;
			case 0x36: // SS:
				seg_override_en = 2;
				seg_override = 10;
				if(rep_override_en) rep_override_en = rep_override_en + 1;
				reg_ip = reg_ip + 1;
				break;
			case 0x2E: // CS:
				seg_override_en = 2;
				seg_override = 9;
				if(rep_override_en) rep_override_en = rep_override_en + 1;
				reg_ip = reg_ip + 1;
				break;
			case 0x3E: // DS:
				seg_override_en = 2;
				seg_override = 11;
				if(rep_override_en) rep_override_en = rep_override_en + 1;
				reg_ip = reg_ip + 1;
				break;
			case 0x50: // PUSH AX
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_AX] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_AX] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x51: // PUSH CX
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_CX] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_CX] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x52: // PUSH DX
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_DX] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_DX] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x53: // PUSH BX
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_BX] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_BX] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x54: // PUSH SP
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_SP] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_SP] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x55: // PUSH BP
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_BP] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_BP] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x56: // PUSH SI
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_SI] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_SI] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x57: // PUSH DI
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]] = regs16[REG_DI] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_DI] >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x58: // POP AX
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_AX] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x59: // POP CX
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_CX] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x5A: // POP DX
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_DX] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x5B: // POP BX
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_BX] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x5C: // POP SP
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_SP] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x5D: // POP BP
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_BP] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x5E: // POP SI
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_SI] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x5F: // POP DI
				regs16[REG_SP] = regs16[REG_SP] + 2;
				regs16[REG_DI] = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8) | mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				reg_ip = reg_ip + 1;
				break;
			case 0x90: // XCHG AX, AX (NOP)
				reg_ip = reg_ip + 1;
				break;
			case 0x91: // XCHG AX, CX
				i = regs16[REG_CX];
				regs16[REG_CX] = regs16[REG_AX];
				regs16[REG_AX] = i;
				reg_ip = reg_ip + 1;
				break;
			case 0x92: // XCHG AX, DX
				i = regs16[REG_DX];
				regs16[REG_DX] = regs16[REG_AX];
				regs16[REG_AX] = i;
				reg_ip = reg_ip + 1;
				break;
			case 0x93: // XCHG AX, BX
				i = regs16[REG_BX];
				regs16[REG_BX] = regs16[REG_AX];
				regs16[REG_AX] = i;
				reg_ip = reg_ip + 1;
				break;
			case 0x94: // XCHG AX, SP
				i = regs16[REG_SP];
				regs16[REG_SP] = regs16[REG_AX];
				regs16[REG_AX] = i;
				reg_ip = reg_ip + 1;
				break;
			case 0x95: // XCHG AX, BP
				i = regs16[REG_BP];
				regs16[REG_BP] = regs16[REG_AX];
				regs16[REG_AX] = i;
				reg_ip = reg_ip + 1;
				break;
			case 0x96: // XCHG AX, SI
				i = regs16[REG_SI];
				regs16[REG_SI] = regs16[REG_AX];
				regs16[REG_AX] = i;
				reg_ip = reg_ip + 1;
				break;
			case 0x97: // XCHG AX, DI
				i = regs16[REG_DI];
				regs16[REG_DI] = regs16[REG_AX];
				regs16[REG_AX] = i;
				reg_ip = reg_ip + 1;
				break;
			case 0x9A: // CALL imm16:imm16
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = regs16[REG_CS] & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (regs16[REG_CS] >> 8) & 255;
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = (reg_ip + 5) & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = ((reg_ip + 5) >> 8 ) & 255;
				regs16[REG_CS] = i_data2;
				reg_ip = i_data0;
				break;
			case 0x9C: // PUSHF
				scratch_uint  = regs8[FLAG_CF] | (regs8[FLAG_CF+1] << 2) | (regs8[FLAG_CF+2] << 4) | (regs8[FLAG_CF+3] << 6) | (regs8[FLAG_CF+4] << 7) | (regs8[FLAG_CF+5] << 8) | (regs8[FLAG_CF+6] << 9) | (regs8[FLAG_CF+7] << 10) | (regs8[FLAG_CF+8] << 11);
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+0] = scratch_uint & 255;
				mem[(regs16[REG_SS]<<4)+regs16[REG_SP]+1] = (scratch_uint >> 8) & 255;
				reg_ip = reg_ip + 1;
				break;
			case 0x9D: // POPF
				regs16[REG_SP] = regs16[REG_SP] + 2;
				scratch_uint = (mem[(regs16[REG_SS]<<4)-1+regs16[REG_SP]]<<8)|mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
				regs8[FLAG_CF+0] = ((1 & scratch_uint)!=0);
				regs8[FLAG_CF+1] = ((4 & scratch_uint)!=0);
				regs8[FLAG_CF+2] = ((16 & scratch_uint)!=0);
				regs8[FLAG_CF+3] = ((64 & scratch_uint)!=0);
				regs8[FLAG_CF+4] = ((128 & scratch_uint)!=0);
				regs8[FLAG_CF+5] = ((256 & scratch_uint)!=0);
				regs8[FLAG_CF+6] = ((512 & scratch_uint)!=0);
				regs8[FLAG_CF+7] = ((1024 & scratch_uint)!=0);
				regs8[FLAG_CF+8] = ((2048 & scratch_uint)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x9E: // SAHF
				scratch_uint =  (regs8[FLAG_CF+8] << 11) | (regs8[FLAG_CF+7] << 10) | (regs8[FLAG_CF+6] << 9) | (regs8[FLAG_CF+5] << 8) | regs8[REG_AH];
				regs8[FLAG_CF+0] = ((1 & scratch_uint)!=0);
				regs8[FLAG_CF+1] = ((4 & scratch_uint)!=0);
				regs8[FLAG_CF+2] = ((16 & scratch_uint)!=0);
				regs8[FLAG_CF+3] = ((64 & scratch_uint)!=0);
				regs8[FLAG_CF+4] = ((128 & scratch_uint)!=0);
				regs8[FLAG_CF+5] = ((256 & scratch_uint)!=0);
				regs8[FLAG_CF+6] = ((512 & scratch_uint)!=0);
				regs8[FLAG_CF+7] = ((1024 & scratch_uint)!=0);
				regs8[FLAG_CF+8] = ((2048 & scratch_uint)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x9F: // LAHF
				regs8[REG_AH] = regs8[FLAG_CF] | (regs8[FLAG_CF+1] << 2) | (regs8[FLAG_CF+2] << 4) | (regs8[FLAG_CF+3] << 6) | (regs8[FLAG_CF+4] << 7);
				reg_ip = reg_ip + 1;
				break;
			case 0xA8: // TEST AL, imm8
				op_result = regs16[REG_AL] & (i_data0 & 255);
				i = (op_result >> 0) & 1;
				i += (op_result >> 1) & 1;
				i += (op_result >> 2) & 1;
				i += (op_result >> 3) & 1;
				i += (op_result >> 4) & 1;
				i += (op_result >> 5) & 1;
				i += (op_result >> 6) & 1;
				i += (op_result >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (op_result >> 7) & 1;
				regs8[FLAG_ZF] = (op_result==0);
				regs8[FLAG_CF] = 0;
				regs8[FLAG_OF] = 0;
				reg_ip = reg_ip + 2;
				break;
			case 0xA9: // TEST AX, imm16
				op_result = regs16[REG_AX] & i_data0;
				i = (op_result >> 0) & 1;
				i += (op_result >> 1) & 1;
				i += (op_result >> 2) & 1;
				i += (op_result >> 3) & 1;
				i += (op_result >> 4) & 1;
				i += (op_result >> 5) & 1;
				i += (op_result >> 6) & 1;
				i += (op_result >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (op_result >> 15) & 1;
				regs8[FLAG_ZF] = (op_result==0);
				regs8[FLAG_CF] = 0;
				regs8[FLAG_OF] = 0;
				reg_ip = reg_ip + 3;
				break;
			case 0xB0: // MOV AL, imm8
				regs8[REG_AL] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB1: // MOV CL, imm8
				regs8[REG_CL] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB2: // MOV DL, imm8
				regs8[REG_DL] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB3: // MOV BL, imm8
				regs8[REG_BL] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB4: // MOV AH, imm8
				regs8[REG_AH] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB5: // MOV CH, imm8
				regs8[REG_CH] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB6: // MOV DH, imm8
				regs8[REG_DH] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB7: // MOV BH, imm8
				regs8[REG_BH] = i_data0 & 255;
				reg_ip = reg_ip + 2;
				break;
			case 0xB8: // MOV AX, imm16
				regs8[REG_AL] = i_data0 & 255;
				regs8[REG_AH] = (i_data0>>8) & 255;
				reg_ip = reg_ip + 3;
				break;
			case 0xB9: // MOV CX, imm16
				regs8[REG_CL] = i_data0 & 255;
				regs8[REG_CH] = (i_data0>>8) & 255;
				reg_ip = reg_ip + 3;
				break;
			case 0xBA: // MOV DX, imm16
				regs8[REG_DL] = i_data0 & 255;
				regs8[REG_DH] = (i_data0>>8) & 255;
				reg_ip = reg_ip + 3;
				break;
			case 0xBB: // MOV BX, imm16
				regs8[REG_BL] = i_data0 & 255;
				regs8[REG_BH] = (i_data0>>8) & 255;
				reg_ip = reg_ip + 3;
				break;
			case 0xBC: // MOV SP, imm16
				regs16[REG_SP] = i_data0;
				reg_ip = reg_ip + 3;
				break;
			case 0xBD: // MOV BP, imm16
				regs16[REG_BP] = i_data0;
				reg_ip = reg_ip + 3;
				break;
			case 0xBE: // MOV SI, imm16
				regs16[REG_SI] = i_data0;
				reg_ip = reg_ip + 3;
				break;
			case 0xBF: // MOV DI, imm16
				regs16[REG_DI] = i_data0;
				reg_ip = reg_ip + 3;
				break;
			case 0xCC: // INT3
				reg_ip = reg_ip + 1;
				pc_interrupt(3);
				break;
			case 0xCE: // INTO
				if(regs8[FLAG_OF]) {
					pc_interrupt(0);
				}
				reg_ip = reg_ip + 1;
				break;
			case 0xE0: // LOOPxx
			case 0xE1: // LOOPx
			case 0xE2: // LOOP
			case 0xE3: // JCXZ
				regs16[REG_CX] = regs16[REG_CX] - 1;
				scratch_uint = (regs16[REG_CX]!=0);
				if(i_reg4bit==0) scratch_uint = scratch_uint & (!regs8[FLAG_ZF]);
				else if(i_reg4bit==1) scratch_uint = scratch_uint & regs8[FLAG_ZF];
				else if(i_reg4bit==3) {
					regs16[REG_CX] = regs16[REG_CX] + 1;
					scratch_uint = !regs16[REG_CX];
				}
				reg_ip = reg_ip + 2 + scratch_uint*(char)i_data0;
				break;
			case 0xE6: // OUT imm8, AL (?)
				reg_ip = reg_ip + 2;
				switch((unsigned char)i_data0) {
					case 0x40:
						if(pit == 0) pit_tick = 1.0*regs8[REG_AL];
						if(pit++) {
							pit_tick += 256.0*regs8[REG_AL];
							if(pit_tick == 0.0) pit_tick = 65536.0/1193.18;
							else pit_tick = pit_tick/1193.18;
						}
						break;
					case 0x43:
						pit = 0;
						break;
				}
				break;
			case 0xE7: // OUT imm8, AX (?)
				reg_ip = reg_ip + 2;
				outb((i_data0 & 255)+0, regs16[REG_AL]);
				outb((i_data0 & 255)+1, regs16[REG_AH]);
				break;
			case 0xEE: // OUT DX, AL
				reg_ip = reg_ip + 1;
				outb(regs16[REG_DX], regs8[REG_AL]);
				//if(regs16[REG_DX]>0x300) { printf("outb(0x%.3x, 0x%.2x);\n", regs16[REG_DX], regs8[REG_AL]); fflush(stdout); }
				break;
			case 0xEF: // OUT DX, AX (?)
				reg_ip = reg_ip + 1;
				outb(regs16[REG_DX]+0, regs8[REG_AL]);
				outb(regs16[REG_DX]+1, regs8[REG_AH]);
				break;
			case 0xE8: // CALL rel16
				regs16[REG_SP] = regs16[REG_SP] - 2;
				mem[(regs16[REG_SS] << 4) + regs16[REG_SP]] = (reg_ip + 3) & 255;
				mem[(regs16[REG_SS] << 4) + regs16[REG_SP] + 1] = ((reg_ip + 3) >> 8) & 255;
				reg_ip = reg_ip + 3 + (short)i_data0;
				break;
			case 0xE9: // JMP rel16
				reg_ip = reg_ip + 3 + (short)i_data0;
				break;
			case 0xEA: // JMP imm16:imm16
				regs16[REG_CS] = i_data2;
				reg_ip = i_data0;
				break;
			case 0xEB: // JMP rel8
				reg_ip = reg_ip + 2 + (char)(i_data0 & 255);
				break;
			case 0xF2: // REPNE
			case 0xF3: // REPx
				rep_override_en = 2;
				rep_mode = i_w;
				if(seg_override_en) seg_override_en = seg_override_en + 1;
				reg_ip = reg_ip + 1;
				break;
			case 0x40: // INC AX
				op_source = regs16[REG_AX];
				op_dest = regs16[REG_AX];
				regs16[REG_AX]++;
				op_result = regs16[REG_AX];
				i  = (regs16[REG_AX] >> 0) & 1;
				i += (regs16[REG_AX] >> 1) & 1;
				i += (regs16[REG_AX] >> 2) & 1;
				i += (regs16[REG_AX] >> 3) & 1;
				i += (regs16[REG_AX] >> 4) & 1;
				i += (regs16[REG_AX] >> 5) & 1;
				i += (regs16[REG_AX] >> 6) & 1;
				i += (regs16[REG_AX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_AX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_AX]==0);
				regs8[FLAG_OF] = (regs16[REG_AX]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x41: // INC CX
				op_source = regs16[REG_CX];
				op_dest = regs16[REG_CX];
				regs16[REG_CX]++;
				op_result = regs16[REG_CX];
				i  = (regs16[REG_CX] >> 0) & 1;
				i += (regs16[REG_CX] >> 1) & 1;
				i += (regs16[REG_CX] >> 2) & 1;
				i += (regs16[REG_CX] >> 3) & 1;
				i += (regs16[REG_CX] >> 4) & 1;
				i += (regs16[REG_CX] >> 5) & 1;
				i += (regs16[REG_CX] >> 6) & 1;
				i += (regs16[REG_CX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_CX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_CX]==0);
				regs8[FLAG_OF] = (regs16[REG_CX]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x42: // INC DX
				op_source = regs16[REG_DX];
				op_dest = regs16[REG_DX];
				regs16[REG_DX]++;
				op_result = regs16[REG_DX];
				i  = (regs16[REG_DX] >> 0) & 1;
				i += (regs16[REG_DX] >> 1) & 1;
				i += (regs16[REG_DX] >> 2) & 1;
				i += (regs16[REG_DX] >> 3) & 1;
				i += (regs16[REG_DX] >> 4) & 1;
				i += (regs16[REG_DX] >> 5) & 1;
				i += (regs16[REG_DX] >> 6) & 1;
				i += (regs16[REG_DX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_DX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_DX]==0);
				regs8[FLAG_OF] = (regs16[REG_DX]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x43: // INC BX
				op_source = regs16[REG_BX];
				op_dest = regs16[REG_BX];
				regs16[REG_BX]++;
				op_result = regs16[REG_BX];
				i  = (regs16[REG_BX] >> 0) & 1;
				i += (regs16[REG_BX] >> 1) & 1;
				i += (regs16[REG_BX] >> 2) & 1;
				i += (regs16[REG_BX] >> 3) & 1;
				i += (regs16[REG_BX] >> 4) & 1;
				i += (regs16[REG_BX] >> 5) & 1;
				i += (regs16[REG_BX] >> 6) & 1;
				i += (regs16[REG_BX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_BX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_BX]==0);
				regs8[FLAG_OF] = (regs16[REG_BX]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x44: // INC SP
				op_source = regs16[REG_SP];
				op_dest = regs16[REG_SP];
				regs16[REG_SP]++;
				op_result = regs16[REG_SP];
				i  = (regs16[REG_SP] >> 0) & 1;
				i += (regs16[REG_SP] >> 1) & 1;
				i += (regs16[REG_SP] >> 2) & 1;
				i += (regs16[REG_SP] >> 3) & 1;
				i += (regs16[REG_SP] >> 4) & 1;
				i += (regs16[REG_SP] >> 5) & 1;
				i += (regs16[REG_SP] >> 6) & 1;
				i += (regs16[REG_SP] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_SP] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_SP]==0);
				regs8[FLAG_OF] = (regs16[REG_SP]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x45: // INC BP
				op_source = regs16[REG_BP];
				op_dest = regs16[REG_BP];
				regs16[REG_BP]++;
				op_result = regs16[REG_BP];
				i  = (regs16[REG_BP] >> 0) & 1;
				i += (regs16[REG_BP] >> 1) & 1;
				i += (regs16[REG_BP] >> 2) & 1;
				i += (regs16[REG_BP] >> 3) & 1;
				i += (regs16[REG_BP] >> 4) & 1;
				i += (regs16[REG_BP] >> 5) & 1;
				i += (regs16[REG_BP] >> 6) & 1;
				i += (regs16[REG_BP] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_BP] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_BP]==0);
				regs8[FLAG_OF] = (regs16[REG_BP]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x46: // INC SI
				op_source = regs16[REG_SI];
				op_dest = regs16[REG_SI];
				regs16[REG_SI]++;
				op_result = regs16[REG_SI];
				i  = (regs16[REG_SI] >> 0) & 1;
				i += (regs16[REG_SI] >> 1) & 1;
				i += (regs16[REG_SI] >> 2) & 1;
				i += (regs16[REG_SI] >> 3) & 1;
				i += (regs16[REG_SI] >> 4) & 1;
				i += (regs16[REG_SI] >> 5) & 1;
				i += (regs16[REG_SI] >> 6) & 1;
				i += (regs16[REG_SI] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_SI] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_SI]==0);
				regs8[FLAG_OF] = (regs16[REG_SI]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x47: // INC DI
				op_source = regs16[REG_DI];
				op_dest = regs16[REG_DI];
				regs16[REG_DI]++;
				op_result = regs16[REG_DI];
				i  = (regs16[REG_DI] >> 0) & 1;
				i += (regs16[REG_DI] >> 1) & 1;
				i += (regs16[REG_DI] >> 2) & 1;
				i += (regs16[REG_DI] >> 3) & 1;
				i += (regs16[REG_DI] >> 4) & 1;
				i += (regs16[REG_DI] >> 5) & 1;
				i += (regs16[REG_DI] >> 6) & 1;
				i += (regs16[REG_DI] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_DI] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_DI]==0);
				regs8[FLAG_OF] = (regs16[REG_DI]==32768);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x48: // DEC AX
				op_source = regs16[REG_AX];
				op_dest = regs16[REG_AX];
				regs16[REG_AX]--;
				op_result = regs16[REG_AX];
				i  = (regs16[REG_AX] >> 0) & 1;
				i += (regs16[REG_AX] >> 1) & 1;
				i += (regs16[REG_AX] >> 2) & 1;
				i += (regs16[REG_AX] >> 3) & 1;
				i += (regs16[REG_AX] >> 4) & 1;
				i += (regs16[REG_AX] >> 5) & 1;
				i += (regs16[REG_AX] >> 6) & 1;
				i += (regs16[REG_AX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_AX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_AX]==0);
				regs8[FLAG_OF] = (regs16[REG_AX]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x49: // DEC CX
				op_source = regs16[REG_CX];
				op_dest = regs16[REG_CX];
				regs16[REG_CX]--;
				op_result = regs16[REG_CX];
				i  = (regs16[REG_CX] >> 0) & 1;
				i += (regs16[REG_CX] >> 1) & 1;
				i += (regs16[REG_CX] >> 2) & 1;
				i += (regs16[REG_CX] >> 3) & 1;
				i += (regs16[REG_CX] >> 4) & 1;
				i += (regs16[REG_CX] >> 5) & 1;
				i += (regs16[REG_CX] >> 6) & 1;
				i += (regs16[REG_CX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_CX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_CX]==0);
				regs8[FLAG_OF] = (regs16[REG_CX]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x4A: // DEC DX
				op_source = regs16[REG_DX];
				op_dest = regs16[REG_DX];
				regs16[REG_DX]--;
				op_result = regs16[REG_DX];
				i  = (regs16[REG_DX] >> 0) & 1;
				i += (regs16[REG_DX] >> 1) & 1;
				i += (regs16[REG_DX] >> 2) & 1;
				i += (regs16[REG_DX] >> 3) & 1;
				i += (regs16[REG_DX] >> 4) & 1;
				i += (regs16[REG_DX] >> 5) & 1;
				i += (regs16[REG_DX] >> 6) & 1;
				i += (regs16[REG_DX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_DX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_DX]==0);
				regs8[FLAG_OF] = (regs16[REG_DX]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x4B: // DEC BX
				op_source = regs16[REG_BX];
				op_dest = regs16[REG_BX];
				regs16[REG_BX]--;
				op_result = regs16[REG_BX];
				i  = (regs16[REG_BX] >> 0) & 1;
				i += (regs16[REG_BX] >> 1) & 1;
				i += (regs16[REG_BX] >> 2) & 1;
				i += (regs16[REG_BX] >> 3) & 1;
				i += (regs16[REG_BX] >> 4) & 1;
				i += (regs16[REG_BX] >> 5) & 1;
				i += (regs16[REG_BX] >> 6) & 1;
				i += (regs16[REG_BX] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_BX] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_BX]==0);
				regs8[FLAG_OF] = (regs16[REG_BX]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x4C: // DEC SP
				op_source = regs16[REG_SP];
				op_dest = regs16[REG_SP];
				regs16[REG_SP]--;
				op_result = regs16[REG_SP];
				i  = (regs16[REG_SP] >> 0) & 1;
				i += (regs16[REG_SP] >> 1) & 1;
				i += (regs16[REG_SP] >> 2) & 1;
				i += (regs16[REG_SP] >> 3) & 1;
				i += (regs16[REG_SP] >> 4) & 1;
				i += (regs16[REG_SP] >> 5) & 1;
				i += (regs16[REG_SP] >> 6) & 1;
				i += (regs16[REG_SP] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_SP] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_SP]==0);
				regs8[FLAG_OF] = (regs16[REG_SP]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x4D: // DEC BP
				op_source = regs16[REG_BP];
				op_dest = regs16[REG_BP];
				regs16[REG_BP]--;
				op_result = regs16[REG_BP];
				i  = (regs16[REG_BP] >> 0) & 1;
				i += (regs16[REG_BP] >> 1) & 1;
				i += (regs16[REG_BP] >> 2) & 1;
				i += (regs16[REG_BP] >> 3) & 1;
				i += (regs16[REG_BP] >> 4) & 1;
				i += (regs16[REG_BP] >> 5) & 1;
				i += (regs16[REG_BP] >> 6) & 1;
				i += (regs16[REG_BP] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_BP] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_BP]==0);
				regs8[FLAG_OF] = (regs16[REG_BP]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x4E: // DEC SI
				op_source = regs16[REG_SI];
				op_dest = regs16[REG_SI];
				regs16[REG_SI]--;
				op_result = regs16[REG_SI];
				i  = (regs16[REG_SI] >> 0) & 1;
				i += (regs16[REG_SI] >> 1) & 1;
				i += (regs16[REG_SI] >> 2) & 1;
				i += (regs16[REG_SI] >> 3) & 1;
				i += (regs16[REG_SI] >> 4) & 1;
				i += (regs16[REG_SI] >> 5) & 1;
				i += (regs16[REG_SI] >> 6) & 1;
				i += (regs16[REG_SI] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_SI] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_SI]==0);
				regs8[FLAG_OF] = (regs16[REG_SI]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x4F: // DEC DI
				op_source = regs16[REG_DI];
				op_dest = regs16[REG_DI];
				regs16[REG_DI]--;
				op_result = regs16[REG_DI];
				i  = (regs16[REG_DI] >> 0) & 1;
				i += (regs16[REG_DI] >> 1) & 1;
				i += (regs16[REG_DI] >> 2) & 1;
				i += (regs16[REG_DI] >> 3) & 1;
				i += (regs16[REG_DI] >> 4) & 1;
				i += (regs16[REG_DI] >> 5) & 1;
				i += (regs16[REG_DI] >> 6) & 1;
				i += (regs16[REG_DI] >> 7) & 1;
				regs8[FLAG_PF] = i & 1;
				regs8[FLAG_SF] = (regs16[REG_DI] >> 15) & 1;
				regs8[FLAG_ZF] = (regs16[REG_DI]==0);
				regs8[FLAG_OF] = (regs16[REG_DI]==32767);
				regs8[FLAG_AF] = (((op_source ^ (op_dest ^ op_result)) & 0x10)!=0);
				reg_ip = reg_ip + 1;
				break;
			case 0x72: // JC rel8
				if(regs8[FLAG_CF] == 1)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x73: // JNC rel8
				if(regs8[FLAG_CF] == 0)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x74: // JE rel8
				if(regs8[FLAG_ZF] == 1)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x75: // JNE rel8
				if(regs8[FLAG_ZF] == 0)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x76: // JNA rel8
				if((regs8[FLAG_CF] == 1) | (regs8[FLAG_ZF] == 1))
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x77: // JA rel8
				if((regs8[FLAG_CF] == 0) & (regs8[FLAG_ZF] == 0))
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x78: // JS rel8
				if(regs8[FLAG_SF] == 1)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x79: // JNS rel8
				if(regs8[FLAG_SF] == 0)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x7A: // JPE rel8
				if(regs8[FLAG_PF] == 1)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x7B: // JPO rel8
				if(regs8[FLAG_PF] == 0)
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x7C: // JL rel8
				if(regs8[FLAG_SF] != regs8[FLAG_OF])
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x7D: // JNL rel8
				if(regs8[FLAG_SF] == regs8[FLAG_OF])
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x7E: // JLE rel8
				if((regs8[FLAG_ZF] == 1) | (regs8[FLAG_SF] != regs8[FLAG_OF]))
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x7F: // JG rel8
				if((regs8[FLAG_ZF] == 0) & (regs8[FLAG_SF] == regs8[FLAG_OF]))
					reg_ip = reg_ip + 2 + (char)i_data0;
				else
					reg_ip = reg_ip + 2;
				break;
			case 0x98: // CBW
				regs8[REG_AH] = -SIGN_OF(regs8[REG_AL]);
				reg_ip++;
				break;
			case 0x99: // CWD
				regs16[REG_DX] = -SIGN_OF(regs16[REG_AX]);
				reg_ip++;
				break;
			case 0xD7: // XLAT
				if(seg_override_en) regs8[REG_AL] = mem[(regs16[seg_override]<<4)+regs8[REG_AL]+regs16[REG_BX]];
				else regs8[REG_AL] = mem[(regs16[REG_DS]<<4)+regs8[REG_AL]+regs16[REG_BX]];
				reg_ip++;
				break;
			case 0xF5: // CMC
				regs8[FLAG_CF] ^= 1;
				reg_ip++;
				break;
			case 0xF8: // CLC
				regs8[FLAG_CF] = 0;
				reg_ip++;
				break;
			case 0xF9: // STC
				regs8[FLAG_CF] = 1;
				reg_ip++;
				break;
			case 0xFA: // CLI
				regs8[FLAG_IF] = 0;
				reg_ip++;
				break;
			case 0xFB: // STI
				regs8[FLAG_IF] = 1;
				reg_ip++;
				break;
			case 0xFC: // CLD
				regs8[FLAG_DF] = 0;
				reg_ip++;
				break;
			case 0xFD: // STD
				regs8[FLAG_DF] = 1;
				reg_ip++;
				break;
			default:
				skipx = 1;
		}

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// // // // //

		if(skipx) {
		switch(xlat_opcode_id) {
			case 5: // INC|DEC|JMP|CALL|PUSH
				if(i_reg < 2) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_from_addr];
						op_source = *(unsigned short*)&mem[REGS_BASE+2*REG_ZERO];
						op_result = *(unsigned short*)&mem[op_from_addr] += 1 - 2*i_reg + op_source;
					} else {
						op_dest = mem[op_from_addr];
						op_source = *(unsigned char*)&mem[REGS_BASE+2*REG_ZERO];
						op_result = mem[op_from_addr] += 1 - 2*i_reg + op_source;
					}
					op_source = 1;
					set_AF_OF_arith();
					set_OF(op_dest + 1 - i_reg == 1 << (TOP_BIT - 1));
					if(xlat_opcode_id == 5) set_opcode(0x10);
				}
				else if(i_reg != 6) {
					if(!(i_reg - 3)) {
						op_dest = *(unsigned short*)&(mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]]);
						op_source = *(unsigned short*)&regs16[REG_CS];
						op_result = *(unsigned short*)&(mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]]) = op_source;
					}
					if(i_reg & 2) {
						i_w = 1;
						op_dest = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]];
						op_source = *(unsigned short*)&reg_ip + 2 + i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6);
						op_result = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]] = op_source;
					}
					if(i_reg & 1) regs16[REG_CS] = *(short*)&mem[op_from_addr + 2];
					if(i_w) {
						op_dest = *(unsigned short*)&reg_ip;
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&reg_ip = op_source;
					} else {
						op_dest = reg_ip;
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = reg_ip = op_source;
					}
					set_opcode(0x9A);
				} else {
					i_w = 1;
					if(i_w) {
						op_dest = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]];
						op_source = *(unsigned short*)&mem[rm_addr];
						op_result = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]] = op_source;
					}
				}
				break;
			case 6: // TEST r/m, imm16 / NOT|NEG|MUL|IMUL|DIV|IDIV reg
				op_to_addr = op_from_addr;
				if(i_reg==0) {
					set_opcode(0x20);
					reg_ip += i_w + 1;
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&i_data2;
						op_result = *(unsigned short*)&mem[op_to_addr] & op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&i_data2;
						op_result = mem[op_to_addr] & op_source;
					}
				} else
				if(i_reg==2) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] =~ op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] =~ op_source;
					}
				} else
				if(i_reg==3) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] =- op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] =- op_source;
					}
					op_dest = 0;
					set_opcode(0x28);
					set_CF(op_result > op_dest);
				} else
				if(i_reg==4) { // MUL
					if(i_w)	{
						set_opcode(0x10);
						op_result = *(unsigned short*)&mem[rm_addr] * (unsigned short)*regs16;
						regs16[i_w + 1] = op_result >> 16;
						regs16[REG_AX] = op_result;
						set_OF(set_CF(op_result - (unsigned short)op_result));
					} else {
						set_opcode(0x10);
						op_result = *(unsigned char*)&mem[rm_addr] * (unsigned char)*regs8;
						regs8[i_w + 1] = op_result >> 16;
						regs16[REG_AX] = op_result;
						set_OF(set_CF(op_result - (unsigned char)op_result));
					}
				} else
				if(i_reg==5) { // IMUL
					if(i_w)	{
						set_opcode(0x10);
						op_result = *(short*)&mem[rm_addr] * (short)*regs16;
						regs16[i_w + 1] = op_result >> 16;
						regs16[REG_AX] = op_result;
						set_OF(set_CF(op_result - (short)op_result));
					} else {
						set_opcode(0x10);
						op_result = *(char*)&mem[rm_addr] * (char)*regs8;
						regs8[i_w + 1] = op_result >> 16;
						regs16[REG_AX] = op_result;
						set_OF(set_CF(op_result - (char)op_result));
					}
				} else
				if(i_reg==6) { // DIV
					if(i_w) {
						scratch_int = *(unsigned short*)&mem[rm_addr];
						if(scratch_int)
							!(scratch2_uint = (unsigned)(scratch_uint = (regs16[i_w+1] << 16) + regs16[REG_AX]) / scratch_int, scratch2_uint - (unsigned short)scratch2_uint) ? regs16[i_w+1] = scratch_uint - scratch_int * (*regs16 = scratch2_uint) : pc_interrupt(0);
					} else {
						scratch_int = *(unsigned char*)&mem[rm_addr];
						if(scratch_int)
							!(scratch2_uint = (unsigned short)(scratch_uint = (regs8[i_w+1] << 16) + regs16[REG_AX]) / scratch_int, scratch2_uint - (unsigned char)scratch2_uint) ? regs8[i_w+1] = scratch_uint - scratch_int * (*regs8 = scratch2_uint) : pc_interrupt(0);
					}
				} else
				if(i_reg==7) { // IDIV
					if(i_w)	{
						scratch_int = *(short*)&mem[rm_addr];
						if(scratch_int)
							!(scratch2_uint = (int)(scratch_uint = (regs16[i_w+1] << 16) + regs16[REG_AX]) / scratch_int, scratch2_uint - (short)scratch2_uint) ? regs16[i_w+1] = scratch_uint - scratch_int * (*regs16 = scratch2_uint) : pc_interrupt(0);
					} else {
						scratch_int = *(char*)&mem[rm_addr];
						if(scratch_int)
							!(scratch2_uint = (short)(scratch_uint = (regs8[i_w+1] << 16) + regs16[REG_AX]) / scratch_int, scratch2_uint - (char)scratch2_uint) ? regs8[i_w+1] = scratch_uint - scratch_int * (*regs8 = scratch2_uint) : pc_interrupt(0);
					}
				}
				break;
			case 7: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP AL/AX, immed
				rm_addr = REGS_BASE;
				i_data2 = i_data0;
				i_mod = 3;
				i_reg = extra;
				reg_ip--;
			case 8: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP reg, immed
				op_to_addr = rm_addr;
				regs16[REG_SCRATCH] = (i_d |= !i_w) ? (char)i_data2 : i_data2;
				op_from_addr = REGS_BASE + 2 * REG_SCRATCH;
				reg_ip += !i_d + 1;
				set_opcode(0x08 * (extra = i_reg));
			case 9: // ADD|OR|ADC|SBB|AND|SUB|XOR|CMP|MOV reg, r/m /****/
				if(extra==0) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] += op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] += op_source;
					}
					set_CF(op_result < op_dest);
				} else
				if(extra==1) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] |= op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] |= op_source;
					}
				} else
				if(extra==2) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] += regs8[FLAG_CF] + op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] += regs8[FLAG_CF] + op_source;
					}
					set_CF(regs8[FLAG_CF] && (op_result == op_dest) || (op_result < (int)op_dest));
					set_AF_OF_arith();
				} else
				if(extra==3) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] -= regs8[FLAG_CF] + op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] -= regs8[FLAG_CF] + op_source;
					}
					set_CF(regs8[FLAG_CF] && (op_result == op_dest) || (- op_result < -(int)op_dest));
					set_AF_OF_arith();
				} else
				if(extra==4) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] &= op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] &= op_source;
					}
				} else
				if(extra==5) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] -= op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] -= op_source;
					}
					set_CF(op_result > op_dest);
				} else
				if(extra==6) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] ^= op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] ^= op_source;
					}
				} else
				if(extra==7) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] - op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] - op_source;
					}
					set_CF(op_result > op_dest);
				} else
				if(extra==8) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] = op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] = op_source;
					}
				}
				break;
			case 10: // MOV sreg, r/m | POP r/m | LEA reg, r/m
				if(!i_w) { // MOV
					i_w = 1;
					i_reg += 8;
					DECODE_RM_REG;
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] = op_source;
					} else {
						op_dest = mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = mem[op_to_addr] = op_source;
					}
				} else
				if(!i_d) { // LEA
					seg_override_en = 1;
					seg_override = REG_ZERO;
					DECODE_RM_REG;
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_from_addr];
						op_source = *(unsigned short*)&rm_addr;
						op_result = *(unsigned short*)&mem[op_from_addr] = op_source;
					} else {
						op_dest = mem[op_from_addr];
						op_source = *(unsigned char*)&rm_addr;
						op_result = mem[op_from_addr] = op_source;
					}
				}
				else { // POP
					i_w = 1;
					regs16[REG_SP] += 2;
					if(i_w) {
						op_dest = *(unsigned short*)&mem[rm_addr];
						op_source = *(unsigned short*)&mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
						op_result = *(unsigned short*)&mem[rm_addr] = op_source;
					} else {
						op_dest = mem[rm_addr];
						op_source = *(unsigned char*)&mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
						op_result = mem[rm_addr] = op_source;
					}
				}
				break;
			case 11: // MOV AL/AX, [loc] /****/
				i_mod = i_reg = 0;
				i_rm = 6;
				i_data1 = i_data0;
				DECODE_RM_REG;
				if(i_w) {
					op_dest = *(unsigned short*)&mem[op_from_addr];
					op_source = *(unsigned short*)&mem[op_to_addr];
					op_result = *(unsigned short*)&mem[op_from_addr] = op_source;
				} else {
					op_dest = mem[op_from_addr];
					op_source = *(unsigned char*)&mem[op_to_addr];
					op_result = mem[op_from_addr] = op_source;
				}
				break;
			case 12: // ROL|ROR|RCL|RCR|SHL|SHR|???|SAR reg/mem, 1/CL/imm (80186)
				scratch2_uint = SIGN_OF(mem[rm_addr]);
				if(extra) {
					++reg_ip;
					scratch_uint = (char)i_data1;
				} else {
					if(i_d) scratch_uint = 31 & regs8[REG_CL];
					else scratch_uint = 1;
				}
				if(scratch_uint) {
					if(i_reg < 4) {
						scratch_uint %= (i_reg >> 1) + TOP_BIT;
						if(i_w) {
							op_dest = *(unsigned short*)&scratch2_uint;
							op_source = *(unsigned short*)&mem[rm_addr];
							op_result = *(unsigned short*)&scratch2_uint = op_source;
						} else {
							op_dest = scratch2_uint;
							op_source = *(unsigned char*)&mem[rm_addr];
							op_result = scratch2_uint = op_source;
					}
					}
					if(i_reg & 1){
						if(i_w) {
							op_dest = *(unsigned short*)&mem[rm_addr];
							op_source = *(unsigned short*)&scratch_uint;
							op_result = *(unsigned short*)&mem[rm_addr] >>= op_source;
						} else {
							op_dest = mem[rm_addr];
							op_source = *(unsigned char*)&scratch_uint;
							op_result = mem[rm_addr] >>= op_source;
						}
					}
					else {
						if(i_w) {
							op_dest = *(unsigned short*)&mem[rm_addr];
							op_source = *(unsigned short*)&scratch_uint;
							op_result = *(unsigned short*)&mem[rm_addr] <<= op_source;
						} else {
							op_dest = mem[rm_addr];
							op_source = *(unsigned char*)&scratch_uint;
							op_result = mem[rm_addr] <<= op_source;
						}
					}
					if(i_reg > 3) set_opcode(0x10);
					if(i_reg > 4) set_CF(op_dest >> (scratch_uint - 1) & 1);
				}
				if(i_reg==0) { // ROL
					if(i_w) {
						op_dest = *(unsigned short*)&mem[rm_addr];
						op_source = *(unsigned short*)&scratch2_uint >> (TOP_BIT - scratch_uint);
						op_result = *(unsigned short*)&mem[rm_addr] += op_source;
					} else {
						op_dest = mem[rm_addr];
						op_source = *(unsigned char*)&scratch2_uint >> (TOP_BIT - scratch_uint);
						op_result = mem[rm_addr] += op_source;
					}
					set_OF(SIGN_OF(op_result) ^ set_CF(op_result & 1));
				} else
				if(i_reg==1) { // ROR
					scratch2_uint &= (1 << scratch_uint) - 1;
					if(i_w) {
						op_dest = *(unsigned short*)&mem[rm_addr];
						op_source = *(unsigned short*)&scratch2_uint << (TOP_BIT - scratch_uint);
						op_result = *(unsigned short*)&mem[rm_addr] += op_source;
					} else {
						op_dest = mem[rm_addr];
						op_source = *(unsigned char*)&scratch2_uint << (TOP_BIT - scratch_uint);
						op_result = mem[rm_addr] += op_source;
					}
					set_OF(SIGN_OF(op_result * 2) ^ set_CF(SIGN_OF(op_result)));
				} else
				if(i_reg==2) { // RCL
					if(i_w) {
						op_dest = *(unsigned short*)&mem[rm_addr];
						op_source = *(unsigned short*)&scratch2_uint >> (1 + TOP_BIT - scratch_uint);
						op_result = *(unsigned short*)&mem[rm_addr] += (regs8[FLAG_CF] << (scratch_uint - 1)) +  op_source;
					} else {
						op_dest = mem[rm_addr];
						op_source = *(unsigned char*)&scratch2_uint >> (1 + TOP_BIT - scratch_uint);
						op_result = mem[rm_addr] += (regs8[FLAG_CF] << (scratch_uint - 1)) +  op_source;
					}
					set_OF(SIGN_OF(op_result) ^ set_CF(scratch2_uint & 1 << (TOP_BIT - scratch_uint)));
				} else
				if(i_reg==3) { // RCR
					if(i_w) {
						op_dest = *(unsigned short*)&mem[rm_addr];
						op_source = *(unsigned short*)&scratch2_uint << (1 + TOP_BIT - scratch_uint);
						op_result = *(unsigned short*)&mem[rm_addr] += (regs8[FLAG_CF] << (TOP_BIT - scratch_uint)) + op_source;
					} else {
						op_dest = mem[rm_addr];
						op_source = *(unsigned char*)&scratch2_uint << (1 + TOP_BIT - scratch_uint);
						op_result = mem[rm_addr] += (regs8[FLAG_CF] << (TOP_BIT - scratch_uint)) + op_source;
					}
					set_CF(scratch2_uint & 1 << (scratch_uint - 1));
					set_OF(SIGN_OF(op_result) ^ SIGN_OF(op_result * 2));
				} else
				if(i_reg==4) { // SHL
					set_OF(SIGN_OF(op_result) ^ set_CF(SIGN_OF(op_dest << (scratch_uint - 1))));
				} else
				if(i_reg==5) { // SHR
					set_OF(SIGN_OF(op_dest));
				} else
				if(i_reg==7) { // SAR
					if(!(scratch_uint < TOP_BIT)) set_CF(scratch2_uint);
					set_OF(0);
					if(i_w) {
						op_dest = *(unsigned short*)&mem[rm_addr];
						op_source = *(unsigned short*)&scratch2_uint *= ~(((1 << TOP_BIT) - 1) >> scratch_uint);
						op_result = *(unsigned short*)&mem[rm_addr] += op_source;
					} else {
						op_dest = mem[rm_addr];
						op_source = *(unsigned char*)&scratch2_uint *= ~(((1 << TOP_BIT) - 1) >> scratch_uint);
						op_result = mem[rm_addr] += op_source;
					}
				}
				break;
			case 15: // TEST reg, r/m
				if(i_w) {
					op_dest = *(unsigned short*)&mem[op_from_addr];
					op_source = *(unsigned short*)&mem[op_to_addr];
					op_result = *(unsigned short*)&mem[op_from_addr] & op_source;
				} else {
					op_dest = mem[op_from_addr];
					op_source = *(unsigned char*)&mem[op_to_addr];
					op_result = mem[op_from_addr] & op_source;
				}
				break;
			case 16: // XCHG AX, regs16
				i_w = 1;
				op_to_addr = REGS_BASE;
				op_from_addr = GET_REG_ADDR(i_reg4bit);
				if(op_to_addr != op_from_addr) {
					op_dest = *(unsigned short*)&mem[op_to_addr];
					op_source = *(unsigned short*)&mem[op_from_addr];
					op_result = *(unsigned short*)&mem[op_to_addr] ^= op_source;
					op_dest = *(unsigned short*)&mem[op_from_addr];
					op_source = *(unsigned short*)&mem[op_to_addr];
					op_result = *(unsigned short*)&mem[op_from_addr] ^= op_source;
					op_dest = *(unsigned short*)&mem[op_to_addr];
					op_source = *(unsigned short*)&mem[op_from_addr];
					op_result = *(unsigned short*)&mem[op_to_addr] ^= op_source;
				}
				break;
			case 17: // MOVSx (extra=0)|STOSx (extra=1)|LODSx (extra=2) /****/
				if(seg_override_en) scratch2_uint = seg_override;
				else scratch2_uint = REG_DS;
				if(rep_override_en) scratch_uint = regs16[REG_CX];
				else scratch_uint = 1;
				while(scratch_uint>0) {
					scratch_uint--;
					if(extra < 2)
						i = (regs16[REG_ES]<<4)+regs16[REG_DI];
					else
						i = REGS_BASE;
					if(extra & 1)
						j = REGS_BASE;
					else
						j = (regs16[scratch2_uint]<<4)+regs16[REG_SI];
					if(i_w) {
						if(i>=0xA0000 && i<=0xBFFFF) {
							memw(i, mem[j]);
							memw(i+1, mem[j+1]);
						}
						op_dest = *(unsigned short*)&mem[i];
						op_source = *(unsigned short*)&mem[j];
						op_result = *(unsigned short*)&mem[i] = op_source;
					} else {
						if(i>=0xA0000 && i<=0xBFFFF)
							memw(i, mem[j]);
						op_dest = mem[i];
						op_source = *(unsigned char*)&mem[j];
						op_result = mem[i] = op_source;
					}
					if(!(extra & 1)) regs16[REG_SI] -= ((regs8[FLAG_DF] << 1) - 1)*(i_w + 1);
					if(!(extra & 2)) regs16[REG_DI] -= ((regs8[FLAG_DF] << 1) - 1)*(i_w + 1);
				}
				if(rep_override_en) regs16[REG_CX] = 0;
				break;
			case 18: // CMPSx (extra=0)|SCASx (extra=1)
				if(seg_override_en) scratch2_uint = seg_override;
				else scratch2_uint = REG_DS;
				if(rep_override_en) scratch_uint = regs16[REG_CX];
				else scratch_uint = 1;
				if(scratch_uint) {
					while(scratch_uint>0) {
						if(!rep_override_en) scratch_uint--;
						if(extra) {
							if(i_w) {
								op_dest = *(unsigned short*)&mem[REGS_BASE];
								op_source = *(unsigned short*)&mem[(regs16[REG_ES]<<4)+regs16[REG_DI]];
								op_result = *(unsigned short*)&mem[REGS_BASE] - op_source;
							} else {
								op_dest = mem[REGS_BASE];
								op_source = *(unsigned char*)&mem[(regs16[REG_ES]<<4)+regs16[REG_DI]];
								op_result = mem[REGS_BASE] - op_source;
							}
						} else {
							if(i_w) {
								op_dest = *(unsigned short*)&mem[(regs16[scratch2_uint]<<4)+regs16[REG_SI]];
								op_source = *(unsigned short*)&mem[(regs16[REG_ES]<<4)+regs16[REG_DI]];
								op_result = *(unsigned short*)&mem[(regs16[scratch2_uint]<<4)+regs16[REG_SI]] - op_source;
							} else {
								op_dest = mem[(regs16[scratch2_uint]<<4)+regs16[REG_SI]];
								op_source = *(unsigned char*)&mem[(regs16[REG_ES]<<4)+regs16[REG_DI]];
								op_result = mem[(regs16[scratch2_uint]<<4)+regs16[REG_SI]] - op_source;
							}
						}
						if(!extra) regs16[REG_SI] -= (2 * regs8[FLAG_DF] - 1)*(i_w + 1);
						regs16[REG_DI] -= (2 * regs8[FLAG_DF] - 1)*(i_w + 1);
						if(rep_override_en && !(--regs16[REG_CX] && (!op_result == rep_mode))) scratch_uint = 0;
					}
					set_flags_type = FLAGS_UPDATE_SZP | FLAGS_UPDATE_AO_ARITH;
					set_CF(op_result > op_dest);
				}
				break;
			case 19: // RET|RETF|IRET
				i_d = i_w;
				i_w = 1;
				regs16[REG_SP] += 2;
				if(i_w) {
					op_dest = *(unsigned short*)&reg_ip;
					op_source = *(unsigned short*)&mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
					op_result = *(unsigned short*)&reg_ip = op_source;
				} else {
					op_dest = reg_ip;
					op_source = *(unsigned char*)&mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
					op_result = reg_ip = op_source;
				}
				if(extra) {
					i_w = 1;
					regs16[REG_SP] += 2;
					op_dest = *(unsigned short*)&regs16[REG_CS];
					op_source = *(unsigned short*)&mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
					op_result = *(unsigned short*)&regs16[REG_CS] = op_source;
				}
				if(extra & 2) {
					i_w = 1;
					regs16[REG_SP] += 2;
					op_dest = *(unsigned short*)&scratch_uint;
					op_source = *(unsigned short*)&mem[(regs16[REG_SS]<<4)-2+regs16[REG_SP]];
					op_result = *(unsigned short*)&scratch_uint = op_source;
					set_flags(op_result);
				}
				else if(!i_d) regs16[REG_SP] += i_data0;
				break;
			case 20: // MOV r/m, immed /****/
				if(i_w) {
					  op_dest = *(unsigned short*)&mem[op_from_addr];
					  op_source = *(unsigned short*)&i_data2;
					  op_result = *(unsigned short*)&mem[op_from_addr] = op_source;
                                          if(op_from_addr>=0xA0000 && op_from_addr<=0xBFFFF) {
                                          	memw(op_from_addr, i_data2&255);
                                          	memw(i+1, (i_data2>>8)&255);
                                          }
				} else {
					  op_dest = mem[op_from_addr];
					  op_source = *(unsigned char*)&i_data2;
					  op_result = mem[op_from_addr] = op_source;
                                          if(op_from_addr>=0xA0000 && op_from_addr<=0xBFFFF)
                                          	memw(op_from_addr, i_data2&255);
				}
				break;
			case 21: // IN AL/AX, DX/imm8
				if(extra)
					scratch_uint = regs16[REG_DX];
				else
					scratch_uint = (unsigned char)i_data0;
				switch(scratch_uint) {
					case 0x60:
						regs8[REG_AL] = io_ports[0x60];
						io_ports[0x64] = 0;
						break;
					case 0x64:
						regs8[REG_AL] = io_ports[0x64];
						break;
					default:
						regs8[REG_AL] = inb(scratch_uint);
						//printf("inb(%.3x) = %.2x\n", scratch_uint, regs8[REG_AL]);
						//fflush(stdout);
						break;
				}
				break;
			case 24: // NOP|XCHG reg, r/m
				if(op_to_addr != op_from_addr) {
					if(i_w) {
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] ^= op_source;
						op_dest = *(unsigned short*)&mem[op_from_addr];
						op_source = *(unsigned short*)&mem[op_to_addr];
						op_result = *(unsigned short*)&mem[op_from_addr] ^= op_source;
						op_dest = *(unsigned short*)&mem[op_to_addr];
						op_source = *(unsigned short*)&mem[op_from_addr];
						op_result = *(unsigned short*)&mem[op_to_addr] ^= op_source;
					} else {
						op_dest = *(unsigned char*)&mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = *(unsigned char*)&mem[op_to_addr] ^= op_source;
						op_dest = *(unsigned char*)&mem[op_from_addr];
						op_source = *(unsigned char*)&mem[op_to_addr];
						op_result = *(unsigned char*)&mem[op_from_addr] ^= op_source;
						op_dest = *(unsigned char*)&mem[op_to_addr];
						op_source = *(unsigned char*)&mem[op_from_addr];
						op_result = *(unsigned char*)&mem[op_to_addr] ^= op_source;
					}
				}
				break;
			case 28: // DAA/DAS
				i_w = 0;
				if(extra)
					DAA_DAS(-=, >=, 0xFF, 0x99);
				else
					DAA_DAS(+=, <, 0xF0, 0x90);
				break;
			case 29: // AAA/AAS
				op_result = AAA_AAS(extra - 1);
				break;
			//// 30 CBW
			//// 31 CWD
			case 32: // CALL FAR imm16:imm16
				i_w = 1;
				op_dest = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]];
				op_source = *(unsigned short*)&regs16[REG_CS];
				op_result = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]] = op_source;
				op_dest = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]];
				op_source = *(unsigned short*)&reg_ip + 5;
				op_result = *(unsigned short*)&mem[(regs16[REG_SS]<<4)+--regs16[REG_SP]] = op_source;
				regs16[REG_CS] = i_data2;
				reg_ip = i_data0;
				break;
			case 37: // LES|LDS reg, r/m
				i_d = 1;
				i_w = 1;
				DECODE_RM_REG;
				op_dest = *(unsigned short*)&mem[op_to_addr];
				op_source = *(unsigned short*)&mem[op_from_addr];
				op_result = *(unsigned short*)&mem[op_to_addr] = op_source;
				op_dest = *(unsigned short*)&mem[REGS_BASE+extra];
				op_source = *(unsigned short*)&mem[rm_addr+2];
				op_result = *(unsigned short*)&mem[REGS_BASE+extra] = op_source;
				break;
			case 41: // AAM
				if(i_data0 &= 0xFF) {
					regs8[REG_AH] = regs8[REG_AL] / i_data0;
					op_result = regs8[REG_AL] %= i_data0;
				}
				else
					pc_interrupt(0);
				break;
			case 42: // AAD
				i_w = 0;
				regs16[REG_AX] = op_result = 0xFF & regs8[REG_AL] + i_data0 * regs8[REG_AH];
				break;
			//// 44 XLAT
			//// 45 CMC
			//// 46 CLC|STC|CLI|STI|CLD|STD
			default:
				break;
		} // switch

		reg_ip += (i_mod*(i_mod != 3) + 2*(!i_mod && i_rm == 6))*i_mod_size + instructions[(TABLE_BASE_INST_SIZE<<8)+raw_opcode_id] + instructions[(TABLE_I_W_SIZE<<8)+raw_opcode_id]*(i_w + 1);

		if(set_flags_type & FLAGS_UPDATE_SZP) {
			regs8[FLAG_SF] = SIGN_OF(op_result);
			regs8[FLAG_ZF] = !op_result;
			regs8[FLAG_PF] = instructions[(unsigned char)op_result + (TABLE_PARITY_FLAG<<8)];
			if(set_flags_type & FLAGS_UPDATE_AO_ARITH) set_AF_OF_arith();
			if(set_flags_type & FLAGS_UPDATE_OC_LOGIC) {
				set_CF(0);
				set_OF(0);
			}
		}
	}

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// // // // //

		if(!seg_override_en && !rep_override_en && regs8[FLAG_IF]) {
			gettimeofday(&now, 0);
			if((now.tv_sec - was.tv_sec)*1000.0 + (now.tv_usec - was.tv_usec)*0.001 > pit_tick) {
				pc_interrupt(8);
				was = now;
			}
		}
		inst_counter++;
		if(!seg_override_en && !rep_override_en && regs8[FLAG_IF] && inst_counter > 40000) {
			inst_counter = 0;
			if(SDL_PollEvent(&sdl_event)) {
				if(sdl_event.type == SDL_QUIT) running = 0;
				if((sdl_event.type == SDL_KEYDOWN) | (sdl_event.type == SDL_KEYUP)) {
					if(sdl_event.key.keysym.sym==SDLK_a) io_ports[0x60] = 0x1E;
					if(sdl_event.key.keysym.sym==SDLK_b) io_ports[0x60] = 0x30;
					if(sdl_event.key.keysym.sym==SDLK_c) io_ports[0x60] = 0x2E;
					if(sdl_event.key.keysym.sym==SDLK_d) io_ports[0x60] = 0x20;
					if(sdl_event.key.keysym.sym==SDLK_e) io_ports[0x60] = 0x12;
					if(sdl_event.key.keysym.sym==SDLK_f) io_ports[0x60] = 0x21;
					if(sdl_event.key.keysym.sym==SDLK_g) io_ports[0x60] = 0x22;
					if(sdl_event.key.keysym.sym==SDLK_h) io_ports[0x60] = 0x23;
					if(sdl_event.key.keysym.sym==SDLK_i) io_ports[0x60] = 0x17;
					if(sdl_event.key.keysym.sym==SDLK_j) io_ports[0x60] = 0x24;
					if(sdl_event.key.keysym.sym==SDLK_k) io_ports[0x60] = 0x25;
					if(sdl_event.key.keysym.sym==SDLK_l) io_ports[0x60] = 0x26;
					if(sdl_event.key.keysym.sym==SDLK_m) io_ports[0x60] = 0x32;
					if(sdl_event.key.keysym.sym==SDLK_n) io_ports[0x60] = 0x31;
					if(sdl_event.key.keysym.sym==SDLK_o) io_ports[0x60] = 0x18;
					if(sdl_event.key.keysym.sym==SDLK_p) io_ports[0x60] = 0x19;
					if(sdl_event.key.keysym.sym==SDLK_q) io_ports[0x60] = 0x10;
					if(sdl_event.key.keysym.sym==SDLK_r) io_ports[0x60] = 0x13;
					if(sdl_event.key.keysym.sym==SDLK_s) io_ports[0x60] = 0x1F;
					if(sdl_event.key.keysym.sym==SDLK_t) io_ports[0x60] = 0x14;
					if(sdl_event.key.keysym.sym==SDLK_u) io_ports[0x60] = 0x16;
					if(sdl_event.key.keysym.sym==SDLK_v) io_ports[0x60] = 0x2F;
					if(sdl_event.key.keysym.sym==SDLK_w) io_ports[0x60] = 0x11;
					if(sdl_event.key.keysym.sym==SDLK_x) io_ports[0x60] = 0x2D;
					if(sdl_event.key.keysym.sym==SDLK_y) io_ports[0x60] = 0x15;
					if(sdl_event.key.keysym.sym==SDLK_z) io_ports[0x60] = 0x2C;

					if(sdl_event.key.keysym.sym==SDLK_1) io_ports[0x60] = 2;
					if(sdl_event.key.keysym.sym==SDLK_2) io_ports[0x60] = 3;
					if(sdl_event.key.keysym.sym==SDLK_3) io_ports[0x60] = 4;
					if(sdl_event.key.keysym.sym==SDLK_4) io_ports[0x60] = 5;
					if(sdl_event.key.keysym.sym==SDLK_5) io_ports[0x60] = 6;
					if(sdl_event.key.keysym.sym==SDLK_6) io_ports[0x60] = 7;
					if(sdl_event.key.keysym.sym==SDLK_7) io_ports[0x60] = 8;
					if(sdl_event.key.keysym.sym==SDLK_8) io_ports[0x60] = 9;
					if(sdl_event.key.keysym.sym==SDLK_9) io_ports[0x60] = 10;
					if(sdl_event.key.keysym.sym==SDLK_0) io_ports[0x60] = 11;

					if(sdl_event.key.keysym.sym==SDLK_EQUALS) io_ports[0x60] = 0x0D;
					if(sdl_event.key.keysym.sym==SDLK_BACKSLASH) io_ports[0x60] = 0x2B;
					if(sdl_event.key.keysym.sym==SDLK_SLASH) io_ports[0x60] = 0x35;
					if(sdl_event.key.keysym.sym==SDLK_PERIOD) io_ports[0x60] = 0x34;
					if(sdl_event.key.keysym.sym==SDLK_SEMICOLON) io_ports[0x60] = 0x27;
					if(sdl_event.key.keysym.sym==SDLK_COMMA) io_ports[0x60] = 0x33;
					if(sdl_event.key.keysym.sym==SDLK_LSHIFT) io_ports[0x60] = 0x2A;
					if(sdl_event.key.keysym.sym==SDLK_LEFTBRACKET) io_ports[0x60] = 0x1A;
					if(sdl_event.key.keysym.sym==SDLK_RIGHTBRACKET) io_ports[0x60] = 0x1B;
					if(sdl_event.key.keysym.sym==SDLK_MINUS) io_ports[0x60] = 0x0C;
					if(sdl_event.key.keysym.sym==SDLK_QUOTE) io_ports[0x60] = 0x28;

					if(sdl_event.key.keysym.sym==SDLK_UP) io_ports[0x60] = 0x48;
					if(sdl_event.key.keysym.sym==SDLK_LEFT) io_ports[0x60] = 0x4B;
					if(sdl_event.key.keysym.sym==SDLK_RIGHT) io_ports[0x60] = 0x4D;
					if(sdl_event.key.keysym.sym==SDLK_DOWN) io_ports[0x60] = 0x50;

					if(sdl_event.key.keysym.sym==SDLK_ESCAPE) io_ports[0x60] = 1;
					if(sdl_event.key.keysym.sym==SDLK_LCTRL) io_ports[0x60] = 0x1D;
					if(sdl_event.key.keysym.sym==SDLK_RETURN) io_ports[0x60] = 0x1C;
					if(sdl_event.key.keysym.sym==SDLK_SPACE) io_ports[0x60] = 0x39;
					if(sdl_event.key.keysym.sym==SDLK_BACKSPACE) io_ports[0x60] = 0x0E;

					if(sdl_event.key.keysym.sym==SDLK_F12) running = 0;
					io_ports[0x64] = 1;
					pc_interrupt(9);
				}
				if(sdl_event.type == SDL_KEYUP)	io_ports[0x60] = io_ports[0x60]|0x80;
				if(io_ports[0x60]==0x1d) k_ctrl = 1;
				if(io_ports[0x60]==0x1d+0x80) k_ctrl = 0;
				if(io_ports[0x60]==0x2a) k_shift = 1;
				if(io_ports[0x60]==0x2a+0x80) k_shift = 0;
			}
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	//free(screen);
	free(disk);
	return 0;
}
