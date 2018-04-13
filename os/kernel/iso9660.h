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

// Note: 'extent' = contiguous area of storage reserved for a file in a file system.
// Note: macros are used for initializing default struct values (instead of
// structs) because of the following features of C:
//  - struct in C are never const, even if declared as 'const' (NOT true in C++)
//  - expressions in an initializer of object with static storage duration
//    (e.g global variables) must be constant expressions or literals.

#ifndef KERNEL_ISO9660_H
#define KERNEL_ISO9660_H

#include <stdint.h>

struct Iso9660DirectoryEntry {
    uint8_t directory_record_length;
    uint8_t attribute_record_length;
    uint32_t extent_location_LSB;
    uint32_t extent_location_MSB;
    uint32_t extent_size_LSB;
    uint32_t extent_size_MSB;
    uint8_t years_elapsed_1990;
    uint8_t month;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t timezone;
    uint8_t file_flags;
    uint8_t interleave_gap;
    uint16_t volume_sequence_LSB;
    uint16_t volume_sequence_MSB;
    uint8_t file_name_length;
    char file_name[1];
} __attribute__((packed));

#define ISO9660_DIRECTORY_ENTRY_DEFAULT {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,{0}}

struct Iso9660Time {
    char year[4];
    char month[2];
    char day[2];
    char hour[2];
    char min[2];
    char sec[2];
    char hundredth_sec[2];
    char timezone;
} __attribute__((packed));

#define ISO9660_TIME_DEFAULT {{0,0,0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, 0}

#define ISO9660_VOLUME_TYPE_BOOT 0
#define ISO9660_VOLUME_TYPE_PRIMARY_DESC 1
#define ISO9660_VOLUME_TYPE_SUPPLIMENTARY_DESC 2
#define ISO9660_VOLUME_TYPE_PARTITION_DESC 3
#define ISO9660_VOLUME_TYPE_TERMINATOR 255

struct Iso9660PrimaryVolumeDescriptor {
    uint8_t type;
    char identifier[5];
    uint8_t version;
    uint8_t zeros1;
    char system_identifier[32];
    char volume_identifier[32];
    uint8_t zeros2[8];
    uint32_t volume_space_size_LSB;
    uint32_t volume_space_size_MSB;
    uint8_t zeros3[32];
    uint16_t volume_set_size_LSB;
    uint16_t volume_set_size_MSB;
    uint16_t volume_sequence_number_LSB;
    uint16_t volume_sequence_number_MSB;
    uint16_t logical_block_size_LSB;
    uint16_t logical_block_size_MSB;
    uint32_t path_table_size_LSB;
    uint32_t path_table_size_MSB;
    uint32_t lpath_table_location;
    uint32_t lpath_optional_table_location;
    uint32_t mpath_table_location_MSB;
    uint32_t mpath_optional_table_location_MSB;
    struct Iso9660DirectoryEntry root;
    char volume_set_identifier[128];
    char publisher_identifier[128];
    char data_preparer_identifier[128];
    char application_identifier[128];
    char copyright_file_identifier[38];
    char abstract_file_identifier[26];
    char bibliographic_file_id[36];
    struct Iso9660Time creation_time;
    struct Iso9660Time modification_time;
    struct Iso9660Time expiration_time;
    struct Iso9660Time effective_time;
    uint8_t file_structure_version;
    uint8_t zeros4;
} __attribute__((packed));

#define ISO9660_PRIMARY_DESCRIPTOR_DEFAULT = { \
    0x01, "CD001", 0x01, 0, {0}, {0}, {0}, 0, 0, \
    {0}, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, ISO9660_ROOT_DIRECTORY_ENTRY_DEFAULT, \
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, \
    ISO9660_TIME_DEFAULT, ISO9660_TIME_DEFAULT, ISO9660_TIME_DEFAULT, \
    ISO9660_TIME_DEFAULT, 0x01, 0 \
}

#endif
