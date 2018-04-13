#ifndef DRIVERS_ATAPI_H
#define DRIVERS_ATAPI_H

#include "../cpu/ioports.h"

// The starting addresses of the memory mapped IO for the primary and
// secondary ATA controllers. Valid values for "bus" parameter.
#define ATA_BUS_ADDR_PRIMARY 0x1f0
#define ATA_BUS_ADDR_SECONDARY 0x170

// Valid values of the drive register and "drive" parameter for master and
// slave drives on each bus.
#define ATA_DRIVE_MASTER 0xa0
#define ATA_DRIVE_SLAVE 0xb0

// Memory mapped IO port definitions, indexed relative to "bus" parameter.
// If different names for write/read, the write name is summerised first.
#define ATA_REG_DATA(x)           (x)
#define ATA_REG_FEATURES_ERROR(x) (x+1)
#define ATA_REG_SECTOR_COUNT(x)   (x+2)
#define ATA_REG_ADDR_LOW(x)       (x+3)
#define ATA_REG_ADDR_MID(x)       (x+4)
#define ATA_REG_ADDR_HIGH(x)      (x+5)
#define ATA_REG_DRIVE(x)          (x+6)
#define ATA_REG_COMMAND_STATUS(x) (x+7)
#define ATA_REG_DCR_ACR(x)        (x+0x206)

// 400ns delay required after switching drives. Can be achieved by reading
// the alternate status register (ATA_REG_DCR_STATUS) four times and ignoring 
// the result.
#define ATA_DRIVE_SWITCH_DELAY(bus) {portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus)); \
                                     portByteIn(ATA_REG_DCR_ACR(bus));}

#define ATA_ACR_RESET 0x04

#define ATAPI_COMMAND_IDENTIFY 0xa1 
#define ATA_COMMAND_IDENTIFY 0xec

#define ATA_STATUS_BUSY 0x80
#define ATA_STATUS_ERR 0x01

void initializeATAPI();

#endif