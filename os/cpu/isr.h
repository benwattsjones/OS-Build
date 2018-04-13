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

#ifndef CPU_ISR_H
#define CPU_ISR_H

void handleInterruptDefault();

void handleInterruptTimer();
void handleInterruptKeyboard();
void handleInterruptPrimaryATA();
void handleInterruptSecondaryATA();
void handleHardwareInterrupts_low();
void handleHardwareInterrupts_high();

void handleException0();
void handleException1();
void handleException2();
void handleException3();
void handleException4();
void handleException5();
void handleException6();
void handleException7();
void handleException8();
void handleException9();
void handleException10();
void handleException11();
void handleException12();
void handleException13();
void handleException14();
void handleException15();
void handleException16();
void handleException17();
void handleException18();
void handleException19();
void handleException20();
void handleException21();
void handleException22();
void handleException23();
void handleException24();
void handleException25();
void handleException26();
void handleException27();
void handleException28();
void handleException29();
void handleException30();
void handleException31();

#endif
