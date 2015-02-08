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

#define DECLARE_SECTION_HANDLER(fn)				void fn(const char* name,ELF32SectionHeader* header,void* img)

typedef struct {
	void (*onHandleSection)(const char* sname,ELF32SectionHeader* header,void* img);
}ElfResolver;

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct elf_handle_s {

}* elf_handle;

/**\brief initialize parser from elf file pointer
 *
 */
extern BOOL parse(FILE* fp,const ElfResolver* resolver);



#if defined(__cplusplus)
}
#endif

#endif /* ELF_PARSER_H_ */
