/*-
 * addr_descriptor.h - IP address descriptor 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    descriptor 
 * \{
 *
 * \file
 * \brief       Header definitions for the address descriptor functions
 *  
 *            Each addr_descriptor has a corresponding addr at the same id from 0 to ADDR_PREDEF_LEN-1.
 *            Predefined address informations have no explicit descriptor and are stored after the ADDR_DESCRIPTORS_LEN (n in the follwing figure) address informations:
 *\verbatim
 +--------------------+-----+--------------------+
 |    Descriptor 0    | ... |    Descriptor n    |
 +--------------------+-----+--------------------+--------------------+-----+
 |       Addr 0       | ... |       Addr n       | Addr_Predefined 0  | ... |
 +--------------------+-----+--------------------+--------------------+-----+
 \endverbatim
 *
 * \author      Wenwen Chen 
 */
#ifndef __ADDR_DESCRIPTOR_H__
#define __ADDR_DESCRIPTOR_H__

#include "descriptor.h"
#include "utils.h"

#ifdef  __cplusplus
extern "C" {
#endif
#if NO_DESCRIPTOR < ADDR_DESCRIPTORS_LEN
#error "NO_DESCRIPTOR should be not smaller then ADDR_DESCRIPTORS_LEN, change NO_DESCRIPTOR in descriptor.h"
#endif

  /*!
   * \brief Length of address data for decriptor
   */
#define ADDR_SIZE sizeof(ADDR_TYPE)

  /*!
   * \brief descriptor for address information
   */
  extern descriptor addr_descriptors[ADDR_DESCRIPTORS_LEN];

  /*!
   * \brief Address informations
   */
  extern ADDR_TYPE descriptor_addrs[ADDR_DESCRIPTORS_LEN+ADDR_PREDEF_LEN]; 


  /*!
   * \brief Check if the addr descriptor of the given id
   *
   *  \param id       Identifier of the address descriptor to be checked
   *
   * \return          1 if it is active,
   *                  otherwise 0
   */
    inline int __attribute__((always_inline)) isActiveAddrDescriptor(unsigned int id){
      if(id < ADDR_DESCRIPTORS_LEN)
        return DESCRIPTOR_INACTIVE != addr_descriptors[id] ;
      if(id < ADDR_DESCRIPTORS_LEN + ADDR_PREDEF_LEN){
        return 1;
      }
      return 0;
    }  


  /*!
   * \brief Get address according to the given id
   *
   * \param id    Identifier of the required address
   *
   * \return      The pointer to the address
   */
  inline ADDR_TYPE * __attribute__((always_inline)) getAddr(unsigned int id){
    if(isActiveAddrDescriptor(id)){
      return descriptor_addrs + id;
    }
    return NULL;
  }

  /*!
   * \brief Update predefined address for the given id
   * 
   * \note  Predefined address has no explicit descriptor, because it should be always active
   *
   * \param id          Identifier of the required address storage
   *        p_addr      Pointer to a new address
   *        addr_size   Length of the new address   
   *
   * \return            On success SUCC is returned, otherwise FAIL.
   */
  int updatePredefAddrDescriptor(unsigned int id, void * p_addr, unsigned int addr_size);

  /*!
   * \brief Update address for the given id
   *
   * \param id          Identifier of the required address storage
   *        p_addr      Pointer to a new address
   *        addr_size   Length of the new address   
   *
   * \return            On success SUCC is returned, otherwise FAIL.
   */
  int updateAddrDescriptor(unsigned int id, void * p_addr, unsigned int addr_size);

  /*!
   * \brief Update predefined address for the given id
   * 
   * \note  Predefined address has no explicit descriptor, because it should be always active
   *
   * \param id    Identifier of the required address storage
   *        ip    New IP address
   *        port  New port
   *
   * \return      On success SUCC is returned, otherwise FAIL.
   */
  inline int __attribute__((always_inline)) updatePredefAddrWithIpAndPort(unsigned int id, IPv4 ip, unsigned int port){
    if(id >= ADDR_DESCRIPTORS_LEN && id < ADDR_DESCRIPTORS_LEN+ADDR_PREDEF_LEN){
      descriptor_addrs[id].sin_family = ADDR_FAMILY;
      descriptor_addrs[id].sin_port = HTONS(port);
      ASSIGN_IP(descriptor_addrs[id].sin_addr.s_addr, ip);
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Update address for the given id
   *
   * \param id    Identifier of the required address storage
   *        ip    New IP address
   *        port  New port
   *
   * \return      None
   */
  inline int __attribute__((always_inline)) updateAddrWithIpAndPort(unsigned int id, IPv4 ip, unsigned int port){
    if(id < ADDR_DESCRIPTORS_LEN){
      descriptor_addrs[id].sin_family = ADDR_FAMILY;
      descriptor_addrs[id].sin_port = HTONS(port);
      ASSIGN_IP(descriptor_addrs[id].sin_addr.s_addr, ip);
      activeDescriptor(addr_descriptors+id);
      return SUCC;
    }
    return FAIL;
  }

  /*!
   * \brief Check if a given address is already stored at the given id
   *
   * \param id        Identifier of the address id to be checked
   *        p_addr    Pointer to a new address
   *        addr_size Size of the new address   
   *
   * \return          1 if the given addr is equal to the address of the given id,
   *                  otherwise 0.
   */
  int checkAddr(unsigned int id, void * p_addr, unsigned int addr_size);

  /*!
   * \brief Get the least active address descriptor 
   *
   * \return          The descriptor id for the least active address descriptor
   */
  inline unsigned int __attribute__((always_inline)) getLeastActiveAddrDescriptor(void){
    return getLeastActiveDescriptor(addr_descriptors, ADDR_DESCRIPTORS_LEN);
  }

  /*!
   * \brief Store a new address
   *
   * \param p_addr     Pointer to a new address
   *        addr_size  Length of the new address   
   *
   * \return          The descriptor id for the new given address
   */
  inline unsigned int __attribute__((always_inline)) addAddrDescriptor(ADDR_TYPE * p_addr, unsigned int addr_size){
    return addNewDescriptor(addr_descriptors, ADDR_DESCRIPTORS_LEN, ADDR_PREDEF_LEN, p_addr, addr_size, checkAddr, updateAddrDescriptor);
  }

  /*!
   * \brief Deative the addr descriptor of the given id
   *
   *  \param id        Identifier of the address descriptor to be deactived
   *
   * \return          None
   */
    inline void __attribute__((always_inline)) deactiveAddrDescriptor(unsigned int id){
      if(id < ADDR_DESCRIPTORS_LEN)
        deactiveDescriptor(addr_descriptors + id);
    }  

  /*!
   * \brief Print the address data for the given descriptor id
   *
   * \param id        Identifier of the address descriptor to be deactived
   *
   * \return          None
   */
  void printAddrDescriptor(unsigned int id);

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADDR_DESCRIPTOR_H__ */
/*!
 * \}
 */
