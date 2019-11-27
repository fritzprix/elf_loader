/*
 * main.c
 *
 *  Created on: 2015. 2. 3.
 *      Author: innocentevil
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elf_types.h"
#include "elf_parser.h"

const char* file_name = "./raw/tachyos_app_VER0.0.elf";

typedef struct tch_dynamic_bin_meta_struct{
	uint16_t b_type;
	uint32_t b_majorv;
	uint32_t b_minorv;
	uint32_t b_vendor;
	uint64_t b_appId;
	uint32_t b_key;
	uint32_t b_chk;
	uint32_t b_sz;
	uint32_t b_entry;
}* tch_loadableInfo;

static void init_appMeta(tch_loadableInfo meta);



int main(int argc, const char* argv[]){

	struct tch_dynamic_bin_meta_struct meta;
	init_appMeta(&meta);

	elf_handle handle = elfParse(argv[1]);
	meta.b_entry = (uint32_t)handle->getEntry(handle);
	meta.b_sz = handle->getLoadableSize(handle);
	uint8_t* exImg = malloc(meta.b_sz);
	handle->getLoadable(handle,exImg);

	FILE* metafile = fopen("appM.bin","w+");
	fwrite(&meta,sizeof(struct tch_dynamic_bin_meta_struct),1,metafile);
	fwrite(exImg,1,meta.b_sz,metafile);
	fclose(metafile);

	FILE* ascii = fopen("appT.txt","w+");
	uint32_t cnt = 0;
	uint8_t* meta_b = (uint8_t*)&meta;
	for(cnt = 0;cnt < sizeof(struct tch_dynamic_bin_meta_struct);cnt++){
		fputc('0',ascii);
		fputc('x',ascii);
		fprintf(ascii,"%x",meta_b[cnt]);
		fputc(',',ascii);
	}
	for(cnt = 0;cnt < meta.b_sz;cnt++){
	//	itoa(exImg[cnt],&c,16);
		fputc('0',ascii);
		fputc('x',ascii);
		fprintf(ascii,"%x",exImg[cnt]);
		fputc(',',ascii);
	}
	fclose(ascii);

	elfClose(handle);
	return 0;
}



static void init_appMeta(tch_loadableInfo meta){
	memset(meta,0,sizeof(struct tch_dynamic_bin_meta_struct));
	meta->b_key = (uint8_t) rand();
	meta->b_chk = meta->b_key ^ 0x0D;
	meta->b_sz = 0;
	meta->b_entry = 0;
}

