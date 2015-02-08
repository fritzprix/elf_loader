/*
 * elf_types.h
 *
 *  Created on: 2015. 2. 3.
 *      Author: innocentevil
 */

#ifndef ELF_TYPES_H_
#define ELF_TYPES_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif


#define ELF32_CLA			((uint8_t) 1)
#define ELF64_CLA			((uint8_t) 0)


typedef struct {
	uint8_t		el_mag0;
	uint8_t		el_mag1;
	uint8_t		el_mag2;
	uint8_t		el_mag3;
	uint8_t		el_class;
	uint8_t		el_enc;
	uint8_t		el_ver;
	uint8_t		el_osv;
	uint8_t		el_abiv;
	uint8_t		el_pad[6];
	uint8_t		el_size;
}ELF32Ident ;

typedef struct {
	ELF32Ident  e_ident;
	uint16_t	e_type;
	uint16_t	e_machine;
	uint32_t	e_version;
	uint32_t	e_entry;
	uint32_t	e_phoff;
	uint32_t	e_shoff;
	uint32_t	e_flags;
	uint16_t	e_ehsize;
	uint16_t	e_phentsize;
	uint16_t	e_phnum;
	uint16_t	e_shentsize;
	uint16_t	e_shnum;
	uint16_t	e_shstrndx;
}ELF32Header ;



#define SHF_WRITE 		((uint32_t) 0x1)   // section contains should be writable during process execution
#define SHF_ALLOC		((uint32_t) 0x2)

typedef struct {
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint32_t	sh_flags;
	uint32_t	sh_addr;
	uint32_t	sh_offset;
	uint32_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint32_t	sh_addralign;
	uint32_t	sh_entsize;
}ELF32SectionHeader;

#if defined(__cplusplus)
}
#endif


#endif /* ELF_TYPES_H_ */
