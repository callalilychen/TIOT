/*-
 * applicationlayerhandler.h - Application layer handler 
 *
 * Copyright 2015 Wenwen Chen
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

  /*!
   * \brief Handle the given application layer message
   *
   * \param message                 The given application layer message
   * \param message_size            Size of the given application layer message
   * \param security_descriptor_id  Descriptor id for security issues
   * \param addr_descriptor_id      Descriptor id for message source address
   *
   * \return                        Id of the triggled application session
   */
  unsigned int handleApplicationLayer(unsigned char * message, unsigned int message_size, unsigned int security_descriptor_id, unsigned int addr_descriptor_id);


  /*!
   * \brief Generate application layer message for the given application message 
   *
   * \param message           The given application message 
   * \param message_size      Size of the given application message 
   * \param buf               Buffer to store the application layer message
   * \param max_buf_size      The maximal size of this buffer 
   *
   * \return                  The size of the generated application layer message
   */
  unsigned int generateApplicationLayer(unsigned char *message, unsigned int message_size, unsigned char *buf, unsigned int max_buf_size);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __APPLICATION_HANDLER_LAYER_H__ */
/*!
 * \}
 */
