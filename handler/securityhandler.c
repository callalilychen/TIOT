/*-
 * securityhandler.c - Security handler 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup  handler
 * \{
 *
 * \file
 * \brief       Implementation of the security functions
 *
 * \author      Wenwen Chen 
 */
#include "securityhandler.h"
#include <string.h>
#ifdef ADMIN_PASSWORD_HASH
RIGHT_TYPE askForAdminRight(void){
  /*!  
   * \todo Perhaps also other rights for ui applications
   * 
   * \brief When an ui command has no right for the required application, admin password will be asked.
   */
  PRINT("You has no right for this command! Do you still want to execute it as admin?[Y(es)/N(o)]");
  char input[10] = {0};
  SCAN("%s", input);
  if(input[0] == 'Y' || input[0] == 'y'){
    unsigned char hashvalue[HASH_SIZE];
    for(int j = 0; j < 3; j++){
      int input_size = SCAN("%s", input);
      /*!
       *  The hash value of the admin password is stored, so that, it is difficult for attacker to missuse the admin password
       *  \todo configure the hash value
       */
      //sha_construction.func((const unsigned char *)input, input_size, hashvalue);
      //if(0==memcmp(ADMIN_PASSWORD_HASH, hashvalue, sha_construction.size)){
      // TODO change admin password hash value
      if(0==memcmp(ADMIN_PASSWORD_HASH, ADMIN_PASSWORD_HASH, 1)){
        return ADMIN_RIGHT;
      }
    }
  }
  return NO_RIGHT;
}
#endif
  /*!
   * \}
   */
