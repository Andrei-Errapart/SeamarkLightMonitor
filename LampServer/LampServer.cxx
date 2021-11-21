// vim: shiftwidth=4
// vim: ts=4
#include <stdarg.h>	// varargs.

#include <exception>
#include <stdexcept>
#include <vector>
#include <list>
#include <memory>
#include <map>
#include <iostream>	// cout

#include <utils/util.h>
#include <utils/Config.h>
#include <utils/TcpServer.h>
#include <utils/CMR.h>
#include <utils/SunRiseSet.h>

#include "LampServer.h"
#include "LampDB.h"
#include "globals.h"

/** Default ping period, seconds. */
static const int	DEFAULT_PING_PERIOD		= 120;
/** Default ping timeout, seconds. */
static const int	DEFAULT_PING_TIMEOUT	= 3 * DEFAULT_PING_PERIOD;
/** Default forward command timeout, seconds. */
static const int	DEFAULT_FORWARD_TIMEOUT	= 3 * 60;
/** Default xml emitting period, seconds. */
static const int	DEFAULT_MONITOR_PERIOD		= 24 * 3600;
/** Keepdown shutdown timer, first cycle, seconds. */
static const int	DEFAULT_KEEPDOWN_TIMER_FIRST	= 30;
/** Keepdown shutdown timer, periodic, seconds. */
static const int	DEFAULT_KEEPDOWN_TIMER_PERIODIC	= 20 * 60;

using namespace std;
using namespace utils;

/*****************************************************************************/
/*
static Option<pair<string,string> >
split2(	const std::string&	s,
		const char			sep)
{
	string::size_type	spos = s.find(sep);
	return spos==string::npos
		? Option<pair<string,string> >()
		: Option<pair<string,string> >(pair<string,string>(s.substr(0,spos), s.substr(spos+1, s.size()-spos-1)));
}
*/

/*****************************************************************************/
/** Escape chars '"' and '\\'. Escaped result is stored in \c buffer.
 */
static const char*
escape_parameter(	const string&	s,
					string&			buffer)
{
	buffer.resize(0);
	for (unsigned int i=0; i<s.size(); ++i) {
		const char	c = s[i];
		switch (c) {
		case '"':
			buffer += "\\\"";
			break;
		case '\\':
			buffer += "\\\\";
			break;
		default:
			buffer += c;
		}
	}

	return buffer.c_str();
}

/*****************************************************************************/
/**
Parse record in the form: "1=val1 2=val2 3=val3"
*/
static map<int, string>
parse_record(	const std::string&	s)
{
	map<int, string>	r;
	string				val;
	
	for (unsigned int so_far=0; so_far<s.size();) {
		const string::size_type	eqpos = s.find('=', so_far);
		if (eqpos == string::npos) {
			break;
		}
		const string::size_type	startpos = eqpos + 1;
		const string			key(s.substr(so_far, eqpos-so_far));
		string::size_type		next_so_far = startpos;
		val.resize(0);
		if (s.size()>startpos) {
			if (s[startpos] == '"') {
				const string::size_type	endpos = s.find('"', startpos+1);
				if (endpos == string::npos) {
					// error.
					throw Error("parse_record: unmatched \" for key %s in record %s", key.c_str(), s.c_str());
				} else {
					val = s.substr(startpos + 1, endpos-startpos-1);
					next_so_far = endpos + 1;
				}
			} else {
				const string::size_type	endpos = s.find(' ', startpos);
				if (endpos == string::npos) {
					val = s.substr(startpos);
					next_so_far = s.size()-1;
				} else {
					val = s.substr(startpos, endpos-startpos);
					next_so_far = endpos + 1;
				}
			}
		}
		r[int_of(key)] = val;

		while (next_so_far < s.size() && isspace(s[next_so_far])) {
			++next_so_far;
		}
		so_far = next_so_far;
	}

	return r;
}

/*****************************************************************************/
static bool
extract_field(	const map<int, string>&	record,
				const int				field_no,
				string&					field)
{
	const map<int, string>::const_iterator it = record.find(field_no);
	if (it!=record.end()) {
		field = it->second;
		return true;
	}
	return false;
}

/*****************************************************************************/
/** Float representation of hours+minutes+seconds+milliseconds. */
static const double float_of_time_only(	const my_time&	t)
{
	const double	ft = t.hour*3600 + t.minute*60 + t.second + 0.001*t.millisecond;
	return ft;
}

/*****************************************************************************/
static bool
time_is_between(	const my_time&	t,
					const my_time&	time_begin,
					const my_time&	time_end)
{
	const double	ft = float_of_time_only(t);
	const double	f1 = float_of_time_only(time_begin);
	const double	f2 = float_of_time_only(time_end);
	return f1<=ft && ft<=f2;
}

/*****************************************************************************/
/*****************************************************************************/
LampConnection::LampConnection(	LampServer&		server,
								LampDB&			db)
:	server_(server)
	,db_(db)
	,keep_down(false)
	,keepdown_shutdown_time(-1)
	,received_ping_time(timer::current_time())
	,sent_ping_time(timer::current_time())
{
	sunshine_run	= false;
}

/*****************************************************************************/
/// Log a message preceded by time in format "HHMMSS".
void LampConnection::log(			const char*		fmt, ...)
{
	// Format MSG
	string	msg;
	va_list	ap;
	va_start(ap, fmt);
	vssprintf(msg, fmt, ap);
	va_end(ap);

	// LOG
	if (id.some()) {
		// ::log("%d: %s", id(),  msg.c_str());
		::log("%s:%d (%d): %s", peer_name.c_str(), peer_port, id(), msg.c_str());
	} else {
		::log("%s:%d: %s", peer_name.c_str(), peer_port, msg.c_str());
	}
}

/*****************************************************************************/
void LampConnection::emit(	const string&	msg)
{
	std::vector<unsigned char>	buffer;
	CMR::emit(buffer, CMR::LAMPNET, msg);
	write(buffer);
	log("Command sent:%s", msg.c_str());
}

/*****************************************************************************/
void
LampConnection::emit_ping()
{
	std::vector<unsigned char>	buffer;
	CMR::emit(buffer, CMR::PING, "");
	write(buffer);
	sent_ping_time = timer::current_time();
	log("Ping sent.");
}

/*****************************************************************************/
void
LampConnection::update_sunrise_sunset(
	const my_time&	current_time
)
{
	sunrise_time = GetSunrise(position_latitude, position_longitude, current_time);
	sunset_time = GetSunset(position_latitude, position_longitude, current_time);
	sunshine_run_time_begin = sunrise_time - 1.5*3600.0;
	sunshine_run_time_end = sunset_time + 1.5*3600.0;

	if (sunshine_run) {
		log("Sunrise: %02d:%02d:%02d, sunshine run time begin: %02d:%02d:%02d",
				sunrise_time.hour,
				sunrise_time.minute,
				sunrise_time.second,
				sunshine_run_time_begin.hour,
				sunshine_run_time_begin.minute,
				sunshine_run_time_begin.second);
		log("Sunset : %02d:%02d:%02d, sunshine run time end  : %02d:%02d:%02d",
				sunset_time.hour,
				sunset_time.minute,
				sunset_time.second,
				sunshine_run_time_end.hour,
				sunshine_run_time_end.minute,
				sunshine_run_time_end.second);
	} else {
		log("Sunrise: %02d:%02d:%02d",
				sunrise_time.hour,
				sunrise_time.minute,
				sunrise_time.second);
		log("Sunset : %02d:%02d:%02d",
				sunset_time.hour,
				sunset_time.minute,
				sunset_time.second);
	}
}

/*****************************************************************************/
LampConnection::~LampConnection()
{
	if (id.some()) {
		try {
			db_.set_online(id(), false);
		} catch (const exception& e) {
			log("Exception: %s", e.what());
		}
	}
	log("Disconnected.");
}

/*****************************************************************************/
/*****************************************************************************/
void LampConnection::handle_connect()
{
	log("Connected.");
}

/*****************************************************************************/
void LampConnection::do_admin_command(
			const string&	cmd,
			const string&	op,
			const string&	params)
{
	if (stringcasecmp(cmd, "sql")==0) {
		// Execute SQL query.
		if (stringcasecmp(op, "query")==0) {
			const string&	query = params;
			log("SQL admin query=%s", query.c_str());
			std::vector<std::vector<std::string> >	results;
			try {
				const int	affected_rows = db_.admin_execute_query(query, results);
				const int	nr_of_fields = results.size()>0 ? results[0].size() : 0;
				emit(ssprintf("SQL OK 1=%d 2=%d 3=%d", results.size(), affected_rows, nr_of_fields));
				string	lbuf;
				for (unsigned int i=0; i<results.size(); ++i) {
					const std::vector<std::string>&	row = results[i];
					lbuf.resize(0);
					lbuf = "SQL ROW";
					string	colbuffer;
					for (unsigned int colIndex=0; colIndex<row.size(); ++colIndex) {
						lbuf += ssprintf(" %d=\"%s\"", colIndex+1, escape_parameter(row[colIndex], colbuffer));
					}
					emit(lbuf);
				}
			} catch (const exception& e) {
				emit(ssprintf("SQL ERROR %s", e.what()));
			}
		} else {
			emit(ssprintf("SQL ERROR Op '%s' unrecognized", op.c_str()));
		}
	} else if (stringcasecmp(cmd, "forward")==0) {
		// Forward command to the one of the connected LampControllers.
		try {
			int	lamp_id = int_of(op);
			LampConnection*	lamp = server_.find_connection(lamp_id);
			if (lamp == 0) {
			} else {
				log("Admin %d commanded lamp %d: %s", admin_id(), lamp->id(), params.c_str());
				lamp->emit(params);
				lamp->forward_admin_id = admin_id;
				lamp->forward_time = timer::current_time();
			}
		} catch (const exception& e) {
			emit(ssprintf("FORWARD ERROR Exception: %s", e.what()));
		}
	} else {
		emit(ssprintf("ERROR Command '%s' unrecognized", cmd.c_str()));
	}
}

/*****************************************************************************/
void
LampConnection::do_lamp_command(
			const string&	cmd,
			const string&	op,
			const string&	params)
{
	if (stringcasecmp(cmd, "config")==0) {
		const map<int, string>	record = parse_record(params);
		if (forward_admin_id.some() && ((timer::current_time() - forward_time) / (1000 * 1000)) < DEFAULT_FORWARD_TIMEOUT) {
			db_.lamp_insert_config(id(), forward_admin_id, record);
		} else {
			db_.lamp_insert_config(id(), utils::Option<int>(), record);
		}
	} else if (stringcasecmp(cmd, "sensorscounters")==0) {
		const map<int, string>	record = parse_record(params);
		db_.lamp_insert_sensors(id(), record);
		if (keep_down) {
			if (started_on_sunshine) {
				log("Scheduled shutdown overriden by started_on_sunshine.");
			} else {
				log("Scheduled shutdown, since got data already.");
				emit("shutdown");
			}
		}
	} else {
		log("Lamp command '%s' unrecognized", cmd.c_str());
	}
}

/*****************************************************************************/
void
LampConnection::handle_read(	const std::vector<unsigned char>&		data)
{
	received_ping_time	= timer::current_time();
	CMR		cmr;
	// log("Got %d bytes", data.size());

	cmrdecoder_.feed(data);
	while (cmrdecoder_.pop(cmr)) {
		try {
			const string	s(cmr.ToString());
			switch (cmr.type) {
			case CMR::PING:
				log("Ping received!");
				// Keep us alive.
				if (id.some()) {
					db_.set_online(id(), true);
				}
				break;
			case CMR::LAMPNET:
				log("Packet: %s", s.c_str());
				{
					// Get the query string.
					std::string	query(cmr.ToString());
					vector<string>	v3;
					split(query, " ", v3, 3);

					// Any commands?
					if (v3.size()>=3) {
						const string&	cmd = v3[0];
						const string&	op = v3[1];

						// LampController login.
						if (stringcasecmp(cmd, "lampcontroller")==0) {
							const map<int, string>	args(parse_record(v3[2]));
							string	imei;
							string	firmware_name;
							string	firmware_version;
							string	firmware_date;
							if (extract_field(args, 1, firmware_name) &&
								extract_field(args, 2, firmware_version) &&
								extract_field(args, 3, firmware_date) &&
								extract_field(args, 4, imei)) {
								bool			wake_up = false;
								const my_time	registration_time = my_time_of_now();
								const int	id = db_.register_lampcontroller(
													imei, firmware_name, firmware_version, firmware_date,
													pair_id, wake_up, keep_down, sunshine_run,
													position_latitude, position_longitude);
								log("Got ID %d for IMEI %s. Other flags: %s, %s, %s",
										id, imei.c_str(),
										(wake_up ? "WakeUp" : "!WakeUp"),
										(keep_down ? "KeepDown" : "!KeepDown"),
										(sunshine_run ? "SunshineRun" : "!SunshineRun") );

								update_sunrise_sunset(registration_time);
								if (sunshine_run) {
									log("Sunshine run time begin: %02d:%02d:%02d",
											sunshine_run_time_begin.hour,
											sunshine_run_time_begin.minute,
											sunshine_run_time_begin.second);
									log("Sunshine run time end  : %02d:%02d:%02d",
											sunshine_run_time_end.hour,
											sunshine_run_time_end.minute,
											sunshine_run_time_end.second);
								}
								started_on_sunshine = sunshine_run && time_is_between(registration_time, sunshine_run_time_begin, sunshine_run_time_end);

								if (started_on_sunshine) {
									log("Started on SUNSHINE. Yes!");
								}

								if (wake_up || started_on_sunshine) {
									emit_ping();
								}
								if (wake_up && !started_on_sunshine) {
									log("Keep down flag overriden by wake_up.");
									keep_down = false;
								}
								this->id = id;
								db_.set_online(id, true);
								if (keep_down) {
									if (started_on_sunshine) {
										log("Keep down flag overriden by started_on_sunshine.");
									} else {
										log("Keep Down flag set.");
										keepdown_shutdown_time = timer::current_time() + DEFAULT_KEEPDOWN_TIMER_FIRST*1000*1000;
									}
								}
							} else {
								log("LampController: incomplete info record.");
							}
						// LampAdmin login
						} else if (stringcasecmp(cmd, "admin")==0) {
							const map<int, string>	args(parse_record(v3[2]));
							log("ADMIN command, op=%s, %d args", op.c_str(), args.size());

							string	username;
							string	password;
							if (extract_field(args, 1, username) && extract_field(args, 2, password)) {
								admin_id = db_.id_of_admin(username, password);
								if (admin_id.some()) {
									log("Admin %s logged in.", username.c_str());
									emit("ADMIN OK");
								} else {
									log("Admin %s failed to log in.", username.c_str());
									emit("ADMIN ERROR");
								}
							}
						} else if (admin_id.some()) {
							do_admin_command(cmd, op, v3[2]);
						} else if (id.some()) {
							do_lamp_command(cmd, op, v3[2]);
						} else {
							log("Unknown packet, cmd=%s, op=%s", cmd.c_str(), op.c_str());
							// emit(ssprintf("%s ERROR Unknown command", cmd.c_str()));
						}
					} else {
						log("Unknown or incomplete query: %s", query.c_str());
					}
				}
				// Keep us alive.
				if (id.some()) {
					db_.set_online(id(), true);
				}
				break;
			default:
				log("Packet: %s", s.c_str());
				break;
			}
		} catch (const exception& e) {
			log("Exception: %s", e.what());
			// close();
			break;
		}
	}
}

/*****************************************************************************/
void
LampConnection::handle_error(	short				what)
{
	log("Error: code %d", what);
}

/*****************************************************************************/
/*****************************************************************************/
LampConnection*
LampServer::find_connection(	const int	lamp_id)
{
	// Find the lamp.
	for (list<TcpConnection*>::iterator it=listener_->connections.begin(); it!=listener_->connections.end(); ++it) {
		LampConnection*	xlamp = dynamic_cast<LampConnection*>(*it);
		if (xlamp!=0 && xlamp->id.some() && xlamp->id()==lamp_id) {
			return xlamp;
		}
	}
	return 0;
}

/*****************************************************************************/
TcpConnection*
LampServer::make_LampConnection(
		string&	peer_name,
		int		peer_port)
{
	return new LampConnection(*this, *db_);
}

/*****************************************************************************/
void
LampServer::manager_timer(	short	event)
{
	const my_time		current_mtime = my_time_of_now();
	const double		current_timeofday = float_of_time_only(current_mtime);
	const double		previous_timeofday = float_of_time_only(manager_timer_lasttime_);

	try {
		// Check receiving timeouts...
		const long_long_t	current_time = timer::current_time();
		list<TcpConnection*>	to_be_closed;
		for (list<TcpConnection*>::iterator it=listener_->connections.begin(); it!=listener_->connections.end(); ++it) {
			LampConnection*	xlamp = dynamic_cast<LampConnection*>(*it);
			if (xlamp!=0) {
				// Received ping timeout?
				const int	dt = (current_time - xlamp->received_ping_time) / (1000 * 1000);
				if (dt > DEFAULT_PING_TIMEOUT) {
					xlamp->log("Ping timeout at %d seconds, closing.", dt);
					to_be_closed.push_front(xlamp);
				}
			}
		}

		// Close the chosen ones.
		for (list<TcpConnection*>::iterator it=to_be_closed.begin(); it!=to_be_closed.end(); ++it) {
			LampConnection*	xlamp = dynamic_cast<LampConnection*>(*it);
			delete xlamp;
		}
	} catch (const exception& e) {
		log("Exception: %s", e.what());
	}

	// Update sunrise/sunset, if required.
	if (current_timeofday < previous_timeofday) {
		const string	stime(current_mtime.ToString());
		::log("LampServer: new day: %s", stime.c_str());
		::log("LampServer: updating sunrise/sunset times.");
		for (list<TcpConnection*>::iterator it=listener_->connections.begin(); it!=listener_->connections.end(); ++it) {
			LampConnection*	xlamp = dynamic_cast<LampConnection*>(*it);
			if (xlamp!=0) {
				xlamp->update_sunrise_sunset(current_mtime);
			}
		}
	}

	try {
		// Check sending periods.
		const long_long_t	current_time = timer::current_time();
		for (list<TcpConnection*>::iterator it=listener_->connections.begin(); it!=listener_->connections.end(); ++it) {
			LampConnection*	xlamp = dynamic_cast<LampConnection*>(*it);
			if (xlamp!=0) {
				// Received ping timeout?
				const int	dt = (current_time - xlamp->sent_ping_time) / (1000 * 1000);
				if (dt > DEFAULT_PING_PERIOD) {
					xlamp->emit_ping();
				}
			}
		}
	} catch (const exception& e) {
		log("Exception: %s", e.what());
	}

	try {
		// Shutdown if required.
		const long_long_t	current_time = timer::current_time();
		for (list<TcpConnection*>::iterator it=listener_->connections.begin(); it!=listener_->connections.end(); ++it) {
			LampConnection*	xlamp = dynamic_cast<LampConnection*>(*it);
			if (xlamp!=0) {
				const bool	sunshine_override =
					xlamp->started_on_sunshine &&
					xlamp->sunshine_run &&
					time_is_between(current_mtime, xlamp->sunshine_run_time_begin, xlamp->sunshine_run_time_end);
				if (xlamp->keepdown_shutdown_time<current_time && xlamp->keep_down && !sunshine_override) {
					xlamp->log("Scheduled shutdown.");
					xlamp->emit("shutdown");
					xlamp->keepdown_shutdown_time = current_time + DEFAULT_KEEPDOWN_TIMER_PERIODIC*1000*1000;
				}
			}
		}
	} catch (const exception& e) {
		log("Exception: %s", e.what());
	}

	// Check if we need to process querylines.
	{
		if (current_mtime.minute != manager_timer_lasttime_.minute) {
			set<int>	query_ids;
			try {
				db_->lamp_query_cronlines(current_mtime.minute, query_ids);
			} catch (const exception& e) {
				log("Exception: %s", e.what());
				log("Skipping queries.");
			}

			// Query sensors info, if needed.
			for (list<TcpConnection*>::iterator it=listener_->connections.begin(); it!=listener_->connections.end(); ++it) {
				LampConnection*	xlamp = dynamic_cast<LampConnection*>(*it);
				if (xlamp!=0 && xlamp->id.some() && query_ids.count(xlamp->id())>0) {
					// Time to read sensors.
					xlamp->emit("sensorcounter ?");
				}
			}
		}
	}

	// Construct XML if needed.
	try {
		my_time	time_sunrise = GetSunrise(cfg_.LocalLatitude, cfg_.LocalLongitude, current_mtime) + 3600.0;
		my_time	time_sunset	 = GetSunset(cfg_.LocalLatitude, cfg_.LocalLongitude, current_mtime) + 3600.0;
		if (monitor_update_!=MONITORUPDATE_SUNRISE && time_is_between(current_mtime, time_sunrise, time_sunset)) {
			log("Monitor update: SunRise");
			monitor_update_ = MONITORUPDATE_SUNRISE;
			monitor_time_ = current_mtime;
			db_->emit_xml(cfg_.MonitorOutput);
		} else if (monitor_update_!=MONITORUPDATE_SUNSET && current_mtime>time_sunset) {
			log("Monitor update: SunSet");
			monitor_update_ = MONITORUPDATE_SUNSET;
			monitor_time_ = current_mtime;
			db_->emit_xml(cfg_.MonitorOutput);
		} else if ((current_mtime-monitor_time_)>DEFAULT_MONITOR_PERIOD) {
			log("Monitor update: TIMEOUT");
			monitor_update_ = MONITORUPDATE_TIMEOUT;
			monitor_time_ = current_mtime;
			db_->emit_xml(cfg_.MonitorOutput);
		}
	} catch (const exception& e) {
		log("Exception: %s", e.what());
	}

	manager_timer_lasttime_ = current_mtime;
}

/*****************************************************************************/
/** Run the lampserver. */
void
LampServer::run(int argc, char** argv)
{
	// 1. Check options
	for (int i=1; i<argc; ++i) {
		if (strcmp(argv[i], "--testmonitor") == 0) {
			db_ = auto_ptr<LampDB>(new LampDB(cfg_.DatabaseName, cfg_.DatabaseUser, cfg_.DatabasePassword, cfg_.SkipInit));
			db_->emit_xml("");
			return;
		}
	}
	
	// Print configuration.
	cout << "Configuration:" << endl;
	cout << cfg_.ToString() << endl;

	// Database.
	cout << "Connecting to database." << endl;
	db_ = auto_ptr<LampDB>(new LampDB(cfg_.DatabaseName, cfg_.DatabaseUser, cfg_.DatabasePassword, cfg_.SkipInit));

	// Open main socket.
	cout << "Opening port " << cfg_.LocalPort << " for listening." << endl;
	listener_ = auto_ptr<TcpPortListener>(
			new TcpPortListener(
				makeFunctor((TcpPortListener::NewConnectionCallback*)0, *this, &LampServer::make_LampConnection),
				cfg_.LocalPort));

	// Timers.
	my_time_of_now(manager_timer_lasttime_);
	manager_timer_ = auto_ptr<Timer>(new Timer(makeFunctor((Timer::TimerCallback*)0, *this, &LampServer::manager_timer), Timer::PERIODIC, 10.0));

	monitor_time_ = my_time_of_now();
	monitor_update_ = MONITORUPDATE_STARTUP;
	try {
		db_->emit_xml(cfg_.MonitorOutput);
	} catch (const exception& e) {
		log("Error writing XML: %s", e.what());
	}

	// Run until shutdown.
	cout << "Entering event loop." << endl;
	TcpPortListener::event_loop();
}

