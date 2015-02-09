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



#define SHF_WRITE 				((uint32_t) 0x1)	///< section contains should be writable during process execution
#define SHF_ALLOC				((uint32_t) 0x2)	///< section occupies memory during process execution  : has size and address
#define SHF_EXECINSTR			((uint32_t) 0x4)	///< section contains executable
#define SHF_MERGE				((uint32_t) 0x10)	///< section may be merged to unnecessary eliminate duplication
#define SHF_STRINGS				((uint32_t) 0x20)	///< section consists of null-terminated character strings
#define SHF_INFO_LINK			((uint32_t) 0x40)	///< section info field in this section holds section header table index
#define SHF_LINK_ORDER 			((uint32_t) 0x80)	///< each section is deployed into output image in linked order
#define SHF_OS_NONCONFORMING	((uint32_t) 0x100)	///< require os specific porcessing
#define SHF_GROUP				((uint32_t) 0x200)	///< this section is member of a section group
#define SHF_TLS					((uint32_t) 0x400) 	///< thread local storage
#define SHF_MASKOS				((uint32_t) 0x0FF00000) 	///< os specific semantic
#define SHF_MASKPROC			((uint32_t) 0xF0000000)		///< processor specific semantic


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


#define DT_NULL				((uint32_t) 0)
#define DT_NEEDED			((uint32_t) 1)
#define DT_PLTRELSZ			((uint32_t) 2)
#define DT_PLTGOT			((uint32_t) 3)
#define DT_HASH				((uint32_t) 4)
#define DT_STRTAB			((uint32_t) 5)
#define DT_SYMTAB			((uint32_t) 6)
#define DT_RELA				((uint32_t) 7)
#define DT_RELASZ			((uint32_t) 8)
#define DT_RELENT			((uint32_t) 9)
#define DT_STRSZ			((uint32_t)10)
#define DT_SYMENT			((uint32_t)11)
#define DT_INIT				((uint32_t)12)
#define DT_FINIT			((uint32_t)13)
#define DT_SONAME			((uint32_t)14)
#define DT_RPATH			((uint32_t)15)
#define DT_SYMBOLIC			((uint32_t)16)
#define DT_REL				((uint32_t)17)
#define DT_RELSZ			((uint32_t)18)
#define DT_RELENT			((uint32_t)19)
#define DT_PLTREL			((uint32_t)20)
#define DT_DEBUG			((uint32_t)21)
#define DT_TEXTREL			((uint32_t)22)
#define DT_JMPREL			((uint32_t)23)
#define DT_BIND_NOW			((uint32_t)24)
#define DT_INIT_ARRAY		((uint32_t)25)
#define DT_FINIT_ARRAY		((uint32_t)26)
#define DT_INIT_ARRAYSZ		((uint32_t)27)
#define DT_FINIT_ARRAYSZ	((uint32_t)28)
#define DT_LOOS				((uint32_t)0x6000000D)
#define DT_HIOS				((uint32_t)0x6ffff000)
#define DT_LOPROC			((uint32_t)0x70000000)
#define DT_HIPROC			((uint32_t)0x7FFFFFFF)

typedef struct {
	uint32_t 	d_tag;
	union {
		uint32_t	d_val;
		uint32_t	d_ptr;
	}d_un;
}ELF32Dynamic;

#if defined(__cplusplus)
}
#endif


#endif /* ELF_TYPES_H_ */
