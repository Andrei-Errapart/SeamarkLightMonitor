/**
vim: shiftwidth=4
vim: ts=4
*/
#include <vector>
#include <string>


/*****************************************************************************/
/** Sever configuration. */
class LampConfig {
public:
	int								LocalPort;
	std::vector<std::pair<std::string, int> >	OtherServers;
	std::string						DatabaseName;
	std::string						DatabaseUser;
	std::string						DatabasePassword;
	bool							SkipInit;
	std::string						MonitorOutput;
	double							LocalLatitude;
	double							LocalLongitude;
	///< Powerlight update frequency, minutes.
	int								PowerlightUpdateFrequency;
public:
	LampConfig();
	std::string ToString() const;
}; // class LampConfig

