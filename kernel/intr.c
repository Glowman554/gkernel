#include <stdint.h>
#include <intr.h>
#include <stdint.h>
#include <console.h>
#include <config.h>
#include "driver/keyboard/keyboard.h"
#include "driver/vga/vga.h"
#include "driver/cmos/cmos.h"
#include "driver/pit/pit.h"
#include <string.h>
#include <widget.h>
#include <fs.h>
#include <initrd.h>
#include <pci.h>
#include <tasks.h>

extern void intr_stub_0(void);
extern void intr_stub_1(void);
extern void intr_stub_2(void);
extern void intr_stub_3(void);
extern void intr_stub_4(void);
extern void intr_stub_5(void);
extern void intr_stub_6(void);
extern void intr_stub_7(void);
extern void intr_stub_8(void);
extern void intr_stub_9(void);
extern void intr_stub_10(void);
extern void intr_stub_11(void);
extern void intr_stub_12(void);
extern void intr_stub_13(void);
extern void intr_stub_14(void);
extern void intr_stub_15(void);
extern void intr_stub_16(void);
extern void intr_stub_17(void);
extern void intr_stub_18(void);

extern void intr_stub_32(void);
extern void intr_stub_33(void);

extern void intr_stub_48(void);

#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS     0x09

#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0   0x00
#define GDT_FLAG_RING3   0x60
#define GDT_FLAG_PRESENT 0x80

#define GDT_FLAG_4K      0x800
#define GDT_FLAG_32_BIT  0x400

#define GDT_ENTRIES 6
static uint64_t gdt[GDT_ENTRIES];
static uint32_t tss[32] = { 0, 0, 0x10 };

#define IDT_ENTRIES 256

char scancode_kbd[256][2] =
{
    {0,               0},         // 0x00
    {27,             27},         // 0x01
    {'1',           '|'},         // 0x02
    {'2',           '"'},         // 0x03 F5
    {'3',           '#'},         // 0x04 F3
    {'4',           '$'},         // 0x05 F1
    {'5',           '%'},         // 0x06 F2
    {'6',           '&'},         // 0x07 F12
    {'7',           '/'},         // 0x08 F10
    {'8',           '('},         // 0x09
    {'9',           ')'},         // 0x0A F8
    {'0',           '='},         // 0x0B F6
    {'/',            '?'},         // 0x0C'
    {'^',           '`'},         // 0x0D
    {'\b',         '\b'},         // 0x0E
    {'\t',         '\t'},         // 0x0F F4
    {'q',           'Q'},         // 0x10
    {'w',           'W'},         // 0x11
    {'e',           'E'},         // 0x12
    {'r',           'R'},         // 0x13
    {'t',           'T'},         // 0x14
    {'z',           'Z'},         // 0x15 q
    {'u',           'U'},         // 0x16 1
    {'i',           'I'},         // 0x17
    {'o',           'O'},         // 0x18
    {'p',           'P'},         // 0x19
    {'u',           'e'},         // 0x1A y
    {'+',           '*'},         // 0x1B s
    {'\n',         '\n'},         // 0x1C a
    {0,               0},         // 0x1D w
    {'a',           'A'},         // 0x1E 2
    {'s',           'S'},         // 0x1F
    {'d',           'D'},         // 0x20
    {'f',           'F'},         // 0x21 c
    {'g',           'G'},         // 0x22 x
    {'h',           'H'},         // 0x23 d
    {'j',           'J'},         // 0x24 e
    {'k',           'K'},         // 0x25 4
    {'l',           'L'},         // 0x26 3
    {'o',           'e'},         // 0x27
    {'a',           'a'},         // 0x28
    {0,               0},      // 0x29 space
    {0,               0},      // 0x2A v
    {'#',          '\''},      // 0x2B f
    {'y',           'Y'},      // 0x2C t
    {'x',           'X'},      // 0x2D r
    {'c',           'C'},      // 0x2E 5
    {'v',           'V'},         // 0x2F
    {'b',           'B'},         // 0x30
    {'n',           'N'},      // 0x31 n
    {'m',           'M'},      // 0x32 b
    {',',           ';'},      // 0x33 h
    {'.',           ':'},      // 0x34 g
    {'-',           '_'},      // 0x35 z
    {0,               0},      // 0x36 6
    {0,               0},         // 0x37
    {0,               0},         // 0x38
    {' ',           ' '},         // 0x39
    {0,               0},      // 0x3A m
    {0,              0},      // 0x3B j [F1]
    {0,              0},      // 0x3C u
    {0,               0},      // 0x3D 7
    {0,               0},      // 0x3E 8
    {0,               0},         // 0x3F
    {0,               0},         // 0x40
    {0,               0},      // 0x41 ,
    {0,               0},      // 0x42 k
    {0,               0},      // 0x43 i
    {0,               0},      // 0x44 o [F10]
    {0,               0},      // 0x45 0
    {0,               0},      // 0x46 9
    {0,               0},         // 0x47
    {0,               0},         // 0x48
    {0,               0},      // 0x49 .
    {0,               0},      // 0x4A -
    {0,               0},      // 0x4B l
    {0,               0},      // 0x4C TODO
    {0,               0},      // 0x4D p
    {0,               0},      // 0x4E �?
    {0,               0},         // 0x4F
    {0,               0},         // 0x50
    {0,               0},         // 0x51
    {0,               0},      // 0x52 TODO
    {0,               0},         // 0x53
    {0,               0},      // 0x54 TODO
    {0,               0},      // 0x55 
    {0,               0},         // 0x56
    {0,               0},         // 0x57 [F11]
    {0,               0},         // 0x58 capslock [F12]
    {0,               0},         // 0x59
    {0,               0},     // 0x5A enter
    {0,               0},      // 0x5B +
    {0,               0},         // 0x5C
    {0,               0},     // 0x5D #
    {0,               0},         // 0x5E
    {0,               0},         // 0x5F
    {0,               0},         // 0x60
    {0,               0},         // 0x61
    {0,               0},         // 0x62
    {0,               0},         // 0x63
    {0,               0},         // 0x64
    {0,               0},         // 0x65
    {0,               0},     // 0x66 backspace
    {0,               0},         // 0x67
    {0,               0},         // 0x68
    {0,               0},         // 0x69 numblock 1 //TODO
    {0,               0},         // 0x6A
    {0,               0},         // 0x6B numblock 6 //TODO
    {0,               0},         // 0x6C numblock 7 //TODO
    {0,               0},         // 0x6D
    {0,               0},         // 0x6E
    {0,               0},         // 0x6F
    {0,               0},         // 0x70 numblock 0 //TODO
    {0,               0},         // 0x71 numblock , //TODO
    {0,               0},         // 0x72 numblock 2 //TODO
    {0,               0},         // 0x73 numblock 5 //TODO
    {0,               0},         // 0x74 numblock 6 //TODO
    {0,               0},         // 0x75 numblock 8 //TODO
    {0,               0},         // 0x76 ESC
    {0,               0},         // 0x77 NUM
    {0,               0},         // 0x78 F11
    {0,               0},      // 0x79 numblock + //TODO
    {0,               0},         // 0x7A numblock 3 //TODO
    {0,               0},      // 0x7B numblock - //TODO
    {0,               0},      // 0x7C numblock * //TODO
    {0,               0},         // 0x7D numblock 9 //TODO
    {0,               0},         // 0x7E scroll lock
    {0,               0},         // 0x7F
    {0,               0},         // 0x80
    {0,               0},         // 0x81
    {0,               0},         // 0x82
    {0,               0},         // 0x83 F7
    {0,               0},         // 0x84
    {0,               0},         // 0x85
    {0,               0},         // 0x86
    {0,               0},         // 0x87
    {0,               0},         // 0x88
    {0,               0},         // 0x89
    {0,               0},         // 0x8A
    {0,               0},         // 0x8B
    {0,               0},         // 0x8C
    {0,               0},         // 0x8D
    {0,               0},         // 0x8E
    {0,               0},         // 0x8F
};


static long long unsigned int idt[IDT_ENTRIES];

static void gdt_set_entry(int i, unsigned int base, unsigned int limit,
    int flags)
{
    gdt[i] = limit & 0xffffLL;
    gdt[i] |= (base & 0xffffffLL) << 16;
    gdt[i] |= (flags & 0xffLL) << 40;
    gdt[i] |= ((limit >> 16) & 0xfLL) << 48;
    gdt[i] |= ((flags >> 8 )& 0xffLL) << 52;
    gdt[i] |= ((base >> 24) & 0xffLL) << 56;
}

void init_gdt(void)
{
    struct {
        uint16_t limit;
        void* pointer;
    } __attribute__((packed)) gdtp = {
        .limit = GDT_ENTRIES * 8 - 1,
        .pointer = gdt,
    };

    // GDT-Eintraege aufbauen
    gdt_set_entry(0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
        GDT_FLAG_CODESEG | GDT_FLAG_4K | GDT_FLAG_PRESENT);
    gdt_set_entry(2, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
        GDT_FLAG_DATASEG | GDT_FLAG_4K | GDT_FLAG_PRESENT);
    gdt_set_entry(3, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
        GDT_FLAG_CODESEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
    gdt_set_entry(4, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT |
        GDT_FLAG_DATASEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
    gdt_set_entry(5, (uint32_t) tss, sizeof(tss),
        GDT_FLAG_TSS | GDT_FLAG_PRESENT | GDT_FLAG_RING3);

    // GDT neu laden
    asm volatile("lgdt %0" : : "m" (gdtp));

    // Segmentregister neu laden, damit die neuen GDT-Eintraege auch wirklich
    // benutzt werden
    asm volatile(
        "mov $0x10, %ax;"
        "mov %ax, %ds;"
        "mov %ax, %es;"
        "mov %ax, %ss;"
        "ljmp $0x8, $.1;"
        ".1:"
    );

    // Taskregister neu laden
    asm volatile("ltr %%ax" : : "a" (5 << 3));
}

#define IDT_FLAG_INTERRUPT_GATE 0xe
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0 0x00
#define IDT_FLAG_RING3 0x60

static void idt_set_entry(int i, void (*fn)(), unsigned int selector,
    int flags)
{
    unsigned long int handler = (unsigned long int) fn;
    idt[i] = handler & 0xffffLL;
    idt[i] |= (selector & 0xffffLL) << 16;
    idt[i] |= (flags & 0xffLL) << 40;
    idt[i] |= ((handler>> 16) & 0xffffLL) << 48;
}

/* Schreibt ein Byte in einen I/O-Port */
//static inline void outb(uint16_t port, uint8_t data)
//{
//    asm volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
//}

static void init_pic(void)
{
    // Master-PIC initialisieren
    outb(0x20, 0x11); // Initialisierungsbefehl fuer den PIC
    outb(0x21, 0x20); // Interruptnummer fuer IRQ 0
    outb(0x21, 0x04); // An IRQ 2 haengt der Slave
    outb(0x21, 0x01); // ICW 4

    // Slave-PIC initialisieren
    outb(0xa0, 0x11); // Initialisierungsbefehl fuer den PIC
    outb(0xa1, 0x28); // Interruptnummer fuer IRQ 8
    outb(0xa1, 0x02); // An IRQ 2 haengt der Slave
    outb(0xa1, 0x01); // ICW 4

    // Alle IRQs aktivieren (demaskieren)
    outb(0x20, 0x0);
    outb(0xa0, 0x0);
}


void init_intr(void)
{
    struct {
        unsigned short int limit;
        void* pointer;
    } __attribute__((packed)) idtp = {
        .limit = IDT_ENTRIES * 8 - 1,
        .pointer = idt,
    };

    // Interruptnummern der IRQs umbiegen
    init_pic();

    // Excpetion-Handler
    idt_set_entry(0, intr_stub_0, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(1, intr_stub_1, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(2, intr_stub_2, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(3, intr_stub_3, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(4, intr_stub_4, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(5, intr_stub_5, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(6, intr_stub_6, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(7, intr_stub_7, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(8, intr_stub_8, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(9, intr_stub_9, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(10, intr_stub_10, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(11, intr_stub_11, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(12, intr_stub_12, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(13, intr_stub_13, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(14, intr_stub_14, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(15, intr_stub_15, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(16, intr_stub_16, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(17, intr_stub_17, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(18, intr_stub_18, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);

    // IRQ-Handler
    idt_set_entry(32, intr_stub_32, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_set_entry(33, intr_stub_33, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);

    // Syscall
    idt_set_entry(48, intr_stub_48, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING3 | IDT_FLAG_PRESENT);

    asm volatile("lidt %0" : : "m" (idtp));

    asm volatile("sti");
}

void ls()
{
	if (pmb_info->mbs_mods_count != 0) {
		struct multiboot_module* modules = pmb_info->mbs_mods_addr;
		int i;

		for (i = 0; i < pmb_info->mbs_mods_count; i++) {
			kprintf(0xf, "Found module %s at 0x%x\n", modules[i].cmdline, modules[i].mod_start);
		}
	}
}

struct cpu_state* syscall(struct cpu_state* cpu)
{
    /*
     * Der Aufrufer uebergibt in eax die Nummer des Syscalls. In den weiteren
     * Registern werden die Parameter uebergeben.
     */
    uint8_t in, i;
	uint8_t ret;
    char inbuff = 0;
    uint8_t good = 0x02;
    
    int h,m,s;
    struct multiboot_module* modules = pmb_info->mbs_mods_addr;
	
    char buf[] = "                                                               ";

	//kprintf(0xa, "Syscall eax %d, ebx %d, ecx %d, edx %d\n", cpu->eax, cpu->ebx ,cpu->ecx, cpu->edx);
    
    switch (cpu->eax) {
        case PUTC:
            kprintf(cpu->ecx, "%c", cpu->ebx);
            break;
        case GETCHAR:
	       	while(inbuff == 0){
	       		in = inb(0x60);
				inbuff = scancode_kbd[in][0];
			}
			//mouse right -> 77
			//mouse left -> 75
			//mouse up -> 72
			//mouse down -> 80
			
			//cpu->ebx = in;
			cpu->ebx = scancode_kbd[in][0];
			
			//kprintf(0xa, "%d\n", in);
			
			break;
		case GETMOV:
			in = inb(0x60);
			cpu->ebx = in;
			break;
		case REBOOT:
		    while (good & 0x02)
		        good = inb(0x64);
		    outb(0x64, 0xFE);
		    asm volatile ("hlt");
		    break;
		case PNUM:
            kprintf(cpu->ecx, "%d", cpu->ebx);
            break;
        case READ_H:
        	h = read_h();
        	cpu->ebx = h;
        	break;
        case READ_M:
        	m = read_m();
        	cpu->ebx = m;
        	break;
        case READ_S:
        	s = read_s();
        	cpu->ebx = s;
        	break;
        case RTICK:
        	reset_tick();
        	break;
        case GTICK:
        	cpu->ebx = get_tick();
        	break;
        case CLRSCR:
        	clrscr();
        	break;
        case SETX:
        	setx(cpu->ebx);
        	break;
        case SETY:
        	sety(cpu->ebx);
        	break;
        case SVGA:
        	init_vga();
			break;
		case GPIXEL:
			cpu->edx = getpixel(cpu->ebx, cpu->ecx);
			break;
		case SPIXEL:
			setpixel(cpu->ebx, cpu->ecx, cpu->edx);
			break;
		case LOADF:
            kprintf(0xf, "Loading file %s\n", cpu->ebx);
            for(i = 0; i < pmb_info->mbs_mods_count; i++){
                strcpy(buf, "                                                               ");
		    	strcpy(buf, modules[i].cmdline);
	    		buf[strlen(modules[i].cmdline)] = 0x0;
                //kprintf(0xf, "Found module %s at position %d with len %d\n", buf, i, strlen(modules[i].cmdline));
                if(strcmp(buf, cpu->ebx)==0){
		    		init_elf((void*) modules[i].mod_start);
		    	}
            }
			break;
		case LS:
			ls();
			break;
		case LSPCI:
			get_pci_devices();
			break;
        case KVER:
            cpu->ebx = VERSION;
            break;
		case EXIT:
			kexit(cpu->ebx);
			break;
		case KVEN:
			cpu->ebx = &VENDOR;
			break;
		case RUNK:
			(*((func_ptr)cpu->ebx))();
			break;
    }

    return cpu;
}


func_ptr interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, func_ptr handler)
{
	kprintf(0xf, "Registering interrupt handler %d at 0x%x\n", n, handler);
	interrupt_handlers[n] = handler;
}

struct cpu_state* handle_interrupt(struct cpu_state* cpu)
{
    struct cpu_state* new_cpu = cpu;
	//kprintf(0x4, "Interupt %d!\n", cpu->intr);
    if (cpu->intr <= 0x1f) {
        //create_error(cpu->intr);
		clrscr();
		kprintf(0x4, "Exception %d ", cpu->intr);
		switch(cpu->intr){
			case 0x0:
				kprintf(0x4, "Divide by Zero");
				break;
			case 0x1:
				kprintf(0x4, "Debug");
				break;
			case 0x2:
				kprintf(0x4, "Non Maskable Interrupt");
				break;
			case 0x3:
				kprintf(0x4, "Breakpoint");
				break;
			case 0x4:
				kprintf(0x4, "Overflow");
				break;
			case 0x5:
				kprintf(0x4, "Bound Range");
				break;
			case 0x6:
				kprintf(0x4, "Invalid Opcode");
				break;
			case 0x7:
				kprintf(0x4, "Device Not Available");
				break;
			case 0x8:
				kprintf(0x4, "Double Fault");
				break;
			case 0x9:
				kprintf(0x4, "Coprocessor Segment Overrun");
				break;
			case 0xa:
				kprintf(0x4, "Invalid TSS");
				break;
			case 0xb:
				kprintf(0x4, "Segment not Present");
				break;
			case 0xc:
				kprintf(0x4, "Stack Fault");
				break;
			case 0xd:
				kprintf(0x4, "General Protection");
				break;
			case 0xe:
				kprintf(0x4, "Page Fault");
				break;
			case 0x10:
				kprintf(0x4, "x87 Floating Point");
				break;
			case 0x11:
				kprintf(0x4, "Alignment Check");
				break;
			case 0x12:
				kprintf(0x4, "Machine Check");
				break;
			case 0x13:
				kprintf(0x4, "SIMD Floating Point");
				break;
			case 0x1e:
				kprintf(0x4, "Security-sensitive event in Host");
				break;
			default:
				kprintf(0x4, "Reserved");
				break;
		}
		kprintf(0xf, "\n");
		kprintf(0x4, "Kernel Version %d\n", VERSION);
        // TODO Hier den CPU-Zustand ausgeben
		
		kprintf(0x4, "eax: %d, ebx: %d, ecx: %d, edx: %d\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
		kprintf(0x4, "esi: %d, edi: %d, ebp: %d\n", cpu->esi, cpu->edi, cpu->ebp);
		kprintf(0x4, "intr: %d, error: %d\n", cpu->intr, cpu->error);
		kprintf(0x4, "eip: %d, cs %d, eflags: %d\n", cpu->eip, cpu->cs, cpu->eflags);
		kprintf(0x4, "esp: %d, ss: %d\n", cpu->esp, cpu->ss);

	    while(1) {
	        // Prozessor anhalten
	        asm volatile("cli; hlt");
	    }
    } else if (cpu->intr >= 0x20 && cpu->intr <= 0x2f) {

        if (cpu->intr == 0x20) {
            new_cpu = schedule(cpu);
            tss[1] = (uint32_t) (new_cpu + 1);
        }
		
		if (interrupt_handlers[cpu->intr] != 0)
		{
			(*(interrupt_handlers[cpu->intr]))();
		}
		
        /*if(cpu->intr == 32){
        	pit_irq_handler(cpu->intr);
		}
		if(cpu->intr == 33){ 
            keyboard_irq_handler(cpu->intr);
            //kprintf(0x4, "Interupt %d!\n", cpu->intr);
		}*/
        if (cpu->intr >= 0x28) {
            // EOI an Slave-PIC
            outb(0xa0, 0x20);
        }
    	outb(0x20, 0x20);
        // EOI an Master-PIC
        outb(0x20, 0x20);
    } else if (cpu->intr == 0x30) {
        new_cpu = syscall(cpu);
    } else {
        kprintf(0x4, "Unbekannter Interrupt\n");
        while(1) {
            // Prozessor anhalten
            asm volatile("cli; hlt");
        }
    }

    return new_cpu;
}
