/*-
 * applicationlayerhandler.h - Application layer handler 
 *
 * Copyright 2005 Wenwen Chen
*/

/*!
 * \addtogroup    handler
 * \{
 *
 * \file
 * \brief       Header definitions for the application layer functions
 *
 * \author      Wenwen Chen 
 */
#ifndef __APPLICATION_LAYER_HANDLER_H__
#define __APPLICATION_LAYER_HANDLER_H__

#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

  unsigned int handleApplicationLayer(unsigned char *, unsigned int, unsigned int, unsigned int);
  unsigned char * generateApplicationLayer(unsigned int *, unsigned int *, unsigned int *);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_HANDLER_LAYER_H__ */
/*!
 * \}
 */
