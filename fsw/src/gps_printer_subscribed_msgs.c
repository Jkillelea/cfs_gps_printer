/*
 * A table of messages that the GPS_PRINTER app subscribes to on startup.
 * The length of the table is defined in gps_printer_subscribed_msgs.h.
 * The table must end with GPS_PRINTER_SUBSCRIBETABLE_END.
 *
 */
#include "gps_printer_subscribed_msgs.h"
#include "gps_reader_msgids.h"

CFE_SB_MsgId_t GPS_Printer_SubscribeTable[GPS_PRINTER_SUBSCRIBETABLE_LEN+1] =
{
    GPS_READER_GPS_INFO_MSG,
    // GPS_READER_GPS_GPGGA_MSG,
    // GPS_READER_GPS_GPGSA_MSG,
    // GPS_READER_GPS_GPGSV_MSG,
    // GPS_READER_GPS_GPRMC_MSG,
    // GPS_READER_GPS_GPVTG_MSG,
    GPS_PRINTER_SUBSCRIBETABLE_END
};
