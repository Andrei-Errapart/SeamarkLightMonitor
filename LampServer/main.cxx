// vim: shiftwidth=4
// vim: ts=4
#include <exception>
#include <stdexcept>

#include "LampServer.h"

using namespace std;
using namespace utils;

/*****************************************************************************/
/** Set up the port and start running FAST.
 */
int
main(	int	argc,
	char**	argv)
{
	cout << "Startup." << endl;
	try {
		LampServer	lamp_server;
		lamp_server.run(argc, argv);
	} catch (const std::exception& e) {
		cout << "Error: " << e.what() << endl;
		cout << "Exiting." << endl;
	}
	return 0;
}

