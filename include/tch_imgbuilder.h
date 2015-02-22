/*
 * tch_loader.h
 *
 *  Created on: 2015. 2. 21.
 *      Author: manics99
 */

#ifndef TCH_LOADER_H_
#define TCH_LOADER_H_

#include <stdint.h>

#if defined(__cplusplus)
extern "C"{
#endif

typedef struct loadable_image_handle_s* loadableImgHandle;
typedef struct loadable_image_def_s* loadableImgDefs;


struct loadable_image_handle_s {
	uint32_t (*getSize)(loadableImgHandle handle);
	void* (*getLoadableImage)(loadableImgHandle handle,void* wb);
};

struct loadable_image_def_s {
	uint32_t vendorId;
	uint32_t appId;
	char name[10];
	uint16_t major;
	uint16_t minor;
	uint32_t rq_stksz;
	uint32_t rq_heapsz;
}__attribute__((packed));


extern loadableImgHandle buildTchLoadable(const char* filename,loadableImgDefs def);



#if defined(__cplusplus)
}
#endif


#endif /* TCH_LOADER_H_ */
