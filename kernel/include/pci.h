#ifndef PCI_H
#define PCI_H
#include <stdint.h>

typedef struct {
	int bus;
	int dev;
	int func;
} pci_bdf_t;

typedef struct {
	pci_bdf_t address;
	uint16_t device_id;
	uint16_t vendor_id;
	uint16_t status;
	uint16_t command;
	uint8_t class_high;
	uint8_t class_middle;
	uint8_t prog_if;
	uint8_t revision;
	uint8_t BIST;
	uint8_t header_type;
	uint8_t latency_timer;
	uint8_t cache_line_size;
	uint8_t capabilities;
	uint8_t interrupt_pin;
	uint8_t interrupt_line;
} pci_device;

void get_pci_devices(void);
pci_device get_pci_device(pci_bdf_t addr);
void print_pci_info(pci_bdf_t dev);
void class_to_text(pci_device dev);
uint8_t get_number_of_highest_set_bit(uint32_t value);
uint8_t get_number_of_lowest_set_bit(uint32_t value);
uint8_t pci_read_register(pci_bdf_t addr, uint32_t barOffset, uint32_t offset);
uint8_t pci_read_register_8(pci_bdf_t addr, uint32_t barOffset, uint32_t offset);
uint16_t pci_read_register_16(pci_bdf_t addr, uint32_t barOffset, uint32_t offset);
uint32_t pci_read_register_32(pci_bdf_t addr, uint32_t barOffset, uint32_t offset);
void pci_write_register(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint8_t val);
void pci_write_register_16(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint16_t val);
void pci_write_register_32(pci_bdf_t addr, uint32_t barOffset, uint32_t offset, uint32_t val);
pci_bdf_t search_pci_device(uint16_t vendor_id, uint16_t device_id);

#endif