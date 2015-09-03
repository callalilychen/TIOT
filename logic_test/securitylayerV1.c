#include "securitylayerV1.h"

#include <string.h>

static uint8_t * dynamicParse(uint8_t * des, uint8_t des_len, uint8_t *msg, uint8_t field_len){
  if(field_len > des_len){
    msg += (field_len - des_len);
    field_len = des_len;
  }else{
    des += (des_len - field_len);
  }
  memcpy(des, msg, field_len);
  return msg+field_len;
}

int parser1(unsigned char* msg, uint8_t msg_len,  message *p_message){
  securityHeaderV1 * p_header = &(p_message -> header);
  uint8_t atl = AUTHORIZATION_TOKEN_LEN(p_header->indicator);
  msg_len -= atl;
  if(msg_len < MAC_LEN){
    return FAIL;
  }
  
  token * p_token = &(p_header -> authorization_token);
  p_token->len = *((uint16_t *)msg);
  uint16_t t_len = p_token -> len;

  msg += 2;
  atl -= 2;
  if(AS_ID(t_len)){
    p_token->as_id = *((uint16_t *)msg);
    msg +=2;
    atl -= 2;
  } 
  if(RS_ID(t_len)){
    p_token->rs_id = *((uint16_t *)msg);
    msg +=2;
    atl -= 2;
  } 
  msg = dynamicParse((uint8_t *)&(p_token->secret_index), sizeof(STATE_INDEX_TYPE), msg, SIL(t_len));
  atl -= SIL(t_len);
  msg = dynamicParse((uint8_t *)&(p_token->perm_index), sizeof(STATE_INDEX_TYPE), msg, PIL(t_len));
  atl -= PIL(t_len);
  msg = dynamicParse((uint8_t *)&(p_token->polices), sizeof(police), msg, APL(t_len));
  atl -= APL(t_len);
  msg = dynamicParse((uint8_t *)&(p_token->time), sizeof(timestamps), msg, TL(t_len));
  atl -= TL(t_len);
  
  if(PF(t_len)){
    p_token -> padding = * msg;
    msg ++;
    atl --;
  }

  msg_len -= KIL(t_len);
  if(atl < 0 || msg_len < MAC_LEN){
    return FAIL;
  }

  msg += atl;
  
  msg = dynamicParse((uint8_t *)&(p_header->key_index), sizeof(STATE_INDEX_TYPE), msg, KIL(t_len));

  printf("dynamic parse len %lu %lu, %lu\n", sizeof(STATE_INDEX_TYPE), sizeof(police), sizeof(timestamps));

  if(CIPHER_FLAG(p_header -> indicator)){
    if((--msg_len)<MAC_LEN){
      return FAIL;
    }
    p_header -> cipher = *msg;
    msg ++;
    if(CIPHER_FLAG(p_header -> cipher)){
      if((--msg_len)<MAC_LEN){
        return FAIL;
      }
      p_header -> res_cipher = *msg;
      msg++;
    }
    
  }

  p_message->header_len -= msg_len;
  p_message->payload_len = msg_len - MAC_LEN;
  p_message->p_payload = msg;
  memcpy(&(p_message->mac), msg+p_message->payload_len, MAC_LEN);

  return SUCC;  
}

static securityHeaderV1 * currHeader = NULL;

unsigned char * setHeaderV1(unsigned char * msg, size_t msg_size, uint8_t *p_header_size){
  return currHeader;
}

STATE_INDEX_TYPE getSecretIndexV1(){
  return (STATE_INDEX_TYPE)((currHeader->authorization_token).secret_index);
}

STATE_INDEX_TYPE getPermIndexV1(){
  return (STATE_INDEX_TYPE)((currHeader->authorization_token).perm_index);
}

STATE_INDEX_TYPE getKeyIndexV1(){
  return (STATE_INDEX_TYPE)(currHeader->key_index);
}

uint8_t setSecretIndexV1(unsigned char* msg, size_t msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)((currHeader->authorization_token).secret_index);
  return 1;
}

uint8_t setPermIndexV1(unsigned char* msg, size_t msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)((currHeader->authorization_token).perm_index);
  return 1;
}

uint8_t setKeyIndexV1(unsigned char* msg, size_t msg_size){
  if(msg_size < 1){
    return 0;
  }
  msg[0] = (unsigned char)(currHeader->key_index);
  return 1;
}
