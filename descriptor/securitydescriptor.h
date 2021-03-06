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
 |  Protocol Type 0   | ... |   Protocol Type n  |  Type_Predefined 0 | ... |  |
 +--------------------+-----+--------------------+--------------------+-----+  |- Descriptor Security
 |       Key 0        | ... |       Key n        | Key_Predefined 0   | ... |  |
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
    uint8_t protocol_type;  /*!<Protocol type for the security layer*/
    tree_node key;  /*!<Key nodes*/
  }descriptor_security;

  /*!
   * \brief Length of security data for decriptor
   */
#define DESCRIPTOR_SECURITY_SIZE sizeof(descriptor_security)

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
  //  inline void __attribute__((always_inline))initSecurityDescriptors(){
  //    descriptor_securitys[PREDEF_NO_SECURITY_DESCRIPTOR].key.size = 0;
  //    descriptor_securitys[PREDEF_NO_SECURITY_DESCRIPTOR].right = NO_RIGHT;
  //    //FIXME really DEFAULT_PRTOCOL_TYPE
  //    descriptor_securitys[PREDEF_NO_SECURITY_DESCRIPTOR].protocol_type = DEFAULT_PROTOCOL_TYPE;
  //  }
  //

  /*!
   * \brief Check if the security descriptor of the given id
   *
   *  \param id       Identifier of the security descriptor to be checked
   *
   * \return          1 if it is active,
   *                  otherwise 0
   */
  inline int __attribute__((always_inline)) isActiveSecurityDescriptor(unsigned int id){
    if(id < SECURITY_DESCRIPTORS_LEN)
      return DESCRIPTOR_INACTIVE != security_descriptors[id] ;
    if(id < SECURITY_DESCRIPTORS_LEN + SECURITY_PREDEF_LEN){
      return 1;
    }
    return 0;
  }  

  /*!
   * \brief Active the security descriptor of the given id
   *
   *  \param id       Identifier of the security descriptor to be checked
   *
   *  \return         None
   *
   */
  inline int __attribute__((always_inline)) activeSecurityDescriptor(unsigned int id){
    if(id < SECURITY_DESCRIPTORS_LEN)
      activeDescriptor(security_descriptors+id);
  }
  /*!
   * \brief Get key node for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *
   * \return      The pointer to the key node, which is stored at the descriptor of id,
   *              or NULL, if id is out of range
   */
  inline tree_node * __attribute__((always_inline))getDescriptorKey(unsigned int id){
    if(isActiveSecurityDescriptor(id)){
      return &(descriptor_securitys[id].key);
    }
    return NULL;
  }

  /*!
   * \brief Get protocol type for the given descriptor id
   *
   * \param id    Identifier of the required security storage
   *
   * \return      The protocol type, which is stored at the descriptor of id,
   *              or NULL, if id is out of range
   */
  inline uint8_t __attribute__((always_inline))getDescriptorProtocolType(unsigned int id){
    if(isActiveSecurityDescriptor(id) ){
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
  inline int __attribute__((always_inline))updateSecurityWithKey(unsigned int id, tree_node * p_key){
    if(id < SECURITY_DESCRIPTORS_LEN){
      if(p_key != NULL){
        copyTreeNode(&(descriptor_securitys[id].key), p_key);
        activeDescriptor(security_descriptors + id);
        return SUCC;
      }
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
  inline int __attribute__((always_inline))updateSecurityWithProtocolType(unsigned int id, uint8_t type){
    if(id < SECURITY_DESCRIPTORS_LEN && type < SECURITY_LAYER_IMPLEMENTATIONS_LEN){
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
  inline int __attribute__((always_inline))updatePredefSecurityWithKey(unsigned int id, tree_node * p_key){
    if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN){
      if(p_key != NULL){
        copyTreeNode(&(descriptor_securitys[id].key), p_key);
        descriptor_securitys[id].key.size = p_key -> size;
        return SUCC;
      }
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
  inline int __attribute__((always_inline))updatePredefSecurityWithProtocolType(unsigned int id, uint8_t type){
    if(id >= SECURITY_DESCRIPTORS_LEN && id < SECURITY_DESCRIPTORS_LEN+SECURITY_PREDEF_LEN && type < SECURITY_LAYER_IMPLEMENTATIONS_LEN){
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
    int updateSecurityDescriptor(unsigned int id, void * p_sec, unsigned int sec_size);

  /*!
   * \brief Update predefined security information for the given id
   *
   * \param id         Identifier of the required address storage
   *        p_sec      Pointer to a new security information
   *        addr_size  Length of the new address   
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
  int updatePredefSecurityDescriptor(unsigned int id, void * p_sec, unsigned int sec_size);

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
  inline unsigned int __attribute__((always_inline))getLeastActiveSecurityDescriptor(void){
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
  inline unsigned int __attribute__((always_inline))addSecurityDescriptor(descriptor_security * p_sec){
    return addNewDescriptor(security_descriptors, SECURITY_DESCRIPTORS_LEN, SECURITY_PREDEF_LEN, p_sec, DESCRIPTOR_SECURITY_SIZE, checkSecurity, updateSecurityDescriptor);
  }

  /*!
   * \brief Deative the security descriptor at the given id
   *
   *  \param id        Identifier of the security descritptor to be deactived
   *
   * \return          None
   */
      inline void __attribute__((always_inline))deactiveSecurityDescriptor(unsigned int id){
        if(id < SECURITY_DESCRIPTORS_LEN)
          deactiveDescriptor(security_descriptors + id);
      } 

  /*!
   * \brief Print security descriptor header
   * 
   * \return None
   */
  void printSecurityDescriptorHeader();


  /*!
   * \brief Print security descriptor of the given id
   * 
   * \param id        Identifier of the security descritptor to be printed 
   *
   * \return None
   */
  void printSecurityDescriptor(unsigned int id);
  
  /*!
   * \brief Copy the destination security descriptor to the source security descriptor
   * 
   * \param dest_id        Identifier of the destination security descritptor 
   * \param src_id         Identifier of the source security descritptor 
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
  int copySecurityDescriptor(unsigned int dest_id, unsigned int src_id);
#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SECURITY_DESCRIPTOR_H__ */
/*!
 * \}
 */
