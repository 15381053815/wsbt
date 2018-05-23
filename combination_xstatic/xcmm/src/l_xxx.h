#ifndef __L_XXX_H_
#define __L_XXX_H_

#ifdef __cplusplus
extern "C" {
#endif
	int lxexec(void* fh);
	int lxcas(void* addr,
			void* oc,void* nc);
#ifdef __cplusplus
}
#endif
#endif