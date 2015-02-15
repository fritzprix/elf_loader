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
static DECLARE_SEGMENT_HANDLER(segmentHandler);

static ElfResolver resolver;

int main(void){

	resolver.onHandleSection = sectionHandler;
	resolver.onHandleSegment = segmentHandler;
	elf_handle handle = elfParse(file_name);
	elfClose(handle);
	return 0;
}


static DECLARE_SECTION_HANDLER(sectionHandler){

}

static DECLARE_SEGMENT_HANDLER(segmentHandler){

}

