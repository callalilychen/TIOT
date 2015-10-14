#include "connection.h"
#define SL_STOP_TIMEOUT        0xFF
/* Use bit 32: Lower bits of status variable are used for NWP events
 *      1 in a 'status_variable', the device has completed the ping operation
 *      0 in a 'status_variable', the device has not completed the ping operation
 */
#define STATUS_BIT_PING_DONE  31
/*
 * GLOBAL VARIABLES -- Start
 */
_u32  g_Status = 0;
_u32  g_PingPacketsRecv = 0;
_u32  g_GatewayIP = 0;
_u32  g_StationIP = 0;
/*
 * GLOBAL VARIABLES -- End
 */
/*
 * STATIC FUNCTION DEFINITIONS -- Start
 */

static void SimpleLinkPingReport(SlPingReport_t *pPingReport);
/*
 * STATIC FUNCTION DEFINITIONS -- End
 */
/*!
    \brief This function initializes the application variables

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
_i32 initializeConnectionVariables()
{
    g_Status = 0;
    g_PingPacketsRecv = 0;
    g_GatewayIP = 0;

    return SUCCESS;
}
    /*!
     * \brief Start the device in STA mode
     *
     * Following function configures the device to default state by cleaning
     * the persistent settings stored in NVMEM (viz. connection profiles &
     * policies, power policy etc)
     *
     * Applications may choose to skip this step if the developer is sure
     * that the device is in its default state at start of application
     *
     * Note that all profiles and persistent settings that were done on the
     * device will be lost
     */
_i32 startAP(void){
      SlPingStartCommand_t PingParams = {0};
    SlPingReport_t Report = {0};
    _u8 SecType = 0;
    _i32 mode = ROLE_STA;
_i32 retVal = configureSimpleLinkToDefaultState();
    if(retVal < 0)
    {
        if (DEVICE_NOT_IN_STATION_MODE == retVal)
            CLI_Write((_u8 *)" Failed to configure the device in its default state \n\r");

        LOOP_FOREVER();
    }

    CLI_Write((_u8 *)" Device is configured in default state \n\r");
    /*
     * Assumption is that the device is configured in station mode already
     * and it is in its default state
     */
    mode = sl_Start(0, 0, 0);
    if (ROLE_AP == mode)
    {
        /* If the device is in AP mode, we need to wait for this event before doing anything */
        while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
    }
    else
    {
        /* Configure CC3100 to start in AP mode */
        retVal = sl_WlanSetMode(ROLE_AP);
        if(retVal < 0)
            LOOP_FOREVER();

        /* Configure the SSID of the CC3100 */
        retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SSID,
                pal_Strlen(SSID_AP_MODE), (_u8 *)SSID_AP_MODE);
        if(retVal < 0)
            LOOP_FOREVER();

        SecType = SEC_TYPE_AP_MODE;
        /* Configure the Security parameter the AP mode */
        retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SECURITY_TYPE, 1,
                (_u8 *)&SecType);
        if(retVal < 0)
            LOOP_FOREVER();

        retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_PASSWORD, pal_Strlen(PASSWORD_AP_MODE),
                (_u8 *)PASSWORD_AP_MODE);
        if(retVal < 0)
            LOOP_FOREVER();

        retVal = sl_Stop(SL_STOP_TIMEOUT);
        if(retVal < 0)
            LOOP_FOREVER();

        CLR_STATUS_BIT(g_Status, STATUS_BIT_IP_ACQUIRED);

        mode = sl_Start(0, 0, 0);
        if (ROLE_AP == mode)
        {
            /* If the device is in AP mode, we need to wait for this event before doing anything */
            while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
        }
        else
        {
            CLI_Write((_u8 *)" Device couldn't be configured in AP mode \n\r");
            LOOP_FOREVER();
        }
    }

    CLI_Write((_u8 *)" Device started as Access Point\n\r");

    /* Wait */
    CLI_Write((_u8 *)" Waiting for clients to connect...!\n\r");
    while((!IS_IP_LEASED(g_Status)) || (!IS_STA_CONNECTED(g_Status))) { _SlNonOsMainLoopTask(); }
    CLI_Write((_u8 *)" Client connected to the device \n\r");
    CLI_Write((_u8 *)" Pinging...! \n\r");

    /* Set the ping parameters */
    PingParams.PingIntervalTime = PING_INTERVAL;
    PingParams.PingSize = PING_PKT_SIZE;
    PingParams.PingRequestTimeout = PING_TIMEOUT;
    PingParams.TotalNumberOfAttempts = NO_OF_ATTEMPTS;
    PingParams.Flags = 0;
    PingParams.Ip = g_StationIP; /* Fill the station IP address connected to CC3100 */

    /* Ping client connected to CC3100 */
    retVal = sl_NetAppPingStart((SlPingStartCommand_t*)&PingParams, SL_AF_INET,
                       (SlPingReport_t*)&Report, SimpleLinkPingReport);
    if(retVal < 0)
        LOOP_FOREVER();

    /* Wait */
    while(!IS_PING_DONE(g_Status)) { _SlNonOsMainLoopTask(); }

    if (0 == g_PingPacketsRecv)
    {
        CLI_Write((_u8 *)" A STATION couldn't connect to the device \n\r");
        ASSERT_ON_ERROR(LAN_CONNECTION_FAILED);
    }

    CLI_Write((_u8 *)" Device and the station are successfully connected \n\r");
    return SUCCESS;

}
    /*!
     * \brief Start the device in STA mode
     *
     * Following function configures the device to default state by cleaning
     * the persistent settings stored in NVMEM (viz. connection profiles &
     * policies, power policy etc)
     *
     * Applications may choose to skip this step if the developer is sure
     * that the device is in its default state at start of application
     *
     * Note that all profiles and persistent settings that were done on the
     * device will be lost
     */
_i32 startWlanStion(void){
    _i32 retVal = configureSimpleLinkToDefaultState();
    if(retVal < 0)
    {
        if (DEVICE_NOT_IN_STATION_MODE == retVal)
            CLI_Write((_u8 *)" Failed to configure the device in its default state \n\r");

        LOOP_FOREVER();
    }

    CLI_Write((_u8 *)" Device is configured in default state \n\r");

    /*
     * Assumption is that the device is configured in station mode already
     * and it is in its default state
     */
    retVal = sl_Start(0, 0, 0);
    if ((retVal < 0) ||
        (ROLE_STA != retVal) )
    {
        CLI_Write((_u8 *)" Failed to start the device \n\r");
        LOOP_FOREVER();
    }

    CLI_Write((_u8 *)" Device started as STATION \n\r");

    /* Connecting to WLAN AP */
    retVal = establishConnectionWithAP();
    if(retVal < 0)
    {
        CLI_Write((_u8 *)" Failed to establish connection w/ an AP \n\r");
        LOOP_FOREVER();
    }
  CLI_Write((_u8 *)" Connection established w/ AP and IP is acquired \n\r");
  return retVal;
}

/*!
    \brief This function configure the SimpleLink device in its default state. It:
           - Sets the mode to STATION
           - Configures connection policy to Auto and AutoSmartConfig
           - Deletes all the stored profiles
           - Enables DHCP
           - Disables Scan policy
           - Sets Tx power to maximum
           - Sets power policy to normal
           - Unregisters mDNS services
           - Remove all filters

    \param[in]      none

    \return         On success, zero is returned. On error, negative is returned
*/
_i32 configureSimpleLinkToDefaultState()
{
    SlVersionFull   ver = {0};
    _WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};

    _u8           val = 1;
    _u8           configOpt = 0;
    _u8           configLen = 0;
    _u8           power = 0;

    _i32          retVal = -1;
    _i32          mode = -1;

    mode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(mode);

    /* If the device is not in station-mode, try configuring it in station-mode */
    if (ROLE_STA != mode)
    {
        if (ROLE_AP == mode)
        {
            /* If the device is in AP mode, we need to wait for this event before doing anything */
            while(!IS_IP_ACQUIRED(g_Status)) { _SlNonOsMainLoopTask(); }
        }

        /* Switch to STA role and restart */
        retVal = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(retVal);

        retVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(retVal);

        /* Check if the device is in station again */
        if (ROLE_STA != retVal)
        {
            /* We don't want to proceed if the device is not coming up in station-mode */
            ASSERT_ON_ERROR(DEVICE_NOT_IN_STATION_MODE);
        }
    }

    /* Get the device's version-information */
    configOpt = SL_DEVICE_GENERAL_VERSION;
    configLen = sizeof(ver);
    retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &configOpt, &configLen, (_u8 *)(&ver));
    ASSERT_ON_ERROR(retVal);

    /* Set connection policy to Auto + SmartConfig (Device's default connection policy) */
    retVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove all profiles */
    retVal = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(retVal);

    /*
     * Device in station-mode. Disconnect previous connection if any
     * The function returns 0 if 'Disconnected done', negative number if already disconnected
     * Wait for 'disconnection' event if 0 is returned, Ignore other return-codes
     */
    retVal = sl_WlanDisconnect();
    if(0 == retVal)
    {
        /* Wait */
        while(IS_CONNECTED(g_Status)) { _SlNonOsMainLoopTask(); }
    }

    /* Enable DHCP client*/
    retVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&val);
    ASSERT_ON_ERROR(retVal);

    /* Disable scan */
    configOpt = SL_SCAN_POLICY(0);
    retVal = sl_WlanPolicySet(SL_POLICY_SCAN , configOpt, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Set Tx power level for station mode
       Number between 0-15, as dB offset from max power - 0 will set maximum power */
    power = 0;
    retVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (_u8 *)&power);
    ASSERT_ON_ERROR(retVal);

    /* Set PM policy to normal */
    retVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(retVal);

    /* Unregister mDNS services */
    retVal = sl_NetAppMDNSUnRegisterService(0, 0);
    ASSERT_ON_ERROR(retVal);

    /* Remove  all 64 filters (8*8) */
    pal_Memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
    retVal = sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask,
                       sizeof(_WlanRxFilterOperationCommandBuff_t));
    ASSERT_ON_ERROR(retVal);

    retVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(retVal);

    retVal = initializeConnectionVariables();
    ASSERT_ON_ERROR(retVal);

    return retVal; /* Success */
}

/*!
    \brief Connecting to a WLAN Access point

    This function connects to the required AP (SSID_NAME).
    The function will return once we are connected and have acquired IP address

    \param[in]  None

    \return     0 on success, negative error-code on error

    \note

    \warning    If the WLAN connection fails or we don't acquire an IP address,
                We will be stuck in this function forever.
*/
_i32 establishConnectionWithAP()
{

  _i32 retVal = 0;
    SlSecParams_t secParams = {0};

    secParams.Key = (_i8 *)PASSKEY;
    secParams.KeyLen = pal_Strlen(PASSKEY);
    secParams.Type = SEC_TYPE;

    retVal = sl_WlanConnect((_i8 *)SSID_NAME, pal_Strlen(SSID_NAME), 0, &secParams, 0);
    ASSERT_ON_ERROR(retVal);

    /* Wait */
    while((!IS_CONNECTED(g_Status)) || (!IS_IP_ACQUIRED(g_Status))) { _SlNonOsMainLoopTask(); }

    return SUCCESS;
}
/*!
    \brief This function handles ping report events

    \param[in]      pPingReport holds the ping report statistics

    \return         None

    \note

    \warning
*/
static void SimpleLinkPingReport(SlPingReport_t *pPingReport)
{
    SET_STATUS_BIT(g_Status, STATUS_BIT_PING_DONE);

    if(pPingReport == NULL)
    {
        CLI_Write((_u8 *)" [PING REPORT] NULL Pointer Error\r\n");
        return;
    }

    g_PingPacketsRecv = pPingReport->PacketsReceived;
}
/*!
    \brief This function checks the LAN connection by pinging the AP's gateway

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
_i32 checkLanConnection()
{
    SlPingStartCommand_t pingParams = {0};
    SlPingReport_t pingReport = {0};

    _i32 retVal = -1;

    CLR_STATUS_BIT(g_Status, STATUS_BIT_PING_DONE);
    g_PingPacketsRecv = 0;

    /* Set the ping parameters */
    pingParams.PingIntervalTime = PING_INTERVAL;
    pingParams.PingSize = PING_PKT_SIZE;
    pingParams.PingRequestTimeout = PING_TIMEOUT;
    pingParams.TotalNumberOfAttempts = NO_OF_ATTEMPTS;
    pingParams.Flags = 0;
    pingParams.Ip = g_GatewayIP;

    /* Check for LAN connection */
    retVal = sl_NetAppPingStart( (SlPingStartCommand_t*)&pingParams, SL_AF_INET,
                                 (SlPingReport_t*)&pingReport, SimpleLinkPingReport);
    ASSERT_ON_ERROR(retVal);

    /* Wait */
    while(!IS_PING_DONE(g_Status)) { _SlNonOsMainLoopTask(); }

    if(0 == g_PingPacketsRecv)
    {
        /* Problem with LAN connection */
        ASSERT_ON_ERROR(LAN_CONNECTION_FAILED);
    }

    /* LAN connection is successful */
    return SUCCESS;
}

/*!
    \brief This function checks the internet connection by pinging
           the external-host (HOST_NAME)

    \param[in]  None

    \return     0 on success, negative error-code on error
*/
_i32 checkInternetConnection()
{
    SlPingStartCommand_t pingParams = {0};
    SlPingReport_t pingReport = {0};

    _u32 ipAddr = 0;

    _i32 retVal = -1;

    CLR_STATUS_BIT(g_Status, STATUS_BIT_PING_DONE);
    g_PingPacketsRecv = 0;

    /* Set the ping parameters */
    pingParams.PingIntervalTime = PING_INTERVAL;
    pingParams.PingSize = PING_PKT_SIZE;
    pingParams.PingRequestTimeout = PING_TIMEOUT;
    pingParams.TotalNumberOfAttempts = NO_OF_ATTEMPTS;
    pingParams.Flags = 0;
    pingParams.Ip = g_GatewayIP;

    /* Check for Internet connection */
    retVal = sl_NetAppDnsGetHostByName((_i8 *)HOST_NAME, pal_Strlen(HOST_NAME), &ipAddr, SL_AF_INET);
    ASSERT_ON_ERROR(retVal);

    /* Replace the ping address to match HOST_NAME's IP address */
    pingParams.Ip = ipAddr;

    /* Try to ping HOST_NAME */
    retVal = sl_NetAppPingStart( (SlPingStartCommand_t*)&pingParams, SL_AF_INET,
                                 (SlPingReport_t*)&pingReport, SimpleLinkPingReport);
    ASSERT_ON_ERROR(retVal);

    /* Wait */
    while(!IS_PING_DONE(g_Status)) { _SlNonOsMainLoopTask(); }

    if (0 == g_PingPacketsRecv)
    {
        /* Problem with internet connection*/
        ASSERT_ON_ERROR(INTERNET_CONNECTION_FAILED);
    }

    /* Internet connection is successful */
    return SUCCESS;
}

