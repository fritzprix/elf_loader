/*
 * elf_parser.c
 *
 *  Created on: 2015. 2. 3.
 *      Author: innocentevil
 */



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#include "elf_parser.h"
#include "elf_types.h"
#define __DBG


const char* SHDR_TYPE_STRING[] = {
		"SHT_NULL",
		"SHT_PROGBITS",
		"SHT_SYMTAB",
		"SHT_STRTAB",
		"SHT_STRRELA",
		"SHT_HASH",
		"SHT_DYNAMIC",
		"SHT_NOTE",
		"SHT_NOBITS",
		"SHT_REL",
		"SHT_SHLIB",
		"SHT_DYNSYM",
		"RESERVED",
		"RESERVED",
		"SHT_INIT_ARRAY",
		"SHT_FINI_ARRAY",
		"SHT_PREINIT_ARRAY",
		"SHT_GROUP",
		"SHT_SYMTAB_SHNDX",
		"SHT_RESERVERD"
};

const uint8_t ELF_MAGIC[] = {
		0x7F,
		'E',
		'L',
		'F'
};

typedef struct elf_image_s {
	uint32_t 			filesz;
	ELF32Header 		header;
	void*				shdrs;
	void*				phdrs;
	char*       		snstr;    // section name string table
	char* 				symstr;
	ELF32SymbolEntry*	symTable;
	void* 				entry;
	uint32_t			limgsz;
	uint8_t*			limg;
}* elf_image;

typedef struct elf_handle_prototype_s {
	struct elf_handle_s	_pix;
	struct elf_image_s	 elfImg;
}* elf_handle_prototype;


static void* elf_getEntry(elf_handle handle);
static uint32_t elf_getLoadableSize(elf_handle handle);
static uint32_t elf_getLoadable(elf_handle handle,uint8_t* img);

static int elf_isValidElf(ELF32Header* header);
static void elf_prtElfHeader(elf_image img);
static void elf_prtPrgHeader(ELF32ProgramHeader* phdr);
static void elf_prtSecHeader(elf_image img,ELF32SectionHeader* sheader);

static BOOL onSectionHandle(elf_handle_prototype handle,const char* sname,ELF32SectionHeader* header,void* img);
static BOOL onPgmHeaderHandle(elf_handle_prototype handle,ELF32ProgramHeader* phdr,void* segment);




elf_handle elfParse(const char* fname){
	FILE* fp = fopen(fname,"r");
	char* sc_str = NULL;
	if(fp == NULL)
		return NULL;
	uint16_t sidx = 0,pidx = 0;
	uint8_t* segment = NULL;

	elf_handle_prototype ins = malloc(sizeof(struct elf_handle_prototype_s));
	struct elf_image_s* elfImg = &ins->elfImg;

	fseek(fp,0,SEEK_END);
	elfImg->filesz = ftell(fp); // 	get file size
	fseek(fp,0,SEEK_SET);


	if(fread(&elfImg->header,sizeof(ELF32Header),1,fp) == 0) //	get elf header
		return FALSE;
	if(!elf_isValidElf(&elfImg->header)){
		printf("Invalid File Format\n");
		return FALSE;
	}

#if defined(__DBG)
	elf_prtElfHeader(elfImg);	// 	print elf
#endif

	if(elfImg->header.e_ident.el_class != ELF32_CLA)    // only accept 32 bit elf format
		return FALSE;

	elfImg->shdrs = malloc(elfImg->header.e_shentsize * elfImg->header.e_shnum);   // prepare for memory space for read section headers
	elfImg->phdrs = malloc(elfImg->header.e_phentsize * elfImg->header.e_phnum);


	fseek(fp,elfImg->header.e_shoff,SEEK_SET);									// seek to section header table entry
	fread(elfImg->shdrs,elfImg->header.e_shentsize,elfImg->header.e_shnum,fp);		// read all section headers

	fseek(fp,elfImg->header.e_phoff,SEEK_SET);
	fread(elfImg->phdrs,elfImg->header.e_phentsize,elfImg->header.e_phnum,fp);

	printf("machine code : %d\n",elfImg->header.e_machine);
	/// load string section
	ELF32ProgramHeader* phdr = NULL;
	ELF32SectionHeader* shdr = &((ELF32SectionHeader*) elfImg->shdrs)[elfImg->header.e_shstrndx];  // lookup section name string section
	elfImg->snstr = malloc(shdr->sh_size * sizeof(uint8_t)); 						// allocate chunk for string setction
	fseek(fp,shdr->sh_offset,SEEK_SET);											// move to string section
	fread(elfImg->snstr,sizeof(uint8_t),shdr->sh_size,fp);

	// iterate section header and bind required reference into elfimg object
	elfImg->limgsz = 0;
	for(sidx = 0;sidx < elfImg->header.e_shnum;sidx++){
		shdr = &((ELF32SectionHeader*) elfImg->shdrs)[sidx];
		if((shdr->sh_flags & SHF_ALLOC) && (shdr->sh_size)){
			elfImg->limgsz += shdr->sh_size;
		}
	}

	printf("Loadable Image Size : %d \n",elfImg->limgsz);
	elfImg->limg = (uint8_t*) malloc(elfImg->limgsz);

	for(sidx = 0;sidx < elfImg->header.e_shnum;sidx++){
		shdr = &((ELF32SectionHeader*) elfImg->shdrs)[sidx];
		char* sname = NULL;
		if(shdr->sh_size){
#if defined(__DBG)
				elf_prtSecHeader(elfImg,shdr);
#endif
			if(shdr->sh_flags & SHF_ALLOC){
				fseek(fp,shdr->sh_offset,SEEK_SET);
				fread(&elfImg->limg[shdr->sh_addr],1,shdr->sh_size,fp);
			}else {
				segment = malloc(shdr->sh_size);
				fseek(fp,shdr->sh_offset,SEEK_SET);
				fread(segment,sizeof(uint8_t),shdr->sh_size,fp);
				sname = &elfImg->snstr[shdr->sh_name];
				if(!onSectionHandle(ins,&elfImg->snstr[shdr->sh_name],shdr,segment)){
					free(segment);
				}
			}
		}
	}


	// iterate program header and bind requried reference into elfimg object
	for(;pidx < elfImg->header.e_phnum;pidx++){
		phdr = &((ELF32ProgramHeader*)elfImg->phdrs)[pidx];
		segment = malloc(phdr->p_memsz);
		fseek(fp,phdr->p_offset,SEEK_SET);
		fread(segment,sizeof(uint8_t),phdr->p_memsz,fp);
#if defined(__DBG)
		elf_prtPrgHeader(phdr);
#endif
		if(!onPgmHeaderHandle(ins,phdr,segment))
			free(segment);
	}
	fclose(fp);
	ins->_pix.getEntry = elf_getEntry;
	ins->_pix.getLoadable = elf_getLoadable;
	ins->_pix.getLoadableSize = elf_getLoadableSize;

	return (elf_handle)ins;
}


void elfClose(elf_handle handle){

}


static int elf_isValidElf(ELF32Header* header){
	return (memcmp(header,&ELF_MAGIC,sizeof(ELF_MAGIC)) == 0);
}

static void elf_prtElfHeader(elf_image img){
	printf("===============       ELF32 Header       ======================\n");
	printf("======= Entry Address                        : %10d =====\n",img->header.e_entry);
	printf("======= Bytes offset of Program Header       : %10d =====\n",img->header.e_phoff);
	printf("======= Bytes size of Program Header Table   : %10d =====\n",img->header.e_version);
	printf("======= Number of Entries in Program Header  : %10d =====\n",img->header.e_phnum);
	printf("======= Bytes offset of Section Header Table : %10d =====\n",img->header.e_shoff);
	printf("======= Bytes size of Section Header Table   : %10d =====\n",img->header.e_shentsize);
	printf("======= Number of entries in Section Header  : %10d =====\n",img->header.e_shnum);
	printf("======= String Table Index                   : %10d =====\n",img->header.e_shstrndx);
}


static void elf_prtSecHeader(elf_image img,ELF32SectionHeader* sheader){
	printf("\n");
	char* typestr = (char*) SHDR_TYPE_STRING[19];
	printf("==========       ELF32 Section    @   %15d  ==========\n",sheader->sh_offset);
	printf("==========       Section Name      :  %15s  ==========\n",&img->snstr[sheader->sh_name]);
	printf("==========       Section Base Addr :  %15d  ==========\n",sheader->sh_addr);
	printf("==========       Section Size      :  %15d  ==========\n",sheader->sh_size);
	printf("==========       Section align     :  %15d  ==========\n",sheader->sh_addralign);
	if(sheader->sh_type < sizeof(SHDR_TYPE_STRING))
		typestr = (char*) SHDR_TYPE_STRING[sheader->sh_type];
	printf("==========       Section Type      :  %15s  ==========\n",typestr);

	printf("==========       Section Attributes:  ");
	if(sheader->sh_flags & SHF_ALLOC){
		printf("ALLOC | ");
	}
	if(sheader->sh_flags & SHF_EXECINSTR){
		printf("EXEC | ");
	}
	if(sheader->sh_flags & SHF_INFO_LINK){
		printf("HAS_LNK | ");
	}
	if(sheader->sh_flags & SHF_LINK_ORDER){
		printf("IS_ORDERED | ");
	}
	if(sheader->sh_flags & SHF_MERGE){
		printf("Has MERGEABLE | ");
	}
	if(sheader->sh_flags & SHF_STRINGS){
		printf("HAS_CSTR | ");
	}
	printf("\n");
	printf("==========       Section Entry Size:  %15d  ==========\n",sheader->sh_entsize);
}

static void elf_prtPrgHeader(ELF32ProgramHeader* phdr){
	printf("===============       ELF32 Program Header   ==================\n");
	printf("======= Segment Type                         : %10d =====\n",phdr->p_type);
	printf("======= Segment offset in bytes              : %10d =====\n",phdr->p_offset);
	printf("======= Segment Virtual Address              : %10d =====\n",phdr->p_vaddr);
	printf("======= Segment Physical Address             : %10d =====\n",phdr->p_paddr);
	printf("======= Segment file size in bytes           : %10d =====\n",phdr->p_filesz);
	printf("======= Segment mem size in bytes            : %10d =====\n",phdr->p_memsz);

}

static BOOL onSectionHandle(elf_handle_prototype handle,const char* name,ELF32SectionHeader* header,void* img){
	elf_handle_prototype hp = (elf_handle_prototype) handle;
	uint32_t entCnt = 0,idx = 0;
	if(!strcmp(name,".symtab")){
		hp->elfImg.symTable = (ELF32SymbolEntry*) img;
		printf("symtab : %d \n" ,img);
		printf("Sym Entry Cnt : %d\n",header->sh_size / sizeof(ELF32SymbolEntry));
		return TRUE;
	}
	if(!strcmp(name,".strtab")){
		hp->elfImg.symstr = (char*) img;
		printf("sym str : %d \n",img);
		return TRUE;
	}
	return FALSE;
}

static BOOL onPgmHeaderHandle(elf_handle_prototype handle,ELF32ProgramHeader* phdr,void* segment){

}

static void* elf_getEntry(elf_handle handle){
	elf_handle_prototype ins = (elf_handle_prototype) handle;
	if(!ins)
		return NULL;
	return (void*) ins->elfImg.header.e_entry;
}

static uint32_t elf_getLoadableSize(elf_handle handle){
	elf_handle_prototype ins = (elf_handle_prototype) handle;
	if(!ins)
		return 0;
	return ins->elfImg.limgsz;
}

static uint32_t elf_getLoadable(elf_handle handle,uint8_t* img){
	elf_handle_prototype ins = (elf_handle_prototype) handle;
	if(!ins)
		return 0;
	memcpy(img,ins->elfImg.limg,ins->elfImg.limgsz);
	return ins->elfImg.limgsz;
}


