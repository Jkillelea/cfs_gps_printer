#include "gps_print_msgs.h"

// convert from DDDMM.mmmmm (decimal minutes) to DDD.dddddd (plain decimal) format
// TODO: double check this!
static double decimal_minutes2decimal_decimal(const double decimal_minutes) {
    double degrees = ((int) (decimal_minutes/100.0)); // DDD
    double minutes = decimal_minutes - 100*degrees;   // MM.mmmmmm
    double decimal = minutes / 60;                    // 0.ddddddd
    return (degrees + decimal);                       // DDD.dddddd
}

void print_info(nmeaINFO *info) {
    OS_printf("info {\n");
    OS_printf("    mask        = %d,\n", info->smask);
    OS_printf("    utc         = %d-%d-%dT%d:%d:%d.%d\n",
            info->utc.year, info->utc.mon, info->utc.day,
            info->utc.hour, info->utc.min, info->utc.sec,
            info->utc.hsec);
    OS_printf("    sig         = %d,\n",  info->sig);
    OS_printf("    fix         = %d,\n",  info->fix);
    OS_printf("    PDOP        = %lf,\n", info->PDOP);
    OS_printf("    HDOP        = %lf,\n", info->HDOP);
    OS_printf("    VDOP        = %lf,\n", info->VDOP);
    OS_printf("    lat         = %lf,\n", decimal_minutes2decimal_decimal(info->lat));
    OS_printf("    lon         = %lf,\n", decimal_minutes2decimal_decimal(info->lon));
    OS_printf("    elv         = %lf,\n", info->elv);
    OS_printf("    speed       = %lf,\n", info->speed);
    OS_printf("    direction   = %lf,\n", info->direction);
    OS_printf("    declination = %lf,\n", info->declination);
    OS_printf("    satinfo = {\n");
    OS_printf("        inview = %d,\n", info->satinfo.inview);
    OS_printf("        inuse  = %d,\n", info->satinfo.inuse);
    OS_printf("    }\n");
    OS_printf("};\n");
}

void print_gpgga(nmeaGPGGA *gpgga) {
    OS_printf("GPGGA {\n");
    OS_printf("    utc      = %d-%d-%dT%d:%d:%d.%d\n",
            gpgga->utc.year, gpgga->utc.mon, gpgga->utc.day,
            gpgga->utc.hour, gpgga->utc.min, gpgga->utc.sec,
            gpgga->utc.hsec);
    OS_printf("    lat      = %lf %c,\n", decimal_minutes2decimal_decimal(gpgga->lat), gpgga->ns);
    OS_printf("    lon      = %lf %c,\n", decimal_minutes2decimal_decimal(gpgga->lon), gpgga->ew);
    OS_printf("    sig      = %d,\n",     gpgga->sig);
    OS_printf("    satinuse = %d,\n",     gpgga->satinuse);
    OS_printf("    HDOP     = %lf,\n",    gpgga->HDOP);
    OS_printf("    elv      = %lf %c,\n", gpgga->elv,  gpgga->elv_units);
    OS_printf("    diff     = %lf %c,\n", gpgga->diff, gpgga->diff_units);
    OS_printf("    dgps_age = %lf,\n",    gpgga->dgps_age);
    OS_printf("    dgps_sid = %d\n",      gpgga->dgps_sid);
    OS_printf("};\n");
}

void print_gpgsa(nmeaGPGSA *gpgsa) {
    OS_printf("GPGSA {\n");
    OS_printf("    fix_mode = %c,\n", gpgsa->fix_mode);
    OS_printf("    fix_type = %d,\n", gpgsa->fix_type);
    int i;
    for(i = 0; gpgsa->sat_prn[i] != 0 && i < NMEA_MAXSAT; i++)
    {
    OS_printf("    %d ", gpgsa->sat_prn[i]);
    }
    if (i > 0)
    {
    OS_printf("    \n");
    }

    OS_printf("    PDOP = %lf,\n", gpgsa->PDOP);
    OS_printf("    HDOP = %lf,\n", gpgsa->HDOP);
    OS_printf("    VDOP = %lf,\n", gpgsa->VDOP);
    OS_printf("};\n");
}

void print_gpgsv(nmeaGPGSV *gpgsv) {
    OS_printf("GPGSV {\n");
    OS_printf("    pack_count = %d\n", gpgsv->pack_count);
    OS_printf("    pack_index = %d\n", gpgsv->pack_index);
    OS_printf("    sat_count  = %d\n", gpgsv->sat_count);
    for (int i = 0; i < NMEA_SATINPACK && gpgsv->sat_data[i].id != 0; i++) {
    OS_printf("    sat %d = {\n", gpgsv->sat_data[i].id);
    OS_printf("        sig     = %d\n", gpgsv->sat_data[i].sig);
    OS_printf("        in_use  = %d\n", gpgsv->sat_data[i].in_use);
    OS_printf("        elv/azm = %d/%d\n", gpgsv->sat_data[i].elv, gpgsv->sat_data[i].azimuth);
    OS_printf("    }\n");
    }
    OS_printf("};\n");
}

void print_gpvtg(nmeaGPVTG *gpvtg) {
    OS_printf("GPVTG {\n");
    OS_printf("    dir = %lf %c\n", gpvtg->dir, gpvtg->dir_t);
    OS_printf("    dec = %lf %c\n", gpvtg->dec, gpvtg->dec_m);
    OS_printf("    spn = %lf %c\n", gpvtg->spn, gpvtg->spn_n);
    OS_printf("    spk = %lf %c\n", gpvtg->spk, gpvtg->spk_k);
    OS_printf("};\n");
}

void print_gprmc(nmeaGPRMC *gprmc) {
    OS_printf("GPRMC {\n");
    OS_printf("    utc         = %d-%d-%dT%d:%d:%d.%d\n",
            gprmc->utc.year, gprmc->utc.mon, gprmc->utc.day,
            gprmc->utc.hour, gprmc->utc.min, gprmc->utc.sec,
            gprmc->utc.hsec);
    OS_printf("    status      = %c\n",     gprmc->status);
    OS_printf("    lat         = %lf %c\n", decimal_minutes2decimal_decimal(gprmc->lat), gprmc->ns);
    OS_printf("    lon         = %lf %c\n", decimal_minutes2decimal_decimal(gprmc->lon), gprmc->ew);
    OS_printf("    speed       = %lf\n",    gprmc->speed);
    OS_printf("    direction   = %lf\n",    gprmc->direction);
    OS_printf("    declination = %lf %c\n", gprmc->declination, gprmc->declin_ew);
    OS_printf("    mode        = %c\n",     gprmc->mode);
    OS_printf("};\n");
}

