/* OS-Build. Copyright (C) 2017, 2018. Ben Watts-Jones.
 *
 * This program is distributed under the GNU General Public License Version 3 
 * (GPLv3) as published by the Free Software Foundation. You should have 
 * recieved a copy of the GPLv3 licence along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MECHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GPLv3 license for more details.
 */

#include <stdint.h>

#include "atapi.h"
#include "../cpu/ioports.h"
#include "screen.h"
#include "pci.h"

/* CD (and DVD/Blu-ray) drives are traditionally attached via the ATA (IDE)
   bus. Optical drives are communicated with using SCSI command over ATA,
   a protocol known as ATAPI. This communication used to be parralell (PATAPI)
   but is now commonly serial (SATAPI), with the optical disk connected via a
   SATA port. Further variation comes from the fact that this is also done
   typically via the PCI (or PCIe) bus. Fortunately, all such devices all such
   devices may be communicated with using ATAPI. (Although note actual hardware
   may be non-standard, or attached indirectly via USB or a raid card).

   ATA supports up to four devices: there is a master and slave bus, each of
   which supports a primary and secondary drive. These devices are communicated
   with using IO ports, which are defined in 'atapi.h'.

   The ATAPI commands often appear long and arbitary, as such they are best
   looked up on the osdev wiki.
*/

void resetATA(uint32_t bus)
{
    portByteOut(ATA_REG_DCR_ACR(bus), ATA_DCR_SOFTWARE_RESET);
    ATA_DRIVE_SWITCH_DELAY(bus);
    portByteOut(ATA_REG_DCR_ACR(bus), 0);
    ATA_DRIVE_SWITCH_DELAY(bus);
}

// Note this function is not being used currently.
uint32_t isDrivePacketDevice(uint32_t bus)
{
    uint32_t ret = 1;
    resetATA(bus);
    uint8_t sig1 = portByteIn(ATA_REG_SECTOR_COUNT(bus));
    uint8_t sig2 = portByteIn(ATA_REG_ADDR_LOW(bus));
    uint8_t sig3 = portByteIn(ATA_REG_ADDR_MID(bus));
    uint8_t sig4 = portByteIn(ATA_REG_ADDR_HIGH(bus));
    if (sig1 == 0x01 && sig2 == 0x01 && sig3 == 0x00 && sig4 == 0x00) {
        printk("Non-packet device detected\n\0");
    } else if (sig1 == 0x01 && sig2 == 0x01 && sig3 == 0x14 && sig4 == 0xeb) {
        printk("Packet device detected\n\0");
    } else {
        printk("No valid packet data recorded\n\0");
        ret = 0;
    }
    printk("Device packet data: %x %x %x %x\n\0", sig1, sig2, sig3, sig4);
    return ret;
}

enum AtaReturnCodes identifyDrive(uint32_t bus, uint32_t drive)
{
    portByteOut(ATA_REG_DRIVE(bus), drive);
    ATA_DRIVE_SWITCH_DELAY(bus);
    portByteOut(ATA_REG_SECTOR_COUNT(bus), 0);
    portByteOut(ATA_REG_ADDR_LOW(bus), 0);
    portByteOut(ATA_REG_ADDR_MID(bus), 0);
    portByteOut(ATA_REG_ADDR_HIGH(bus), 0);
    portByteOut(ATA_REG_COMMAND_STATUS(bus), ATA_COMMAND_IDENTIFY);
    uint8_t status = portByteIn(ATA_REG_COMMAND_STATUS(bus));
    if (status == 0)
        return ERROR;
    uint8_t status_mid = portByteIn(ATA_REG_ADDR_MID(bus));
    uint8_t status_high = portByteIn(ATA_REG_ADDR_HIGH(bus));
    if (status_mid == 0 && status_high == 0) {
        return ATA_DRIVE;
    } else if (status_mid == 0x14 && status_high == 0xeb) {
        return ATAPI_DRIVE;
    } else if (status_mid == 0x3c && status_high == 0xc3) {
        return SATA_DRIVE;
    } else {
        return UNKNOWN_DRIVE;
    }
}

enum AtaReturnCodes checkATABus(uint32_t bus)
{
    resetATA(ATA_BUS_ADDR_PRIMARY);
    uint8_t ata_status = portByteIn(ATA_REG_COMMAND_STATUS(bus));
    return (ata_status == FLOATING_BUS) ? NO_DRIVE : DRIVE_EXISTS;
}

enum AtaReturnCodes findATAPI()
{
    if (checkATABus(ATA_BUS_ADDR_PRIMARY) == DRIVE_EXISTS) {
        if (identifyDrive(ATA_BUS_ADDR_PRIMARY, ATA_DRIVE_MASTER) == ATAPI_DRIVE)
            return PRIMARY_MASTER;
        if (identifyDrive(ATA_BUS_ADDR_PRIMARY, ATA_DRIVE_SLAVE) == ATAPI_DRIVE)
            return PRIMARY_SLAVE;
    }
    if (checkATABus(ATA_BUS_ADDR_SECONDARY) == DRIVE_EXISTS) {
        if (identifyDrive(ATA_BUS_ADDR_SECONDARY, ATA_DRIVE_MASTER) == ATAPI_DRIVE)
            return SECONDARY_MASTER;
        if (identifyDrive(ATA_BUS_ADDR_SECONDARY, ATA_DRIVE_SLAVE) == ATAPI_DRIVE)
            return SECONDARY_SLAVE;
    }
    return NO_DRIVE;
}

void initializeATAPI()
{
    if (findATAPI() != NO_DRIVE) printk("found atapi drive!\n\0");
}
    
