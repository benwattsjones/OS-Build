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

#include "pci.h"
#include "../cpu/ioports.h"
#include "screen.h"

#define MAX_BUS 256
#define MAX_DEVICE 32
#define INVALID_VENDOR 0xffff
#define HEADER_FUNCTIONS_BIT 0x80
#define MAX_FUNCTIONS 8
#define CONFIG_ADDRESS 0xcf8
#define CONFIG_DATA 0xcfc

/* PCI has 256 busses. Each of which may connect up to 32 hardware devices (e.g. a
   raid card, a USB controller...). Each device may have up to 8 separately
   addressable functions. Thus a PCI device is addressed using three values:
   bus, device and function (0 if a device has only one function).

   Each function/device must have atleast 16 four byte (little edian) registers.
   These registers are addressed by their offset (0x00 - 0x3C). This memory is
   known as 'configuration space'. 

   CPUs cannot directly communicate with PCI configuration space. Such 
   functionality is typically provided by the host bridge. The protocol used
   here is to select the configuration space desired using IO port 0xcf8,
   the 'CONFIG_ADDRESS' port, and reading/writing to the registers using
   port 0xcfc, the 'CONFIG_DATA' port. PCIe also supports this mechanism, 
   though introduces the faster alternative of memory mapped configuration
   space.

   The meaning of the different registers in configuration space differes with
   different devices, but starts the same, providing a number of register
   values. (See osdev wiki for full details)
    - Vendor ID (offset 0) - each vendor is assigned a value. 0xffff is invalid,
      and may be used to determine whether a device is attached or not.
    - Class, subclass, program interface - determines the device type.
    - Header - determines the layout of the remaining configuration space,
      and whether the device has multiple functions.

   One may thus identify all PCI devices by enumerating across all busses and
   possible device values, checking Vendor ID and Header to verify the devices
   existance and need to check multiple functions.
*/

uint32_t getPCIInfo(uint32_t bus,  uint32_t device, uint32_t func, uint32_t reg)
{
    uint32_t address = (1 << 31) | (bus << 16) | (device << 11) | (func << 8) | (reg & 0xfc);
    portDoubleOut(CONFIG_ADDRESS, address);
    return portDoubleIn(CONFIG_DATA);
}

void displayPCIDeviceInfo(uint32_t bus, uint32_t device, uint32_t func, uint32_t header)
{
    uint32_t class_data = getPCIInfo(bus, device, func,  0x08);
    uint32_t prog_if = (class_data >> 8) & 0xff;
    uint32_t subclass = (class_data >> 16) & 0xff;
    uint32_t class = (class_data >> 24) & 0xff;
    if (class != 0xff && class == 0x01 && subclass == 0x01) {
        printk("Bus %x, Dev %x, func %x, Head %x, Class %x, Sub %x, Prog IF %x\n\0",
               bus, device, func, header, class, subclass, prog_if);
    }
}

void enumeratePCI()
{
    uint32_t bus, device, header, func, vendor_id;
    for (bus = 0; bus < MAX_BUS; bus++) {
        for (device = 0; device < MAX_DEVICE; device++) {
            vendor_id = (getPCIInfo(bus, device, 0, 0) & 0xffff);
            if (vendor_id == INVALID_VENDOR)
                continue;
            header = (getPCIInfo(bus, device, 0, 0x0c) >> 16) & 0xff;
            if (header & HEADER_FUNCTIONS_BIT) {
                for (func = 0; func < MAX_FUNCTIONS; func++) {
                    displayPCIDeviceInfo(bus, device, func, header);
                }
            } else {
                displayPCIDeviceInfo(bus, device, 0, header);
            }
        }
    }
}

