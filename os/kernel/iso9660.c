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

#include "iso9660.h"

/* I have chosen iso9660 as the file system to implement as it is the default 
 * for booting from CDs/DVDs, also works with USB sticks, and is simple. A file
 * system must be implemented before we can enter 'user space' and perform 
 * process management, as these processes are initiated (in part) by running
 * files.
 *
 * Note that iso9660 filenames may only contain uppercase A-Z, underscores,
 * and digits. There is also one dot for the file type and one semicolon
 * (hidden) for the version number. However, some OSs do not implement this
 * correctly.
 *
 * 
 */


// This file is to contain functions for interpreting data read from disk
// (not yet implemented)
