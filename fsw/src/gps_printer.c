#include "cfe.h"
#include "gps_reader_msgids.h"
#include "gps_reader_msgs.h"

CFE_SB_PipeId_t gpsPipe;
CFE_SB_MsgPtr_t gpsMessage;

void GPS_PRINTER_Main(void) {
    uint32 runStatus = CFE_ES_APP_RUN;
    GPS_PRINTER_Init();

    while (CFE_ES_RunLoop(&runStatus) == TRUE) {
        int32 status = CFE_SB_RcvMsg(&gpsMessage, gpsPipe, CFE_SB_PEND_FOREVER);
        if (status == CFE_SUCCESS) {
            OS_printf("GPS_PRINTER: got a gps message\n");
            
            CFE_SB_MsgId_t msgId = CFE_SB_GetMsgId(gpsMessage);
            
            switch (msgId) {
            case GPS_READER_GPS_INFO_MSG:
                OS_printf("GPS_PRINTER: GPS_READER_GPS_INFO_MSG Message\n");
                break;
            case GPS_READER_GPS_GPGGA_MSG:
                OS_printf("GPS_PRINTER: GPGGA Message\n");
                break;
            case GPS_READER_GPS_GPGSA_MSG:
                OS_printf("GPS_PRINTER: GPGGPS_READER_GPS_GPGSA_MSG Message\n");
                break;
            case GPS_READER_GPS_GPRMC_MSG:
                OS_printf("GPS_PRINTER: GPGGPS_READER_GPS_GPRMC_MSG Message\n");
                break;
            case GPS_READER_GPS_GPVTG_MSG:
                OS_printf("GPS_PRINTER: GPGGPS_READER_GPS_GPVTG_MSG Message\n");
                break;
            default:
                OS_printf("GPS_PRINTER: Unkown message ID %d\n", msgId);
                break;
            }
        }
    }
}

void GPS_PRINTER_Init(void) {
    CFE_ES_RegisterApp();
    CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);
    
    OS_printf("GPS_PRINTER: Startup.");

    CFE_SB_CreatePipe(&gpsPipe, 10, "GPS_PRINTER_PIPE");
    CFE_SB_Subscribe(GPS_READER_GPS_INFO_MSG, gpsPipe);
}
