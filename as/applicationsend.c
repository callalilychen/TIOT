/*-
 * applicationsend.c - Send ommand appliction for authorization server 
 *
 * Copyright 2015 Wenwen Chen
 */

/*!
 * \addtogroup    as_application 
 * \{
 *
 * \file
 * \brief       Implementation of the authorization server send command applications 
 *
 * \author      Wenwen Chen 
 */
#include "applicationsend.h"
#include "securitydescriptor.h"
#include "securitylayerhandler.h"
#include "packagehandler.h"
#include "addr_descriptor.h"
#include "tree.h"
unsigned int selected_addr_id = 0;
unsigned int selected_security_id = 0;

/*!
 * \brief Handle send message cmd to the currently chosen address
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleSend(unsigned char* req, unsigned int req_size, application_session * p_session);
const application sendapplication = {
  .name = "s:",
  .name_size = 2,
  .usage = "%s\t<message>\tSend the given message to the selected address without security issues",
  .required_right = NO_RIGHT,
  .func = handleSend
};

unsigned int handleSend(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  if(!isActiveAddrDescriptor(selected_addr_id)){
    PRINT("%s Address descriptor %u does not exist or is not active\n", ERROR_MESSAGE, selected_addr_id);
    return 0;
  }
  if(selected_addr_id==test.addr_descriptor_id){
    pthread_mutex_lock(&(test.lock));
    if(TEST_RUNNING == test.status){
      pthread_mutex_unlock(&(test.lock));
      PRINT("A test is running, please enter 'tstop' to terminate it at frist!\n");
      return 0;
    }
    pthread_mutex_unlock(&(test.lock));
  }
  p_session->addr_descriptor_id = selected_addr_id;
  p_session->security_descriptor_id = NO_DESCRIPTOR;

  memcpy(p_session->message, req, req_size);
  p_session->message_size = req_size;
  return p_session->message_size;
}

/*!
 * \brief Handle send message command with currently chosen security issues to the currently chosen address
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleSecureSend(unsigned char* req, unsigned int req_size, application_session * p_session);
const application securesendapplication = {
  .name = "ss:",
  .name_size = 3,
  .usage = "%s\t<message>\tSend the given message to the selected address with security issues",
  .required_right = NO_RIGHT,
  .func = handleSecureSend
};

unsigned int handleSecureSend(unsigned char* req, unsigned int req_size, application_session * p_session){
  if(req_size < 0 || p_session == NULL){
    return 0;
  }
  if(!isActiveAddrDescriptor(selected_addr_id)){
    PRINT("%s Address descriptor %u does not exist or is not active\n", ERROR_MESSAGE, selected_addr_id);
    return 0;
  }
  if(!isActiveSecurityDescriptor(selected_security_id)){
    PRINT("%s Security descriptor %u does not exist or is not active\n", ERROR_MESSAGE, selected_security_id);
    return 0;
  }
  
  if(selected_addr_id==test.addr_descriptor_id || selected_security_id == test.security_descriptor_id){
    pthread_mutex_lock(&(test.lock));
    if(TEST_RUNNING == test.status){
      pthread_mutex_unlock(&(test.lock));
      PRINT("A test is running, please enter 'tstop' to terminate it at frist!\n");
      return 0;
    }
    pthread_mutex_unlock(&(test.lock));
  }
  p_session->addr_descriptor_id = selected_addr_id;
  p_session->security_descriptor_id = selected_security_id;

  TREE_STATE_TYPE key_id = getKeyIndex(p_session->security_descriptor_id);
  setKeyIndex(p_session->security_descriptor_id, key_id+1);
  key_id = getKeyIndex(p_session->security_descriptor_id);
  /* Calculate key */
  unsigned int depth = 2;
  tree_edge * edges = getEdges(depth);
  edges[0].func = edgeFunc;
  edges[0].params = getPermCode(p_session->security_descriptor_id, &(edges[0].params_size));

  edges[1].func = edgeFunc;
  edges[1].params = (unsigned char *)(&key_id);
  edges[1].params_size = TREE_STATE_SIZE;
  tree_node * p_key_node = NULL;

  p_key_node = fillNodes(getPathFromRoot(depth), edges, depth+1, 1);
  printBlock("Key", p_key_node->block, p_key_node->size);

  if(SUCC!=updateSecurityWithKey(p_session->security_descriptor_id, p_key_node)){
    updatePredefSecurityWithKey(p_session->security_descriptor_id, p_key_node);
  }
  memcpy(p_session->message, req, req_size);
  p_session->message_size = req_size;
  return p_session->message_size;
}

test_setting test = {0};


/*!
 * \brief Handle message, which begins with '['
 *
 *        Such messages will be just printed
 *
 * \param req       Rest of request message after application name
 * \param req_size  Size of this message
 * \param p_session Pointer to the corresponding application session
 *
 * \return          0 
 */
unsigned int handleMsg(unsigned char* req, unsigned int req_size, application_session * p_session);

const application msgapplication = {
  .name = "[",
  .name_size = 1,
  .usage = "\t\tAll messages, which begins with '[' will just be printed",
  .required_right = NO_RIGHT,
  .func = handleMsg
};

unsigned int handleMsg(unsigned char* req, unsigned int req_size, application_session * p_session){
  pthread_mutex_lock(&(test.lock));
  if(TEST_RUNNING==test.status && p_session->addr_descriptor_id == test.addr_descriptor_id){
    test.recv_counter++;
  }
  pthread_mutex_unlock(&(test.lock));
  req[req_size] = '\0';
  PRINT("[%s (%u)\n", req, req_size);
  return 0;
}
 /*!
  * \brief Test send thread function
  *
  * \note  Only plain texts are sent if security descriptor id is not set
  * 
  * \param nothing
  *
  * \return NULL
  */
void* testsend(void *nothing){
  unsigned int buf_index = 0;
  ADDR_SEND_TYPE * p_addr = (ADDR_SEND_TYPE *)getAddr(test.addr_descriptor_id);
  int sleep_flag = (test.interval.tv_sec>0 || test.interval.tv_nsec>0);
  while(1){
    pthread_mutex_lock(&(test.lock));
    if(TEST_IDLE==test.status){
      break;
    }
    for(int i =0; i < MAX_TEST_MESSAGE; i++){
      if(buf_index>=MAX_TEST_MESSAGE){
        buf_index-=MAX_TEST_MESSAGE;
      }
      if(test.buf_size[buf_index]>0){
        break;
      }
      buf_index++;
    }
    SENDTO_FUNC(udp_socket_fd, test.buf[buf_index], test.buf_size[buf_index], 0, p_addr, (ADDR_LEN_TYPE)ADDR_SIZE);
    buf_index++;
    test.send_counter ++;
    
    PRINT("Test %u is sent!\n",test.send_counter);
    if(test.times!=0 && test.times <= test.send_counter){
      break;
    }
    pthread_mutex_unlock(&(test.lock));
    if(sleep_flag){
      nanosleep(&(test.interval), NULL);
    }
  }
  float quoto = test.recv_counter/test.send_counter;
  PRINT("=====TEST RESULT=====\n");
  PRINT("%s Send %u packages, receive %u packages. %.1f%% package loss!\n",  test.recv_counter,test.send_counter,INFO_MESSAGE, 100*(1-quoto));
  PRINT("=====================\n");
  pthread_mutex_unlock(&(test.lock));
  return NULL;
}

/*!
 * \brief Handle test send command
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param secure_flag Flag to indicate, whether to handle package with security layer
 *
 * \return Response message size
 */
static unsigned int testSendCMD(unsigned char *req, unsigned int req_size, unsigned int secure_flag){
  pthread_mutex_lock(&(test.lock));
  if(TEST_RUNNING == test.status){
    pthread_mutex_unlock(&(test.lock));
    PRINT("Another test is running, please enter 'tstop' to terminate it at frist!\n");
  }else if(!isActiveAddrDescriptor(selected_addr_id)){
    pthread_mutex_unlock(&(test.lock));
    PRINT("%s Address descriptor %u does not exist or is not active\n", ERROR_MESSAGE, selected_addr_id);
  }else{
    int input_number = SSCAN((const char*)req, "%s,%s,%s,%s", test.buf[0], test.buf[1], test.buf[2], test.buf[3]);
    pthread_mutex_unlock(&(test.lock));
    PRINT("GET Variable %d\n", input_number);
    if(input_number > 0){
      pthread_mutex_lock(&(test.lock));
      for(int i = 0; i < MAX_TEST_MESSAGE; i++){
        test.buf_size[i] = strlen((const char*)(test.buf[i]));
      }
      test.recv_counter=0;
      test.send_counter=0;
      test.addr_descriptor_id = selected_addr_id;
      if(secure_flag){
        updatePredefSecurityWithProtocolType(PREDEF_TEST_SECURITY_DESCRIPTOR, getDescriptorProtocolType(selected_security_id));
        test.security_descriptor_id = PREDEF_TEST_SECURITY_DESCRIPTOR;
      }else{
        test.security_descriptor_id = NO_DESCRIPTOR;
      }
      test.status = TEST_RUNNING;
      pthread_mutex_unlock(&(test.lock));
      pthread_create(&(test.thread), NULL, testsend, NULL);
      PRINT("Test send");
      if(test.times > 0){
        PRINT(" %u times", test.times);
      }
      PRINT(":\nMessage 1: %s\nMessage 2: %s\nMessage 3: %s\nMessage 4: %s\n", test.buf[0], test.buf[1], test.buf[2], test.buf[3]);
      //FIXME message_sizes[1] = generateApplicationLayer(message2, strlen(message2), tmp_buf+message_sizes[0], BUFSIZE);
    }else{
      if(secure_flag){
        PRINT("%s %s%s\n", USAGE_MESSAGE, testsecuresendapplication.name, testsecuresendapplication.usage);
      }else{
        PRINT("%s %s%s\n", USAGE_MESSAGE, testsendapplication.name, testsendapplication.usage);
      }
    }
  }
  return 0;
    
}
/*!
 * \brief Handle test send message cmd to the currently chosen address
 *
 * \note  Only plain texts are sent
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleTestSend(unsigned char* req, unsigned int req_size, application_session * p_session);
const application testsendapplication = {
  .name = "ts:",
  .name_size = 3,
  .usage = "%s[,%s,%s,%s]\t<message>,...\tPeriodically send the given messages to the selected address without security issues",
  .required_right = NO_RIGHT,
  .func = handleTestSend
};

unsigned int handleTestSend(unsigned char* req, unsigned int req_size, application_session * p_session){
  return testSendCMD(req, req_size, 0);
}

/*!
 * \brief Handle test send message cmd to the currently chosen address with security issues
 *
 * \note  Only plain texts are sent
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleTestSecureSend(unsigned char* req, unsigned int req_size, application_session * p_session);
const application testsecuresendapplication = {
  .name = "tss:",
  .name_size = 4,
  .usage = "%s[,%s,%s,%s]\t<message>,...\tPeriodically send the given messages to the selected address with security issues",
  .required_right = NO_RIGHT,
  .func = handleTestSecureSend
};

unsigned int handleTestSecureSend(unsigned char* req, unsigned int req_size, application_session * p_session){
  return testSendCMD(req, req_size, 1);
}


/*!
 * \brief Handle request of test setting
 *
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
  unsigned int handleTestSetting(unsigned char* req, unsigned int req_size, application_session * p_session);
  const application testsettingapplication = {
    .name = "tsetting",
    .name_size = 8,
    .usage = "\t\tShow the test setting",
    .required_right = NO_RIGHT,
    .func = handleTestSetting
  };

unsigned int handleTestSetting(unsigned char* req, unsigned int req_size, application_session * p_session){
  PRINT("Test send every ");
  if(test.interval.tv_sec > 0){
    PRINT("%u s", test.interval.tv_sec);
  }else if(test.interval.tv_nsec >= 1000000L){
    PRINT("%u ms", test.interval.tv_nsec/1000000L);
  }else if(test.interval.tv_nsec >= 1000){
    PRINT("%u us", test.interval.tv_nsec/1000);
  }else{
    PRINT("%u ns", test.interval.tv_nsec);
  }
  if(test.times > 0){
    PRINT(" %u times", test.times);
  }
  PRINT("\n");
  return 0;
}
/*!
 * \brief Handle test config request
 *
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
  unsigned int handleTestConfig(unsigned char* req, unsigned int req_size, application_session * p_session);
  const application testconfigapplication = {
    .name = "tedit:",
    .name_size = 6,
    .usage = "%u:%u%s\t<times>,<interval><ms/s>\tSet test send periode",
    .required_right = NO_RIGHT,
    .func = handleTestConfig
  };

unsigned int handleTestConfig(unsigned char* req, unsigned int req_size, application_session * p_session){
  pthread_mutex_lock(&(test.lock));
  if(TEST_RUNNING == test.status){
    pthread_mutex_unlock(&(test.lock));
    PRINT("A test is running, please enter 'tstop' to terminate it at frist!\n");
  }else{
    unsigned char unit[3] = {0};
    unsigned int interval;
    int input_number = SSCAN((const char*)req, "%u:%u%s", &(test.times), &interval, unit);
    PRINT("Input %d, %s\n", input_number, unit);
    pthread_mutex_unlock(&(test.lock));
    if(input_number == 3){
      pthread_mutex_lock(&(test.lock));
      if(unit[0] == 'n'){
        test.interval.tv_sec = 0;
        test.interval.tv_nsec = interval;
      }else if(unit[0] == 'u'){
        test.interval.tv_sec = 0;
        test.interval.tv_nsec = interval*1000;
      }else if(unit[0] == 'm'){
        test.interval.tv_sec = 0;
        test.interval.tv_nsec = interval*1000000L;
      }else{
        test.interval.tv_sec = interval;
        test.interval.tv_nsec = 0;
      }
      pthread_mutex_unlock(&(test.lock));
      PRINT("Test send every %u %s", interval, unit);
      if(test.times > 0){
        PRINT(" %u times", test.times);
      }
      PRINT("\n");
    }else{
      PRINT("%s %s%s\n", USAGE_MESSAGE, testconfigapplication.name, testconfigapplication.usage);
    }
  }
  return 0;
}
/*!
 * \brief Handle test send message cmd to the currently chosen address with security issues
 *
 * \note  Only plain texts are sent
 *
 * \param req      Rest of request message after application name
 * \param req_size Size of this message
 * \param p_session pointer to the corresponding application session
 *
 * \return Response message size
 */
unsigned int handleTestStop(unsigned char* req, unsigned int req_size, application_session * p_session);
const application teststopapplication = {
  .name = "tstop",
  .name_size = 5,
  .usage = "\t\tTerminate the current test",
  .required_right = NO_RIGHT,
  .func = handleTestStop
};

unsigned int handleTestStop(unsigned char* req, unsigned int req_size, application_session * p_session){
  pthread_mutex_lock(&(test.lock));
  test.status = TEST_IDLE;
  pthread_mutex_unlock(&(test.lock));
  return 0;
}
/*!
 * \}
 */
