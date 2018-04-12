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

#define MAX_DESCRIPTORS 3
#define DEF_CODE_FLAGS 0x9a
#define DEF_DATA_FLAGS 0x92
#define DEF_GRANULARITY 0xc0

/* This file redifines the GDT after paging but before enabling interrupts.
 * This is neccessary as the code selector is stored in the IDT, but the
 * code selector stored in the pre-existing GDT initialized in the bootloader
 * is not at a valid address after paging is implemented and the identity 
 * mapping of the first 4MiB unmapped. Thus the GDT must be re-initialized,
 * with the GDTR being passed a virtual address.
 * For details of the GDT, see the original implementation in os/boot/gdt.asm.
 */

struct GDTDescriptor {
    uint16_t segment_limit;
    uint16_t base_address_low;
    uint8_t base_address_mid;
    uint8_t bit_flags;
    uint8_t granularity_flags;
    uint8_t base_address_high;
} __attribute__((packed));

struct GDTRegister {
    uint16_t gdt_size;
    void *gdt_base_address;
} __attribute__ ((packed));

static struct GDTDescriptor _gdt[MAX_DESCRIPTORS];
//static struct GDTRegister __attribute__((used)) _gdtr = {
//    (sizeof(struct GDTDescriptor) * MAX_DESCRIPTORS) - 1,
//    (uint32_t)&_gdt[0]
//};

static void installGDT()
{
    struct GDTRegister _gdtr = {
        (sizeof(struct GDTDescriptor) * MAX_DESCRIPTORS) - 1,
        (void *)&_gdt[0]
    };
    __asm__ __volatile__ ("cli" : : : "memory"); // Not really neccessary as done by boot anyway
    __asm__ __volatile__ ("lgdt %0" : : "m"(_gdtr) : "memory");
}

void setGDTDescriptor(uint32_t i, uint32_t base, uint32_t limit, uint8_t flags, 
                      uint8_t granularity)
{
    if (i > MAX_DESCRIPTORS)
        return;
    _gdt[i].base_address_low = base & 0xffff;
    _gdt[i].base_address_mid = (base >> 16) & 0xff;
    _gdt[i].base_address_high = (base >> 24) & 0xff;
    _gdt[i].segment_limit = limit & 0xffff;
    _gdt[i].bit_flags = flags;
    _gdt[i].granularity_flags = (limit >>16) & 0x0f;
    _gdt[i].granularity_flags |= granularity & 0xf0;
}

void initializeGDT() 
{
//    _gdtr.gdt_size = (sizeof(struct GDTDescriptor) * MAX_DESCRIPTORS) - 1;
//    _gdtr.gdt_base_address = (uint32_t)&_gdt[0];

    // Set null descriptor
    setGDTDescriptor(0, 0, 0, 0, 0);
    // Set default code descriptor
    setGDTDescriptor(1, 0, 0xffffffff, DEF_CODE_FLAGS, DEF_GRANULARITY);
    // Set default data descriptor
    setGDTDescriptor(2, 0, 0xffffffff, DEF_DATA_FLAGS, DEF_GRANULARITY);

    installGDT();
}

