#include <console.h>	
#include <pci.h>
#include <io.h>
#include <stdint.h> 

#define PCI_CONFIG_DATA    0x0CFC
#define PCI_CONFIG_ADDRESS 0x0CF8
 
/* lie�t einen 32Bit-Wert aus dem Config-Space vom PCI-Ger�t (addr) an Offset (offset) und gibt ihn zur�ck */
uint32_t pci_config_readd(pci_bdf_t addr,uint8_t offset);
uint32_t pci_config_read_32(pci_bdf_t addr,uint8_t offset);
uint16_t pci_config_read_16(pci_bdf_t addr,uint8_t offset);
uint8_t pci_config_read_8(pci_bdf_t addr,uint8_t offset);

uint32_t pci_config_readd(pci_bdf_t addr,uint8_t offset) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  uint32_t val;
  int address = 0x80000000|(bus<<16)|(dev<<11)|(func<<8)|(offset&0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  val = inl(PCI_CONFIG_DATA + (offset & 0x3));
  return val;
}
 
uint32_t pci_config_read_32(pci_bdf_t addr,uint8_t offset) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  uint32_t val;
  int address = 0x80000000 | (bus << 16) | (dev << 11) | (func << 8) | (offset & 0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  val = inl(PCI_CONFIG_DATA + (offset & 0x3));
  return val;
}
 
uint16_t pci_config_read_16(pci_bdf_t addr,uint8_t offset) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  uint16_t val;
  int address = 0x80000000 | (bus << 16) | (dev << 11) | (func << 8) | (offset & 0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  val = inw(PCI_CONFIG_DATA + (offset & 0x3));
  return val;
}
 
uint8_t pci_config_read_8(pci_bdf_t addr,uint8_t offset) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  uint8_t val;
  int address = 0x80000000 | (bus << 16) | (dev << 11) | (func << 8) | (offset & 0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  val = inb(PCI_CONFIG_DATA + (offset & 0x3));
  return val;
}

/* schreibt einen 32Bit-Wert (value) in den Config-Space vom PCI-Ger�t (addr) an Offset (offset) */
void pci_config_writed(pci_bdf_t addr,uint8_t offset,uint32_t value);
void pci_config_write_32(pci_bdf_t addr,uint8_t offset,uint32_t value);
void pci_config_write_16(pci_bdf_t addr,uint8_t offset,uint16_t value);
void pci_config_write_8(pci_bdf_t addr,uint8_t offset,uint8_t value);

void pci_config_writed(pci_bdf_t addr,uint8_t offset,uint32_t value) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  int address = 0x80000000|(bus<<16)|(dev<<11)|(func<<8)|(offset&0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  outl(PCI_CONFIG_DATA + (offset & 0x3),value);
}

void pci_config_write_32(pci_bdf_t addr,uint8_t offset,uint32_t value) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  int address = 0x80000000|(bus<<16)|(dev<<11)|(func<<8)|(offset&0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  outl(PCI_CONFIG_DATA + (offset & 0x3),value);
}

void pci_config_write_16(pci_bdf_t addr,uint8_t offset,uint16_t value) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  int address = 0x80000000|(bus<<16)|(dev<<11)|(func<<8)|(offset&0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  outw(PCI_CONFIG_DATA + (offset & 0x3),value);
}

void pci_config_write_8(pci_bdf_t addr,uint8_t offset,uint8_t value) {
  int bus=addr.bus;
  int dev=addr.dev;
  int func=addr.func;
  int address = 0x80000000|(bus<<16)|(dev<<11)|(func<<8)|(offset&0xFC);
  outl(PCI_CONFIG_ADDRESS,address);
  outb(PCI_CONFIG_DATA + (offset & 0x3),value);
}

/* sucht das unterste gesetzte Bit in einem 32Bit-Wert , value darf nicht 0 sein */
uint8_t get_number_of_lowest_set_bit(uint32_t value)
{
  uint8_t pos = 0;
  uint32_t mask = 0x00000001;
  while (!(value & mask))
   { ++pos; mask=mask<<1; }
  return pos;
}
 
/* sucht das oberste gesetzte Bit in einem 32Bit-Wert , value darf nicht 0 sein */
uint8_t get_number_of_highest_set_bit(uint32_t value)
{
  uint8_t pos = 31;
  uint32_t mask = 0x80000000;
  while (!(value & mask))
   { --pos; mask=mask>>1; }
  return pos;
}
 
 
/* analysiert die BARs eines bestimmten PCI-Ger�tes, das PCI-Ger�t sollte hierf�r unbenutzt sein */
void pci_config_bar_analyze(const pci_bdf_t addr)
{
  // Header-Type auslesen um zu ermitteln wie viele BARs vorhanden sind :
  const uint32_t headerType = ( pci_config_readd(addr,0x00C) >> 16 ) & 0x7F;
  // es werden nur Type 0x00 (normale Devices) und 0x01 (PCI-to-PCI-Bridges) unterst�tzt :
  if (headerType >= 0x02) {
	  //kprintf("FEHLER : nicht unterst�tzter Header-Type gefunden!\n");
	  return;
  }
 
  // 6 BARs f�r Type 0x00 und 2 BARs f�r Type 0x01 :
  const uint8_t max_bars = 6 - (headerType * 4);
  uint8_t bar;
  for (bar = 0 ; bar < max_bars ; ++bar)
   {
     // Offset des aktuellen BAR ermitteln :
     const uint8_t barOffset = 0x010+(bar*4);
 
     // pr�fen ob Speicher oder IO :
     if ( (pci_config_readd(addr,barOffset) & 0x1) == 0 )
      { // Speicher-Ressource :
 
        // Prefetchable-Bit auslesen und passenden Text ausw�hlen :
        static const char* ptext_pref = "prefetchable"; // Text f�r prefetchable
        static const char* ptext_nopr = "non-prefetchable"; // Text f�r non-prefetchable
        const char* const ptext = (((pci_config_readd(addr,barOffset) >> 3) & 0x1) == 1) ? ptext_pref : ptext_nopr;
 
        // check Memory-BAR-Type :
        switch((pci_config_readd(addr,barOffset) >> 1) & 0x3)
         {
           case 0x0: // 32Bit Memory-BAR :
             {
             pci_config_writed(addr,barOffset,0xFFFFFFF0); // mit lauter 1en �berschreiben
             const uint32_t barValue = pci_config_readd(addr,barOffset) & 0xFFFFFFF0; // und wieder zur�cklesen
             if (barValue == 0) // es muss mindestens ein Adressbit 1 (also beschreibbar) sein
              {
                if (ptext != ptext_nopr) // unbenutzte BARs m�ssen komplett 0 sein (auch das Prefetchable-Bit darf nicht gesetzt sein)
                 { kprintf(0x4,"FEHLER : 32Bit-Memory-BAR %u enthaelt keine beschreibbaren Adressbits!\n",bar); return; }
 
                // BAR-Infos ausgeben :
                kprintf(0xf, "BAR %u ist unbenutzt.\n",bar);
              }
             else
              {
                const uint8_t lowestBit = get_number_of_lowest_set_bit(barValue);
                // es muss eine g�ltige 32Bit-Adresse sein :
                if ( (get_number_of_highest_set_bit(barValue) != 31) || (lowestBit > 31) || (lowestBit < 4) )
                 { kprintf(0x4, "FEHLER : 32Bit-Memory-BAR %u enthaelt ungueltige beschreibbare Adressbits!\n",bar); return; }
 
                // BAR-Infos ausgeben :
                kprintf(0xf, "BAR %u enhaelt eine %s 32Bit-Memory-Ressource mit einer Groesse von 2^%u Bytes.\n",bar,ptext,lowestBit);
              }
             }
             break;
 
           case 0x1: // 20Bit Memory-BAR :
             {
             if (headerType == 0x01)
              { kprintf(0x4, "FEHLER : 20Bit-Memory-BAR %u ist fuer eine Bridge nicht erlaubt!\n",bar); return; }
 
             pci_config_writed(addr,barOffset,0xFFFFFFF0); // mit lauter 1en �berschreiben
             const uint32_t barValue = pci_config_readd(addr,barOffset) & 0xFFFFFFF0; // und wieder zur�cklesen
             if (barValue == 0) // es muss mindestens ein Adressbit 1 (also beschreibbar) sein
              { kprintf(0x4, "FEHLER : 20Bit-Memory-BAR %u enthaelt keine beschreibbaren Adressbits!\n",bar); return; }
 
             const uint8_t lowestBit = get_number_of_lowest_set_bit(barValue);
             // es muss eine g�ltige 20Bit-Adresse sein :
             if ( (get_number_of_highest_set_bit(barValue) != 19) || (lowestBit > 19) || (lowestBit < 4) )
              { kprintf(0x4, "FEHLER : 20Bit-Memory-BAR %u enthaelt ungueltige beschreibbare Adressbits!\n",bar); return; }
 
             // BAR-Infos ausgeben :
             kprintf(0xf, "BAR %u enhaelt eine %s 20Bit-Memory-Ressource mit einer Groesse von 2^%u Bytes.\n",bar,ptext,lowestBit);
             }
             break;
 
           case 0x2: // 64Bit Memory-BAR :
             {
             // pr�fen ob ein 64Bit-BAR an der aktuellen Position �berhaupt m�glich ist :
             if (bar >= (max_bars-1))
              { kprintf(0x4, "FEHLER : 64Bit-Memory-BAR %u darf nicht an letzter Position beginnen!\n",bar); return; }
             // non-prefetchable 64-BARs k�nnen nicht hinter Bridges benutzt werden (? aber in der Spec sind sie nicht verboten ?) :
             if (ptext != &ptext_pref[0])
              { kprintf(0x4, "FEHLER : 64Bit-Memory-BAR %u enthaelt eine non-prefetchable Memory-Ressource!\n",bar); return; }
 
             pci_config_writed(addr,barOffset,0xFFFFFFF0); // mit lauter 1en �berschreiben
             pci_config_writed(addr,barOffset+4,0xFFFFFFFF); // mit lauter 1en �berschreiben
             const uint32_t barLowValue  = pci_config_readd(addr,barOffset) & 0xFFFFFFF0; // und wieder zur�cklesen
             const uint32_t barHighValue = pci_config_readd(addr,barOffset+4); // und wieder zur�cklesen
 
             uint8_t lowestBit = 0;
             if (barLowValue != 0)
              { // kleiner als 4 GByte :
                lowestBit = get_number_of_lowest_set_bit(barLowValue);
                // es muss eine g�ltige kleine 64Bit-Adresse sein :
                if ( (barHighValue != 0xFFFFFFFF) || (get_number_of_highest_set_bit(barLowValue) != 31) || (lowestBit > 31) || (lowestBit < 4) )
                 { kprintf(0x4, "FEHLER : 64Bit-Memory-BAR %u enthaelt ungueltige beschreibbare Adressbits im unteren Adressteil!\n",bar); return; }
              }
             else
              { // gr��er/gleich als 4 GByte :
                lowestBit = get_number_of_lowest_set_bit(barHighValue) + 32;
                // es muss eine g�ltige gro�e 64Bit-Adresse sein :
                if ( (get_number_of_highest_set_bit(barHighValue) != 31) || (lowestBit > 63) || (lowestBit < 32) )
                 { kprintf(0x4, "FEHLER : 64Bit-Memory-BAR %u enthaelt ungueltige beschreibbare Adressbits im oberen Adressteil!\n",bar); return; }
              }
 
             // BAR-Infos ausgeben :
             kprintf(0xf, "BAR %u enhaelt eine %s 64Bit-Memory-Ressource mit einer Groesse von 2^%u Bytes.\n",bar,ptext,lowestBit);
 
             // den nachfolgenden BAR f�r die Analyse �berspringen :
             ++bar;
 
             // BAR-Infos ausgeben :
             kprintf(0xf, "BAR %u ist nicht nutzbar da er die oberen 32 Bits des vorrangegangenen 64Bit-BARs enthaelt\n",bar);
             }
             break;
 
           default: // ung�ltiger Memory-BAR :
             kprintf(0x4, "FEHLER : Memory-BAR %u ist ungueltig!\n",bar); return;
         }
      }
     else
      { // IO-Ressource :
        pci_config_writed(addr,barOffset,0xFFFFFFFC); // mit lauter 1en �berschreiben
        const uint32_t barValue = pci_config_readd(addr,barOffset) & 0xFFFFFFFC; // und wieder zur�cklesen
        if (barValue == 0) // es muss mindestens ein Adressbit 1 (also beschreibbar) sein
         { kprintf(0x4, "FEHLER : IO-BAR %u enthaelt keine beschreibbaren Adressbits!\n",bar); return; }
 
        const uint8_t lowestBit  = get_number_of_lowest_set_bit(barValue);
        const uint8_t highestBit = get_number_of_highest_set_bit(barValue);
        // es muss entwerder eine g�ltige 32Bit-Adresse oder eine g�ltige 16Bit-Adresse sein :
        if ( ( (highestBit != 31) && (highestBit != 15) ) || (highestBit < lowestBit) || (lowestBit < 2) )
         { kprintf(0x4, "FEHLER : IO-BAR %u enthaelt ungueltige beschreibbare Adressbits!\n",bar); return; }
 
        // BAR-Infos ausgeben :
        kprintf(0xf, "BAR %u enhaelt eine %uBit-IO-Ressource mit einer Groesse von 2^%u Bytes.\n",bar,highestBit+1,lowestBit);
      }
   }
  if (bar != max_bars)
   { kprintf(0x4, "interner Fehler in Schleife!\n"); return; }
}

void get_pci_devices(void);
void class_to_text(pci_device dev);
pci_device get_pci_device(pci_bdf_t addr);

void get_pci_devices(void) {
	kprintf(0xf, "Reading PCI\n");
	pci_bdf_t pci;
	for(int i=0;i<256;i++) {
		for(int j=0;j<32;j++) {
			for(int k=0;k<8;k++) {
				pci.bus=i;
				pci.dev=j;
				pci.func=k;
				uint32_t res=pci_config_readd(pci,0);
				uint16_t dev=(uint16_t)(res >> 16);
				uint16_t ven=(uint16_t)(res & 0x0000FFFF);
				if(ven!=0xffff) {
					kprintf(0xf, "Geraet gefunden: Bus: %d Dev: %d Func: %d Vendor: 0x%x Device: 0x%x\n",i,j,k,ven,dev);
					class_to_text(get_pci_device(pci));
				}
			}
		}
	}	
}

pci_bdf_t search_pci_device(uint16_t vendor_id, uint16_t device_id) {
	pci_bdf_t pci;
	for(int i=0;i<256;i++) {
		for(int j=0;j<32;j++) {
			for(int k=0;k<8;k++) {
				pci.bus=i;
				pci.dev=j;
				pci.func=k;
				uint32_t res=pci_config_readd(pci,0);
				uint16_t dev=(uint16_t)(res >> 16);
				uint16_t ven=(uint16_t)(res & 0x0000FFFF);
				if(dev == device_id && ven == vendor_id) {
					return pci;
				}
			}
		}
	}
	pci.bus = -1;
	pci.dev = -1;
	pci.func = -1;
	return pci;
}

pci_device get_pci_device(pci_bdf_t addr) {
	pci_device pci;
	
	uint32_t res=pci_config_readd(addr,0x000);
	pci.address=addr;
	pci.device_id=(uint16_t)(res >> 16); //High
	pci.vendor_id=(uint16_t)(res & 0x0000FFFF); //Low
	
	res=pci_config_readd(addr,0x004);	
	pci.status=(uint16_t)(res >> 16);
	pci.command=(uint16_t)(res & 0x0000FFFF);
	
	res=pci_config_readd(addr,0x008);
	pci.class_high=(uint8_t)(res >> 24);
	pci.class_middle=(uint8_t)(res >> 16);
	pci.prog_if=(uint8_t)(res >> 8);
	pci.revision=(uint8_t)(res & 0x000000FF);
	
	res=pci_config_readd(addr,0x00C);
	pci.BIST=(uint8_t)(res >> 24);
	pci.header_type=(uint8_t)(res >> 16);
	pci.latency_timer=(uint8_t)(res >> 8);
	pci.cache_line_size=(uint8_t)(res & 0x000000FF);
	
	res=pci_config_readd(addr,0x034);
	pci.capabilities=(uint8_t)(res & 0x000000FF);
	
	res=pci_config_readd(addr,0x03C);
	pci.interrupt_pin=(uint8_t)(res >> 8);
	pci.interrupt_line=(uint8_t)(res & 0x000000FF);
	
	return pci;
}

void print_pci_info(pci_bdf_t addr) {
	pci_device dev=get_pci_device(addr);
	kprintf(0xf, "     Device-ID: %x\n",dev.device_id);
	kprintf(0xf, "     Vendor-ID: %x\n",dev.vendor_id);
	kprintf(0xf, "     Status: %b\n",dev.status);
	kprintf(0xf, "     Command: %b\n",dev.command);
	kprintf(0xf, "     Base-Class: %x\n",dev.class_high);
	kprintf(0xf, "     Sub-Class: %x\n",dev.class_middle);
	kprintf(0xf, "     Prog-Interface: %x\n",dev.prog_if);
	kprintf(0xf, "     Revision: %x\n",dev.revision);
	kprintf(0xf, "     BIST: %x\n",dev.BIST);
	kprintf(0xf, "     Header-Type: %x\n",dev.header_type);
	kprintf(0xf, "     Latency-Timer: %x\n",dev.latency_timer);
	kprintf(0xf, "     Cache-Line-Size: %x\n",dev.cache_line_size);
	kprintf(0xf, "     Capabilities: %x\n",dev.capabilities);
	kprintf(0xf, "     IRQ-Pin: %x\n",dev.interrupt_pin);
	kprintf(0xf, "     IRQ-Line: %x\n",dev.interrupt_line);
	pci_config_bar_analyze(addr);	
}

void class_to_text(pci_device dev) {
	switch(dev.class_high) {
		case 0x0:
			kprintf(0x4, "Klasse nicht vorhanden: ");
			if(dev.class_middle==0x00) kprintf(0xf, "Kein VGA-Geraet");
			if(dev.class_middle==0x01) kprintf(0xf, "VGA-Geraet");
			kprintf(0xf, "\n");
			break;
		case 0x01:
			kprintf(0xf, "Massenspeicher: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "SCSI");
					break;
				case 0x01:
					kprintf(0xf, "IDE");
					break;
				case 0x02:
					kprintf(0xf, "Floppy");
					break;
				case 0x03:
					kprintf(0xf, "IPI");
					break;
				case 0x04:
					kprintf(0xf, "RAID");
					break;
				case 0x05:
					kprintf(0xf, "ATA-Controller");
					if(dev.prog_if==0x20) kprintf(0xf, " (Single DMA)");
					if(dev.prog_if==0x20) kprintf(0xf, " (Chained DMA)");
					break;
				case 0x06:
					kprintf(0xf, "SATA-Controller");
					if(dev.prog_if==0x0) kprintf(0xf, " (Herstellerspezifische Schnittstelle)");
					if(dev.prog_if==0x01) kprintf(0xf, " (AHCI 1.0)");
					break;
				case 0x07:
					kprintf(0xf, "SAS-Controller");
					break;
				case 0x08:
					kprintf(0xf, "Non-Volatile-Memory");
					if(dev.prog_if==0x0) kprintf(0xf, " (Allgemein)");
					if(dev.prog_if==0x01) kprintf(0xf, " NVMHCI (NVM(-Express) Host-Controller)");
					if(dev.prog_if==0x02) kprintf(0xf, " Enterprise-NVMHCI (NVM(-Express) Host-Controller)");
					break;
				case 0x80:
					kprintf(0xf, "Anderes (unbekannt)");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x02:
			kprintf(0xf, "Netzwerk: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Ethernet");
					break;
				case 0x01:
					kprintf(0xf, "Token Ring");
					break;
				case 0x02:
					kprintf(0xf, "FDDI");
					break;
				case 0x03:
					kprintf(0xf, "ATM");
					break;
				case 0x04:
					kprintf(0xf, "ISDN");
					break;
				case 0x05:
					kprintf(0xf, "World FIP (Feldbus)");
					break;
				case 0x06:
					kprintf(0xf, "PICMG 2.14, Multi-Computing");
					break;
				case 0x80:
					kprintf(0xf, "Anderer");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x03:
			kprintf(0xf, "Bildschirmcontroller: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "0x0: ");
					if(dev.prog_if==0x0) kprintf(0xf, "VGA");
					if(dev.prog_if==0x01) kprintf(0xf, "8514");
					break;
				case 0x01:
					kprintf(0xf, "XGA");
					break;
				case 0x02:
					kprintf(0xf, "3D");
					break;
				case 0x80:
					kprintf(0xf, "Anderer");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x04:
			kprintf(0xf, "Multimedia: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Video");
					break;
				case 0x01:
					kprintf(0xf, "Audio");
					break;
				case 0x02:
					kprintf(0xf, "Telefonie");
					break;
				case 0x03:
					kprintf(0xf, "HD-Audio");
					break;
				case 0x80:
					kprintf(0xf, "Anderer");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x05:
			kprintf(0xf, "Speicher-Controller: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "RAM");
					break;
				case 0x01:
					kprintf(0xf, "Flash-Speicher");
					break;
				case 0x80:
					kprintf(0xf, "Anderer");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x06:
			kprintf(0xf, "Bridge: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Host/PCI");
					break;
				case 0x01:
					kprintf(0xf, "PCI/ISA");
					break;
				case 0x02:
					kprintf(0xf, "PCI/EISA");
					break;
				case 0x03:
					kprintf(0xf, "PCI/MicroChannel");
					break;
				case 0x04:
					kprintf(0xf, "PCI/PCI");
					if(dev.prog_if==0x01) kprintf(0xf, " (Subtractive Decode)");
					break;
				case 0x05:
					kprintf(0xf, "PCI/PCMCIA");
					break;
				case 0x06:
					kprintf(0xf, "PCI/NuBus");
					break;
				case 0x07:
					kprintf(0xf, "PCI/CardBus");
					break;
				case 0x08:
					kprintf(0xf, "Raceway, Switched Fabric");
					if(dev.prog_if==0x0) kprintf(0xf, " (Allgemein)");
					if(dev.prog_if==0x01) kprintf(0xf, " NVMHCI (NVM(-Express) Host-Controller)");
					if(dev.prog_if==0x02) kprintf(0xf, " Enterprise-NVMHCI (NVM(-Express) Host-Controller)");
					break;
				case 0x09:
					kprintf(0xf, "Semitransparent PCI/PCI");
					if(dev.prog_if==0x40) kprintf(0xf, " (primaer zu Host)");
					if(dev.prog_if==0x80) kprintf(0xf, " (sekundaer zu Host)");
					break;
				case 0x0A:
					kprintf(0xf, "InfiniBand/PCI");
					break;
				case 0x80:
					kprintf(0xf, "Anderes (unbekannt)");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x07:
			kprintf(0xf, "einfache Kommunikation: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Serielle Schnittstelle");
					if(dev.prog_if==0x0) kprintf(0xf, " (XT-kompatibel)");
					if(dev.prog_if==0x01) kprintf(0xf, " (16450)");
					if(dev.prog_if==0x02) kprintf(0xf, " (16550)");
					if(dev.prog_if==0x03) kprintf(0xf, " (16650)");
					if(dev.prog_if==0x04) kprintf(0xf, " (16750)");
					if(dev.prog_if==0x05) kprintf(0xf, " (16850)");
					if(dev.prog_if==0x06) kprintf(0xf, " (16950)");
					break;
				case 0x01:
					kprintf(0xf, "Parallelport");
					if(dev.prog_if==0x0) kprintf(0xf, " (Standard)");
					if(dev.prog_if==0x01) kprintf(0xf, " (Bidirektional)");
					if(dev.prog_if==0x02) kprintf(0xf, " (ECP 1.0)");
					if(dev.prog_if==0x03) kprintf(0xf, " (IEEE 1284)");
					if(dev.prog_if==0xFE) kprintf(0xf, " (IEEE-1284-Geraet, Target)");
					break;
				case 0x02:
					kprintf(0xf, "Multiport Serial Controller");
					break;
				case 0x03:
					kprintf(0xf, "0x03: ");
					if(dev.prog_if==0x0) kprintf(0xf, "Standard-Modem");
					if(dev.prog_if==0x01) kprintf(0xf, "Hayes-kompatibles Modem (16450)");
					break;
				case 0x04:
					kprintf(0xf, "0x04");
					if(dev.prog_if==0x0) kprintf(0xf, "GPIB (IEEE488.1/2) Controller");
					if(dev.prog_if==0x02) kprintf(0xf, "Hayes-kompatibles Modem (16550)");
					if(dev.prog_if==0x03) kprintf(0xf, "Hayes-kompatibles Modem (16650)");
					if(dev.prog_if==0x04) kprintf(0xf, "Hayes-kompatibles Modem (16750)");
					break;
				case 0x05:
					kprintf(0xf, "SmartCard");
					break;
				case 0x80:
					kprintf(0xf, "Anderes (unbekannt)");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x08:
			kprintf(0xf, "System-Peripherie: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "0x0: ");
					if(dev.prog_if==0x0) kprintf(0xf, "8259- PIC");
					if(dev.prog_if==0x01) kprintf(0xf, "ISA-PIC");
					if(dev.prog_if==0x02) kprintf(0xf, "EISA-PIC");
					if(dev.prog_if==0x10) kprintf(0xf, "IO- APIC");
					if(dev.prog_if==0x20) kprintf(0xf, "IOx-APIC");
					break;
				case 0x01:
					kprintf(0xf, "0x01: ");
					if(dev.prog_if==0x0) kprintf(0xf, "8237-DMA-Controller");
					if(dev.prog_if==0x01) kprintf(0xf, "ISA-DMA-Controller");
					if(dev.prog_if==0x02) kprintf(0xf, "EISA-DMA-Controller");
					break;
				case 0x02:
					kprintf(0xf, "0x02: ");
					if(dev.prog_if==0x0) kprintf(0xf, "Standard-8254-Timer");
					if(dev.prog_if==0x01) kprintf(0xf, "ISA-System-Timer");
					if(dev.prog_if==0x02) kprintf(0xf, "EISA-System-Timer");
					if(dev.prog_if==0x03) kprintf(0xf, "High Performance Event Timer");
					break;
				case 0x03:
					kprintf(0xf, "0x03: ");
					if(dev.prog_if==0x0) kprintf(0xf, "Standard Real Time Clock");
					if(dev.prog_if==0x01) kprintf(0xf, "ISA Real Time Clock");
					break;
				case 0x04:
					kprintf(0xf, "generic PCI Hot Plug Controller");
					break;
				case 0x05:
					kprintf(0xf, "SD Host Controller");
					break;
				case 0x06:
					kprintf(0xf, "IOMMU");
					break;
				case 0x07:
					kprintf(0xf, "Root Complex Event Collector");
					break;
				case 0x80:
					kprintf(0xf, "Anderes");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x09:
			kprintf(0xf, "Input-Geraet: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Tastatur");
					break;
				case 0x01:
					kprintf(0xf, "Digitizer (Stift)");
					break;
				case 0x02:
					kprintf(0xf, "Maus");
					break;
				case 0x03:
					kprintf(0xf, "Scanner");
					break;
				case 0x04:
					kprintf(0xf, "0x04: ");
					if(dev.prog_if==0x0) kprintf(0xf, "Standard-Gameport");
					if(dev.prog_if==0x01) kprintf(0xf, "Gameport");
					break;
				case 0x80:
					kprintf(0xf, "Anderes");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x0A:
			kprintf(0xf, "Docking-Stationen: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Normale Docking-Station");
					break;
				case 0x80:
					kprintf(0xf, "Andere");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x0B:
			kprintf(0xf, "Prozessor: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "386");
					break;
				case 0x01:
					kprintf(0xf, "486");
					break;
				case 0x02:
					kprintf(0xf, "Pentium");
					break;
				case 0x10:
					kprintf(0xf, "Alpha");
					break;
				case 0x20:
					kprintf(0xf, "PowerPC");
					break;
				case 0x30:
					kprintf(0xf, "Mips");
					break;
				case 0x40:
					kprintf(0xf, "Coprozessor");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x0C:
			kprintf(0xf, "Serielle Buscontroller: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Firewire");
					if(dev.prog_if==0x0) kprintf(0xf, " (IEEE 1394)");
					if(dev.prog_if==0x10) kprintf(0xf, " (OHCI)");
					break;
				case 0x01:
					kprintf(0xf, "ACCESS-Bus");
					break;
				case 0x02:
					kprintf(0xf, "SSA (Serial Storage Architecture)");
					break;
				case 0x03:
					kprintf(0xf, "USB-Host ");
					if(dev.prog_if==0x0) kprintf(0xf, "(UHCI)");
					if(dev.prog_if==0x10) kprintf(0xf, "(OHCI)");
					if(dev.prog_if==0x20) kprintf(0xf, "(EHCI)");
					if(dev.prog_if==0x30) kprintf(0xf, "(xHCI)");
					if(dev.prog_if==0x80) kprintf(0xf, "(kein HCI)");
					if(dev.prog_if==0xFE) kprintf(0xf, "(USB-Device)");
					break;
				case 0x04:
					kprintf(0xf, "Fibre Channel");
					break;
				case 0x05:
					kprintf(0xf, "SMB (System Management Bus)");
					break;
				case 0x06:
					kprintf(0xf, "InfiniBand");
					break;
				case 0x07:
					kprintf(0xf, "IPMI ");
					if(dev.prog_if==0x0) kprintf(0xf, "SMIC Interface");
					if(dev.prog_if==0x01) kprintf(0xf, "Keyboard Style Interface");
					if(dev.prog_if==0x02) kprintf(0xf, "Block Transfer Device");
					break;
				case 0x08:
					kprintf(0xf, "SERCOS Interface");
					break;
				case 0x09:
					kprintf(0xf, "CAN-Bus");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x0D:
			kprintf(0xf, "Wireless-Controller: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "iRDA");
					break;
				case 0x01:
					kprintf(0xf, "Consumer Infrared");
					break;
				case 0x10:
					kprintf(0xf, "Radio Frequency");
					break;
				case 0x11:
					kprintf(0xf, "Bluetooth");
					break;
				case 0x12:
					kprintf(0xf, "Broadband");
					break;
				case 0x80:
					kprintf(0xf, "Anderer");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x0E:
			kprintf(0xf, "Intelligente Controller: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "0x0");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x0F:
			kprintf(0xf, "Satelliten-Kommunikation: ");
			switch(dev.class_middle) {
				case 0x01:
					kprintf(0xf, "TV");
					break;
				case 0x02:
					kprintf(0xf, "Audio");
					break;
				case 0x03:
					kprintf(0xf, "Sprache");
					break;
				case 0x04:
					kprintf(0xf, "Daten");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x10:
			kprintf(0xf, "Encryption/Decryption: ");
			switch(dev.class_middle) {
				case 0x01:
					kprintf(0xf, "Netzwerk");
					break;
				case 0x10:
					kprintf(0xf, "Entertainment");
					break;
				case 0x80:
					kprintf(0xf, "Anderer");
					break;
			}
			kprintf(0xf, "\n");
			break;
		case 0x11:
			kprintf(0xf, "Datenerfassung/Signalprozessor: ");
			switch(dev.class_middle) {
				case 0x0:
					kprintf(0xf, "Digital Input/Output");
					break;
				case 0x01:
					kprintf(0xf, "Zaehler");
					break;
				case 0x10:
					kprintf(0xf, "Synchronisation und Frequenzmessung");
					break;
				case 0x20:
					kprintf(0xf, "Management Card");
					break;
				case 0x80:
					kprintf(0xf, "Anderer");
					break;
			}
			kprintf(0xf, "\n");
			break;
		default:
			kprintf(0xf, "Unbekannt\n");
			break;
	}
}

uint8_t pci_read_register_8(pci_bdf_t addr, uint32_t barOffset, uint32_t offset) {
	pci_read_register(addr, barOffset, offset);
}
uint8_t pci_read_register(pci_bdf_t addr, uint32_t barOffset, uint32_t offset) {
	uint8_t res;
	barOffset=(0x010+(barOffset*4));
	uint32_t pci_read=pci_config_readd(addr,barOffset);
	uint32_t pci_backup=pci_config_readd(addr,barOffset);
	if((pci_read & 0x1) == 0 && (pci_read >> 1 & 0x3) == 0) { //32-Bit-Memory-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFF0);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFF0;
		const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
		if(pci_read!=0 && !( (get_number_of_highest_set_bit(pci_read) != 31) || (lowestBit > 31) || (lowestBit < 4) )) {
			pci_config_writed(addr,barOffset,pci_backup);
			pci_read=pci_config_readd(addr,barOffset);
			//kprintf("32-Bit-Memory-Ressource\n");
			uint8_t *address=(uint8_t*)((uint32_t)(pci_read & 0xFFFFFFF0));
			//kprintf("Offset 0x%x: %b - 0x%x\n",offset,address[offset],address[offset]);
			res=address[offset];
		}
	} else { //IO-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFFC);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFFC;
		if((pci_read) != 0) {
			const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
			const uint8_t highestBit = get_number_of_highest_set_bit(pci_read);
			if (!( ( (highestBit != 31) && (highestBit != 15) ) || (highestBit < lowestBit) || (lowestBit < 2) )) {
				pci_config_writed(addr,barOffset,pci_backup);
				pci_read=pci_config_readd(addr,barOffset);
				//kprintf("IO-Ressource\n");
				uint32_t address1=(uint32_t)(pci_read & 0xFFFFFFFC);
				//kprintf("Offset 0x%x: %b - 0x%x\n",offset,inb(address1 + offset),inb(address1 + offset));
				res=inb(address1 + offset);
			}
		}
	}
	pci_config_writed(addr,barOffset,pci_backup);
	return res;
}

uint16_t pci_read_register_16(pci_bdf_t addr, uint32_t barOffset, uint32_t offset) {
	uint16_t res;
	barOffset=(0x010+(barOffset*4));
	uint32_t pci_read=pci_config_readd(addr,barOffset);
	uint32_t pci_backup=pci_config_readd(addr,barOffset);
	if((pci_read & 0x1) == 0 && (pci_read >> 1 & 0x3) == 0) { //32-Bit-Memory-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFF0);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFF0;
		const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
		if(pci_read!=0 && !( (get_number_of_highest_set_bit(pci_read) != 31) || (lowestBit > 31) || (lowestBit < 4) )) {
			pci_config_writed(addr,barOffset,pci_backup);
			pci_read=pci_config_readd(addr,barOffset);
			//kprintf("32-Bit-Memory-Ressource\n");
			uint16_t *address=(uint16_t*)((uint32_t)(pci_read & 0xFFFFFFF0));
			//kprintf("Offset 0x%x: %b - 0x%x\n",offset,address[offset],address[offset]);
			res=address[offset];
		}
	} else { //IO-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFFC);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFFC;
		if((pci_read) != 0) {
			const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
			const uint8_t highestBit = get_number_of_highest_set_bit(pci_read);
			if (!( ( (highestBit != 31) && (highestBit != 15) ) || (highestBit < lowestBit) || (lowestBit < 2) )) {
				pci_config_writed(addr,barOffset,pci_backup);
				pci_read=pci_config_readd(addr,barOffset);
				//kprintf("IO-Ressource\n");
				uint32_t address1=(uint32_t)(pci_read & 0xFFFFFFFC);
				//kprintf("Offset 0x%x: %b - 0x%x\n",offset,inb(address1 + offset),inb(address1 + offset));
				res=inw(address1 + offset);
			}
		}
	}
	pci_config_writed(addr,barOffset,pci_backup);
	return res;
}

uint32_t pci_read_register_32(pci_bdf_t addr, uint32_t barOffset, uint32_t offset) {
	uint16_t res;
	barOffset=(0x010+(barOffset*4));
	uint32_t pci_read=pci_config_readd(addr,barOffset);
	uint32_t pci_backup=pci_config_readd(addr,barOffset);
	if((pci_read & 0x1) == 0 && (pci_read >> 1 & 0x3) == 0) { //32-Bit-Memory-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFF0);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFF0;
		const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
		if(pci_read!=0 && !( (get_number_of_highest_set_bit(pci_read) != 31) || (lowestBit > 31) || (lowestBit < 4) )) {
			pci_config_writed(addr,barOffset,pci_backup);
			pci_read=pci_config_readd(addr,barOffset);
			//kprintf("32-Bit-Memory-Ressource\n");
			uint32_t *address=(uint32_t*)((uint32_t)(pci_read & 0xFFFFFFF0));
			//kprintf("Offset 0x%x: %b - 0x%x\n",offset,address[offset],address[offset]);
			res=address[offset];
		}
	} else { //IO-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFFC);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFFC;
		if((pci_read) != 0) {
			const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
			const uint8_t highestBit = get_number_of_highest_set_bit(pci_read);
			if (!( ( (highestBit != 31) && (highestBit != 15) ) || (highestBit < lowestBit) || (lowestBit < 2) )) {
				pci_config_writed(addr,barOffset,pci_backup);
				pci_read=pci_config_readd(addr,barOffset);
				//kprintf("IO-Ressource\n");
				uint32_t address1=(uint32_t)(pci_read & 0xFFFFFFFC);
				//kprintf("Offset 0x%x: %b - 0x%x\n",offset,inb(address1 + offset),inb(address1 + offset));
				res=inl(address1 + offset);
			}
		}
	}
	pci_config_writed(addr,barOffset,pci_backup);
	return res;
}

void pci_write_register_8(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint8_t val);

void pci_write_register_8(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint8_t val) {
	pci_write_register(addr, barOffset, offset, val);
}

void pci_write_register(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint8_t val) {
	barOffset=(0x010+(barOffset*4));
	uint32_t pci_read=pci_config_readd(addr,barOffset);
	uint32_t pci_backup=pci_config_readd(addr,barOffset);
	if((pci_read & 0x1) == 0 && (pci_read >> 1 & 0x3) == 0) { //32-Bit-Memory-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFF0);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFF0;
		const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
		if(pci_read!=0 && !( (get_number_of_highest_set_bit(pci_read) != 31) || (lowestBit > 31) || (lowestBit < 4) )) {
			pci_config_writed(addr,barOffset,pci_backup);
			pci_read=pci_config_readd(addr,barOffset);
			//kprintf("32-Bit-Memory-Ressource\n");
			uint8_t *address=(uint8_t*)((uint32_t)(pci_read & 0xFFFFFFF0));
			address[offset]=val;
		}
	} else { //IO-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFFC);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFFC;
		if((pci_read) != 0) {
			const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
			const uint8_t highestBit = get_number_of_highest_set_bit(pci_read);
			if (!( ( (highestBit != 31) && (highestBit != 15) ) || (highestBit < lowestBit) || (lowestBit < 2) )) {
				pci_config_writed(addr,barOffset,pci_backup);
				pci_read=pci_config_readd(addr,barOffset);
				//kprintf("IO-Ressource\n");
				uint32_t address1=(uint32_t)(pci_read & 0xFFFFFFFC);
				outb(address1 + offset,val);
			}
		}
	}
	pci_config_writed(addr,barOffset,pci_backup);
}

void pci_write_register_16(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint16_t val) {
	barOffset=(0x010+(barOffset*4));
	uint32_t pci_read=pci_config_readd(addr,barOffset);
	uint32_t pci_backup=pci_config_readd(addr,barOffset);
	if((pci_read & 0x1) == 0 && (pci_read >> 1 & 0x3) == 0) { //32-Bit-Memory-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFF0);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFF0;
		const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
		if(pci_read!=0 && !( (get_number_of_highest_set_bit(pci_read) != 31) || (lowestBit > 31) || (lowestBit < 4) )) {
			pci_config_writed(addr,barOffset,pci_backup);
			pci_read=pci_config_readd(addr,barOffset);
			//kprintf("32-Bit-Memory-Ressource\n");
			uint16_t *address=(uint16_t*)((uint32_t)(pci_read & 0xFFFFFFF0));
			address[offset]=val;
		}
	} else { //IO-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFFC);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFFC;
		if((pci_read) != 0) {
			const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
			const uint8_t highestBit = get_number_of_highest_set_bit(pci_read);
			if (!( ( (highestBit != 31) && (highestBit != 15) ) || (highestBit < lowestBit) || (lowestBit < 2) )) {
				pci_config_writed(addr,barOffset,pci_backup);
				pci_read=pci_config_readd(addr,barOffset);
				//kprintf("IO-Ressource\n");
				uint32_t address1=(uint32_t)(pci_read & 0xFFFFFFFC);
				outw(address1 + offset,val);
			}
		}
	}
	pci_config_writed(addr,barOffset,pci_backup);
}

void pci_write_register_32(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint32_t val) {
	barOffset=(0x010+(barOffset*4));
	uint32_t pci_read=pci_config_readd(addr,barOffset);
	uint32_t pci_backup=pci_config_readd(addr,barOffset);
	if((pci_read & 0x1) == 0 && (pci_read >> 1 & 0x3) == 0) { //32-Bit-Memory-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFF0);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFF0;
		const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
		if(pci_read!=0 && !( (get_number_of_highest_set_bit(pci_read) != 31) || (lowestBit > 31) || (lowestBit < 4) )) {
			pci_config_writed(addr,barOffset,pci_backup);
			pci_read=pci_config_readd(addr,barOffset);
			//kprintf("32-Bit-Memory-Ressource\n");
			uint32_t *address=(uint32_t*)((uint32_t)(pci_read & 0xFFFFFFF0));
			address[offset]=val;
		}
	} else { //IO-Ressource
		pci_config_writed(addr,barOffset,0xFFFFFFFC);
		pci_read=pci_config_readd(addr,barOffset) & 0xFFFFFFFC;
		if((pci_read) != 0) {
			const uint8_t lowestBit  = get_number_of_lowest_set_bit(pci_read);
			const uint8_t highestBit = get_number_of_highest_set_bit(pci_read);
			if (!( ( (highestBit != 31) && (highestBit != 15) ) || (highestBit < lowestBit) || (lowestBit < 2) )) {
				pci_config_writed(addr,barOffset,pci_backup);
				pci_read=pci_config_readd(addr,barOffset);
				//kprintf("IO-Ressource\n");
				uint32_t address1=(uint32_t)(pci_read & 0xFFFFFFFC);
				outl(address1 + offset,val);
			}
		}
	}
	pci_config_writed(addr,barOffset,pci_backup);
}