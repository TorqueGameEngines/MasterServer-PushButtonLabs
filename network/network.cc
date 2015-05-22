/*
	(c) Ben Garney <bgarney@pblabs.com> 2002-2003

    This file is part of the Pushbutton Master Server.

    PMS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the PMS; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/**
 * @brief Basic networking layer
 *
 */

#include "internal.h"
#include "network.h"
#include <time.h>

/**
 * @brief Initialize network library.
 *
 * Called by users of the library to make sure it's set up properly.
 */
void initNetworkLib()	// Initialize the library.
{
	netInit(0,0);
}

/**
 * @brief Destroy network library.
 *
 * Called when the library needs to be broken down.
 */
void killNetworkLib()	// Shut us down.
{
}

/**
 * @brief Get the current time in seconds.
 *
 * Remnants of a braindead kludge to use UL's timer code.
 *
 * Still good in case you have to do a work around on time.
 *
 * @return current time in seconds.
 */
int getAbsTime()
{
	return (int)time(NULL);
}

/**
 * @brief Sleep for the specified number of milliseconds.
 *
 */
void millisleep(int delay)
{
	ulMilliSecondSleep(delay);
}
