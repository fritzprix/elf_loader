/*
 * main.c
 *
 *  Created on: 2015. 2. 3.
 *      Author: innocentevil
 */


#include <stdio.h>
#include <stdlib.h>

#include "elf_types.h"
#include "elf_parser.h"

const char* file_name = "./raw/tachyos_app_VER0.0.elf";

static DECLARE_SECTION_HANDLER(sectionHandler);
static ElfResolver resolver;

int main(void){

	resolver.onHandleSection = sectionHandler;
	FILE* fp = fopen(file_name,"r");
	if(!fp){
		printf("File is not valid!!\n");
		return -1;
	}
	parse(fp,&resolver);
	fclose(fp);
	return 0;
}


static DECLARE_SECTION_HANDLER(sectionHandler){
	switch(header->sh_type){

	}
	printf("%s\n",name);
}
