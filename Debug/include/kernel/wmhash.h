#ifndef _HASH_H_
#define _HASH_H_

#ifdef __cplusplus
extern "C"{;
#endif

UINT32 hash(const void *key, UINT32 length, const UINT32 initval){ return length + initval;}

#ifdef __cplusplus
}
#endif

#endif