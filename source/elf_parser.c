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


const uint8_t ELF_MAGIC[] = {
		0x7F,
		'E',
		'L',
		'F'
};


typedef struct elf_image_s {
	uint32_t sz;
	ELF32Header header;
	void*		shdrs;
	void*		phdrs;
	char*       strs;
}* elf_image;

static struct elf_image_s elfImg;

static int elf_isValidElf(ELF32Header* header);
static void elf_prtElfHeader(ELF32Header* header);
static void elf_prtSecHeader(ELF32SectionHeader* sheader);



BOOL parse(FILE* fp,const ElfResolver* resolver){
	char* sc_str = NULL;
	uint8_t sidx = 0;
	uint8_t* segment = NULL;
	fseek(fp,0,SEEK_END);
	elfImg.sz = ftell(fp);
	fseek(fp,0,SEEK_SET);
	if(fread(&elfImg.header,sizeof(ELF32Header),1,fp) == 0)
		return FALSE;
	if(!elf_isValidElf(&elfImg.header)){
		printf("Invalid File Format\n");
		return FALSE;
	}
	elf_prtElfHeader(&elfImg.header);
	if(elfImg.header.e_ident.el_class != ELF32_CLA)
		return FALSE;
	elfImg.shdrs = malloc(elfImg.header.e_shentsize * elfImg.header.e_shnum);   // prepare for memory space for read section header

	fseek(fp,elfImg.header.e_shoff,SEEK_SET);									// seek to section header table
	fread(elfImg.shdrs,elfImg.header.e_shentsize,elfImg.header.e_shnum,fp);		// read section header table

	/// load string section
	ELF32SectionHeader* shdr = &((ELF32SectionHeader*) elfImg.shdrs)[elfImg.header.e_shstrndx];
	elfImg.strs = malloc(shdr->sh_size * sizeof(uint8_t));
	fseek(fp,shdr->sh_offset,SEEK_SET);
	fread(elfImg.strs,sizeof(uint8_t),shdr->sh_size,fp);

	for(;sidx < elfImg.header.e_shnum;sidx++){
		shdr = &((ELF32SectionHeader*) elfImg.shdrs)[sidx];
		if(shdr->sh_size){
			segment = malloc(shdr->sh_size);
			fseek(fp,shdr->sh_offset,SEEK_SET);
			fread(segment,sizeof(uint8_t),shdr->sh_size,fp);
			printf("\n");
			elf_prtSecHeader(shdr);
			printf("\n");
			resolver->onHandleSection(&elfImg.strs[shdr->sh_name],shdr,segment);
			free(segment);
		}
	}

	free(elfImg.shdrs);

	return TRUE;
}


static int elf_isValidElf(ELF32Header* header){
	return (memcmp(header,&ELF_MAGIC,sizeof(ELF_MAGIC)) == 0);
}

static void elf_prtElfHeader(ELF32Header* header){
	printf("===============       ELF32 Header       ======================\n");
	printf("======= Entry Address                        : %10d =====\n",header->e_entry);
	printf("======= Bytes offset of Program Header Table : %10d =====\n",header->e_phoff);
	printf("======= Bytes size of Program Header Table   : %10d =====\n",header->e_version);
	printf("======= Number of Entries in Program Header  : %10d =====\n",header->e_phnum);
	printf("======= Bytes offset of Section Header Table : %10d =====\n",header->e_shoff);
	printf("======= Bytes size of Section Header Table   : %10d =====\n",header->e_shentsize);
	printf("======= Number of entries in Section Header  : %10d =====\n",header->e_shnum);
	printf("======= String Table Index                   : %10d =====\n",header->e_shstrndx);
}

static void elf_prtSecHeader(ELF32SectionHeader* sheader){
	printf("==========       ELF32 Section    @   %10d  ==========\n",sheader->sh_offset);
	printf("==========       Section Name      :  %10s  ==========\n",&elfImg.strs[sheader->sh_name]);
	printf("==========       Section Base Addr :  %10d  ==========\n",sheader->sh_addr);
	printf("==========       Section Size      :  %10d  ==========\n",sheader->sh_size);
}


