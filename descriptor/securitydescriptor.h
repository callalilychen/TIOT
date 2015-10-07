/*-
 * securitydescriptor.h - Security descriptor 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    descriptor 
 * \{
 *
 * \file
 * \brief       Header definitions for the security descriptor functions
 *
 *              Each security_descriptor has a corresponding key and right information at the same id from 0 to SECURITY_PREDEF_LEN-1.
 *              Predefined security informations have no explicit descriptor and are stored after the ADDR_DESCRIPTORS_LEN (n in the follwing figure) security informations:
 *\verbatim
 +--------------------+-----+--------------------+
 |    Descriptor 0    | ... |    Descriptor n    |
 +--------------------+-----+--------------------+--------------------+-----+ \
 |       Key 0        | ... |       Key n        | Key_Predefined 0   | ... |  |
 +--------------------+-----+--------------------+--------------------+-----+  |- Descriptor Security
 |     Right 0        | ... |      Right n       | Right_Predefined 0 | ... |  |
 +--------------------+-----+--------------------+--------------------+-----+ /
 \endverbatim

 * \author      Wenwen Chen 
 */
#ifndef __SECURITY_DESCRIPTOR_H__
#define __SECURITY_DESCRIPTOR_H__

#include "tree.h" 
#include "securityhandler.h"
#include "descriptor.h"

#ifdef  __cplusplus
extern "C" {
#endif

  /*!
   * \brief descriptor for security informations
   */
  extern descriptor security_descriptors[SECURITY_DESCRIPTORS_LEN];

  /*!
   * \brief Security type for descriptor 
   */
  typedef struct descriptor_security{
    tree_node key;  /*!<Key nodes*/
    RIGHT_TYPE right; /*!<Right informations*/
    uint8_t protocol_type;  /*!<Protocol type for the security layer*/
  }descriptor_security;

  /*!
   * \brief Length of security data for decriptor
   */
#define DESCRIPTOR_SECURITY_SIZE sizeof(descriptor_security)

  /*!
   * \brief The index of the reserved temporary descriptor_security
   */
#define TMP_DESCRIPTOR_SECURITY_INDEX SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN

  /*!
   * \brief Macro indicate the type for security protocol is not defined or does not exist
   */
#define NO_SECURITY_PROTOCOL_TYPE 0xff 
  /*!
   * \brief Security informations
   */
  extern descriptor_security descriptor_securitys[SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN];

//  /*!
//   * \brief Update security protocol for PREDEF_NO_SECURITY_DESCRIPTOR 
//   *
//   * \return      None 
//   */
//  inline void (__attribute__((always_inline))initSecurityDescriptors)(){
//    descriptor_securitys[PREDEF_NO_SECURITY_DESCRIPTOR].key.size = 0;
//    descriptor_securitys[PREDEF_NO_SECURITY_DESCRIPTOR].right = NO_RIGHT;
//    //FIXME really DEFAULT_PRTOCOL_TYPE
//    descriptor_securitys[PREDEF_NO_SECURITY_DESCRIPTOR].protocol_type = DEFAULT_PROTOCOL_TYPE;
//  }
  /*!
   * \brief Get key node for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *
   * \return      The pointer to the key node, which is stored at the descriptor of id,
   *              or NULL, if id is out of range
   */
  inline tree_node * (__attribute__((always_inline))getDescriptorKey)(unsigned int id){
    if(id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
      return &(descriptor_securitys[id].key);
    }
    return NULL;
  }
  /*!
   * \brief Get right for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *
   * \return      The right, which is stored at the descriptor of id,
   *              or NULL, if id is out of range
   */
  inline RIGHT_TYPE (__attribute__((always_inline))getDescriptorRight)(unsigned int id){
    if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
      return descriptor_securitys[id].right;
    }
    return NO_RIGHT;
  }

  /*!
   * \brief Get protocol type for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *
   * \return      The protocol type, which is stored at the descriptor of id,
   *              or NULL, if id is out of range
   */
    inline uint8_t (__attribute__((always_inline))getDescriptorProtocolType)(unsigned int id){
      if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
        return descriptor_securitys[id].protocol_type;
      }
      return NO_SECURITY_PROTOCOL_TYPE; 
    } 

    /*!
     * \brief Update key node for the given descriptor id
     *
     * \param id    Identifier of the required security storage
     *        right New right
     *
     * \return      On success SUCC is returned, otherwise FAIL.
     */
  inline int (__attribute__((always_inline))updateSecurityWithKey)(unsigned int id, tree_node * p_key){
    if(id < SECURITY_DESCRIPTORS_LEN){
      if(p_key != NULL){
        copyTreeNode(&(descriptor_securitys[id].key), p_key);
        descriptor_securitys[id].key.size = p_key -> size;
        activeDescriptor(security_descriptors + id);
        return SUCC;
      }
    }
    return FAIL;
  }
  /*!
   * \brief Update right for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *        right New right
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
  inline int(__attribute__((always_inline))updateSecurityWithRight)(unsigned int id, tree_node * p_key, RIGHT_TYPE right){
    if(id < SECURITY_DESCRIPTORS_LEN){
      descriptor_securitys[id].right = right;
      activeDescriptor(security_descriptors + id);
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Update protocol type for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *        type  New protocol type
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
  inline int (__attribute__((always_inline))updateSecurityWithPrototypeType)(unsigned int id, uint8_t type){
    if(id < SECURITY_DESCRIPTORS_LEN){
      descriptor_securitys[id].protocol_type = type;
      activeDescriptor(security_descriptors + id);
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Update predefined key node for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *        right New right
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
    inline int (__attribute__((always_inline))updatePredefSecurityWithKey)(unsigned int id, tree_node * p_key){
      if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
        if(p_key != NULL){
          copyTreeNode(&(descriptor_securitys[id].key), p_key);
          descriptor_securitys[id].key.size = p_key -> size;
          return SUCC;
        }
        return FAIL;
      }
    }
  /*!
   * \brief Update predefined right for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *        right New right
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
  inline int(__attribute__((always_inline))updatePredefSecurityWithRight)(unsigned int id, tree_node * p_key, RIGHT_TYPE right){
    if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
      descriptor_securitys[id].right = right;
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Update predefined protocol type for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *        type  New protocol type
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
    inline int (__attribute__((always_inline))updatePredefSecurityWithPrototypeType)(unsigned int id, uint8_t type){
      if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
        descriptor_securitys[id].protocol_type = type;
        return SUCC;
      }
      return FAIL;
    }

    /*!
     * \brief Update security information for the given id
     *
     * \param id         Identifier of the required address storage
     *        p_sec      Pointer to a new security information
     *        addr_size  Length of the new address   
     *
     * \return      On success SUCC is returned, otherwise FAIL.
     */
  int updateSecurityDescriptor(unsigned int id, void * p_sec, unsigned int sec_siz);

  /*!
   * \brief Update predefined security information for the given id
   *
   * \param id         Identifier of the required address storage
   *        p_sec      Pointer to a new security information
   *        addr_size  Length of the new address   
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
  int updatePredefSecurityDescriptor(unsigned int id, void * p_sec, unsigned int sec_siz);

  /*!
   * \brief Check if a given security information is already stored at the given descriptor id
   *
   * \param id        Identifier of the isecurity id to be checked
   *        p_sec     Pointer to a new security information
   *        sec_size  Size of the new security information
   *
   * \return          1 if the given security information is equal to the data of the given id,
   *                  otherwise 0.
   */
          int checkSecurity(unsigned int id, void * p_sec, unsigned int sec_size);

  /*!
   * \brief Get the least active security descriptor 
   *
   * \return          The descriptor id for the least active security descriptor
   */
  inline unsigned int (__attribute__((always_inline))getLeastActiveSecurityDescriptor)(void){
    return getLeastActiveDescriptor(security_descriptors, SECURITY_DESCRIPTORS_LEN);
  }

  /*!
   * \brief Store a new security information
   *
   * \note  Predefined security information has no explicit descriptor, because it should be always active
   *
   * \param p_key Pointer to the new key node
   *        right New right
   *        type  New protocol type
   *
   * \return      None
   */
  inline unsigned int(__attribute__((always_inline))addSecurityDescriptor)(descriptor_security * p_sec){
    return addNewDescriptor(security_descriptors, SECURITY_DESCRIPTORS_LEN, SECURITY_PREDEF_LEN, p_sec, DESCRIPTOR_SECURITY_SIZE, checkSecurity, updateSecurityDescriptor);
  }

  /*!
   * \brief Deative the security descriptor at the given id
   *
   *  \param id        Identifier of the security descritptor to be deactived
   *
   * \return          None
   */
    inline void (__attribute__((always_inline))deactiveSecurityDescriptor)(unsigned int id){
      if(id < SECURITY_DESCRIPTORS_LEN)
        deactiveDescriptor(security_descriptors + id);
    } 


#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_DESCRIPTOR_H__ */
/*!
 * \}
 */
