#include "gps_reader_msgids.h"
#include "gps_reader_msgs.h"
#include "gps_print_msgs.h"
#include "gps_printer_perfids.h"

CFE_SB_PipeId_t gpsPipe;
CFE_SB_Buffer_t *gpsMessage;

#define GPS_PRINTER_MAX_SUBS (10)
void GPS_PRINTER_Init(void) {

    const CFE_SB_MsgId_t GpsPrinterSubs[GPS_PRINTER_MAX_SUBS] = {
        CFE_SB_ValueToMsgId(GPS_READER_GPS_INFO_MSG),
        // CFE_SB_ValueToMsgId(GPS_READER_GPS_GPGGA_MSG),
        // CFE_SB_ValueToMsgId(GPS_READER_GPS_GPGSA_MSG),
        // CFE_SB_ValueToMsgId(GPS_READER_GPS_GPGSV_MSG),
        // CFE_SB_ValueToMsgId(GPS_READER_GPS_GPRMC_MSG),
        // CFE_SB_ValueToMsgId(GPS_READER_GPS_GPVTG_MSG),
        CFE_SB_ValueToMsgId(0)
    };

    CFE_ES_WriteToSysLog("GPS_PRINTER: Startup.");

    CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);

    CFE_SB_CreatePipe(&gpsPipe, 10, "GPS_PRINTER_PIPE");

    for (int i = 0; i < GPS_PRINTER_MAX_SUBS; i++)
    {
        if (CFE_SB_MsgIdToValue(GpsPrinterSubs[i]) != 0)
        {
            CFE_Status_t status = CFE_SB_Subscribe(GpsPrinterSubs[i],  gpsPipe);
            if (status != CFE_SUCCESS)
            {
                CFE_ES_WriteToSysLog("GPS_PRINTER: Failed to subscribe to table index %d!\n", i);
            }
        }
        else
        {
            break;
        }
    }
}

void GPS_PRINTER_Main(void) {
    uint32 runStatus = CFE_ES_RunStatus_APP_RUN;
    GPS_PRINTER_Init();

    while (CFE_ES_RunLoop(&runStatus) == true) {

        CFE_ES_PerfLogExit(GPS_PRINTER_PERFID);

        int32 status = CFE_SB_ReceiveBuffer(&gpsMessage, gpsPipe, CFE_SB_PEND_FOREVER);

        CFE_ES_PerfLogEntry(GPS_PRINTER_PERFID);

        if (status == CFE_SUCCESS) {
            CFE_SB_MsgId_t msgId;
            CFE_MSG_GetMsgId(&gpsMessage->Msg, &msgId);

            switch (CFE_SB_MsgIdToValue(msgId)) {
            case GPS_READER_GPS_INFO_MSG: {
                GpsInfoMsg_t *gpsInfo = (GpsInfoMsg_t *) gpsMessage;
                print_info(&gpsInfo->gpsInfo);
                break;
            }
            case GPS_READER_GPS_GPGGA_MSG: {
                GpsGpggaMsg_t *gpsGpgga = (GpsGpggaMsg_t *) gpsMessage;
                print_gpgga(&gpsGpgga->gpsGpgga);
                break;
            }
            case GPS_READER_GPS_GPGSA_MSG: {
                GpsGpgsaMsg_t *gpsGpgsa = (GpsGpgsaMsg_t *) gpsMessage;
                print_gpgsa(&gpsGpgsa->gpsGpgsa);
                break;
            }
            case GPS_READER_GPS_GPGSV_MSG: {
                GpsGpgsvMsg_t *gpsGpgsv = (GpsGpgsvMsg_t *) gpsMessage;
                print_gpgsv(&gpsGpgsv->gpsGpgsv);
                break;
            }
            case GPS_READER_GPS_GPRMC_MSG: {
                GpsGprmcMsg_t *gpsGprmc = (GpsGprmcMsg_t *) gpsMessage;
                print_gprmc(&gpsGprmc->gpsGprmc);
                break;
            }
            case GPS_READER_GPS_GPVTG_MSG: {
                GpsGpvtgMsg_t *gpsGpvtg = (GpsGpvtgMsg_t *) gpsMessage;
                print_gpvtg(&gpsGpvtg->gpsGpvtg);
                break;
            }
            default:
                OS_printf("GPS_PRINTER: Unkown message ID 0x%x\n", CFE_SB_MsgIdToValue(msgId));
                break;
            }
        }
    }
}

