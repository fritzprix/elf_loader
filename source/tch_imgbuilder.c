/*
 * tch_loader.c
 *
 *  Created on: 2015. 2. 21.
 *      Author: manics99
 */


#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "tch_imgbuilder.h"
#include "elf_parser.h"


typedef struct loadable_image_header_s* loadableImgHeader;
typedef struct loadable_image_handle_proto* loadableImgHandlePrototype;

struct loadable_image_handle_proto {
	struct loadable_image_handle_s		_pix;
	loadableImgHeader					header;
	uint8_t* 							loadableImg;
};

struct loadable_image_header_s {
	uint32_t imgsz;			///< loadable image size in bytes (without header)
	uint32_t vendorId;		///< unique vendor identifier
	uint32_t appId;			///< vendor specific app identifier
	char name[10];			///< app name in null terminated character string
	uint16_t app_major;		///< app version specifier - major
	uint16_t app_minor;		///< app version specifier - minor
	uint16_t plf_type; 		///< platform type - type A (minimal node), type B (moderate functional node), type C (full featured node with power supply)
	uint16_t plf_major;		///< platform version specifier - tachyos major version
	uint16_t plf_minor;		///< platform version specifier - tachyos minor version
	uint64_t rq_hwdep;		///< hardware dependency flags (app has its own hardware dependency)
	uint32_t rq_stksz;		///< required stack size
	uint32_t rq_heapsz;		///< required heap size
	uint32_t entry;			///< entry address (offset from image base)
	uint32_t key;			///< key random key for generating check sum
	uint32_t chk;			///< checksum value for app binary image
} __attribute__((packed));


static uint32_t tch_imgBuilder_getSize(loadableImgHandle handle);
static void* tch_imgBuilder_write(loadableImgHandle handle,void* wb);

loadableImgHandle buildTchLoadable(const char* filename,loadableImgDefs def){
	elf_handle elf = elfParse(filename);
	if(elf == NULL)
		return NULL;
	loadableImgHandlePrototype ins = (loadableImgHandlePrototype) malloc(sizeof(struct loadable_image_handle_proto));
	uint32_t sz = elf->getLoadableSize(elf);
	uint32_t entry = (uint32_t)elf->getEntry(elf);
	uint8_t* img = ins->loadableImg = (uint8_t*) malloc(sizeof(struct loadable_image_header_s) + sz);

	loadableImgHeader header = ins->header =(loadableImgHeader) img;
	memcpy(header,def,sizeof(struct loadable_image_def_s));   // copy defs into header
	header->imgsz = sz;
	header->entry = entry;
	header->key = (filename[0] ^ filename[1]);
	img += sizeof(struct loadable_image_header_s);
	elf->getLoadable(elf,img);
	ins->_pix.getLoadableImage = tch_imgBuilder_write;
	ins->_pix.getSize = tch_imgBuilder_getSize;

	uint32_t i = 0;
	for(;i < header->imgsz;i++){
		if(i == 0){
			header->chk = header->key ^ img[i];
		}
		header->chk ^= img[i];
	}

	return (loadableImgHandle) ins;
}


static uint32_t tch_imgBuilder_getSize(loadableImgHandle handle){
	if(handle == NULL)
		return 0;
	loadableImgHandlePrototype ins = (loadableImgHandlePrototype) handle;
	return ins->header->imgsz + sizeof(struct loadable_image_header_s);
}

static void* tch_imgBuilder_write(loadableImgHandle handle,void* wb){
	if(handle == NULL)
		return NULL;
	loadableImgHandlePrototype ins = (loadableImgHandlePrototype) handle;
	memcpy(wb,ins->loadableImg,ins->header->imgsz + sizeof(struct loadable_image_header_s));
	return wb;
}
