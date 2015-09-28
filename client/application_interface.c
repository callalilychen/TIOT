#include "application_interface.h"
#include "applicationexample.h"
#include "applicationcmd.h"

application_session application_sessions[APPLICATION_SESSIONS_LEN] = {0};
const application *msg_applications[MSG_APPLICATION_COUNT] = {&exampleapplication, &ackapplication, &repapplication};
#if(UI_APPLICATION_COUNT>0)
const application *ui_applications[UI_APPLICATION_COUNT] = {&exampleapplication, &repapplication, &keyapplication, &revapplication, &permreqapplication};
#endif
