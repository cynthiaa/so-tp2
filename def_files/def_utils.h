/**
 * @file def_utils.h
 *
 * @brief pegado de cadenas con resolución en tiempo de preprocesamiento
 */

#ifndef _DEF_UTILS_H_
#define _DEF_UTILS_H_


#define __PASTE_(a,b)     a ## b
#define __PASTE2(a,b)     __PASTE_(a,b)
#define __PASTE3(a,b,c)   __PASTE2(a,__PASTE2(b,c))


#endif /* _DEF_UTILS_H_ */

