// vim: shiftwidth=4
// vim: ts=4

#include "globals.h"


#include <stdarg.h>	// varargs.

#include <string>
#include <utils/util.h>

using namespace std;
using namespace utils;

/*****************************************************************************/
/// Log a message preceded by time in format "HHMMSS".
void
log(			const char*		fmt, ...)
{
	my_time	rtime;
	my_time_of_now(rtime);

	cout << ssprintf("%02d%02d%02d: ", rtime.hour, rtime.minute, rtime.second);

	va_list	ap;
	va_start(ap, fmt);
	cout << vssprintf(fmt, ap) << endl << flush;
	va_end(ap);
}

