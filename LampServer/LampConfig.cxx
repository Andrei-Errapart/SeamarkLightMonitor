/**
vim: shiftwidth=4
vim: ts=4
*/
#include <stdarg.h>	// varargs.

#include <exception>
#include <stdexcept>
#include <vector>
#include <string>

#include <utils/util.h>
#include <utils/Config.h>
#include <utils/CMR.h>

#include "LampConfig.h"

static const int	DEFAULT_SERVER_PORT		= 8080;

/** Name of the configuration file. */
static const char*	CONFIGURATION_FILENAME	= "LampServer.ini";

using namespace std;
using namespace utils;

/*****************************************************************************/
/*****************************************************************************/
LampConfig::LampConfig()
{
	// Load the file.
	FileConfig	cfg(CONFIGURATION_FILENAME);
	cfg.load();
	cfg.set_section("LampServer");

	// Load the values.
	bool ok = true;
	// LocalPort
	ok = ok && cfg.get_int("LocalPort", LocalPort);
	// OtherServers
	{
		string			sbuf;
		ok = ok && cfg.get_string("OtherServers", sbuf);
		if (ok) {
			vector<string>	v;
			split(sbuf, " ", v);
			for (unsigned int i=0; i<v.size(); ++i) {
				string&					s = v[i];
				const string::size_type	colonpos = s.find(':');
				if (colonpos == string::npos) {
					OtherServers.push_back(pair<string,int>(s, DEFAULT_SERVER_PORT));
				} else {
					const int	portno = int_of(s.substr(colonpos+1));
					OtherServers.push_back(pair<string,int>(s.substr(0, colonpos), portno));
				}
			}
		}
	}
	// SkipInit
	ok = ok && cfg.get_bool("SkipInit",			SkipInit);
	// MonitorOutput
	ok = ok && cfg.get_string("MonitorOutput",	MonitorOutput);
	// Local position.
	ok = ok && cfg.get_double("LocalLatitude",	LocalLatitude);
	ok = ok && cfg.get_double("LocalLongitude",	LocalLongitude);
	ok = ok && cfg.get_int("PowerlightUpdateFrequency",	PowerlightUpdateFrequency);

	// Database stuff.
	cfg.set_section("Database");
	ok = ok && cfg.get_string("Database",		DatabaseName);
	ok = ok && cfg.get_string("Username",		DatabaseUser);
	ok = ok && cfg.get_string("Password",		DatabasePassword);

	if (!ok) {
		throw runtime_error("Configuration file incomplete or invalid.");
	}
}

/*****************************************************************************/
string
LampConfig::ToString() const
{
	std::string	r;
	r += "[LampServer]\n";
	// LocalPort
	r += ssprintf("LocalPort=%d\n", LocalPort);
	// OtherServers
	{
		string	s;
		for (unsigned int i=0; i<OtherServers.size(); ++i) {
			if (i>0) {
				s += " ";
			}
			const pair<string,int>& os = OtherServers[i];
			s += ssprintf("%s:%d", os.first.c_str(), os.second);
		}
		r += "OtherServers=";
		r += s;
		r += '\n';
	}
	// SkipInit
	r += ssprintf("SkipInit=%s\n", SkipInit ? "true" : "false");
	// MonitorOutput
	r += ssprintf("MonitorOutput=%s\n", MonitorOutput.c_str());
	r += ssprintf("LocalLatitude=%g\n", LocalLatitude);
	r += ssprintf("LocalLongitude=%g\n", LocalLongitude);
	r += ssprintf("PowerlightUpdateFrequency=%d\n", PowerlightUpdateFrequency);

	// Database.
	r += "[Database]\n";
	r += ssprintf("Database=%s\n", DatabaseName.c_str());
	r += ssprintf("User=%s\n", DatabaseUser.c_str());
	r += ssprintf("Password=%s\n", DatabasePassword.c_str());
	return r;
}

