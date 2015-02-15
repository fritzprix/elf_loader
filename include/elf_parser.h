/*
 * elf_parser.h
 *
 *  Created on: 2015. 2. 3.
 *      Author: innocentevil
 */

#ifndef ELF_PARSER_H_
#define ELF_PARSER_H_

#include "elf_types.h"

#ifndef BOOL
typedef enum { TRUE = (1 > 0), FALSE = (1 < 0) } BOOL;
#endif

#define DECLARE_SECTION_HANDLER(fn)				BOOL fn(const char* name,ELF32SectionHeader* header,void* img)
#define DECLARE_SEGMENT_HANDLER(fn)             BOOL fn(ELF32ProgramHeader* phdr,void* segment)

typedef struct {
	BOOL (*onHandleSection)(const char* sname,ELF32SectionHeader* header,void* img);
	BOOL (*onHandleSegment)(ELF32ProgramHeader* phdr,void* segment);
}ElfResolver;

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct elf_handle_s {

}* elf_handle;

/**\brief initialize parser from elf file pointer
 *
 */
extern elf_handle elfParse(const char* filename);
extern void elfClose(elf_handle handle);



#if defined(__cplusplus)
}
#endif

#endif /* ELF_PARSER_H_ */
