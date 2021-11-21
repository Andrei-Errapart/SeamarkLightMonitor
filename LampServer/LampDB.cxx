// vim: shiftwidth=4
// vim: ts=4

#include <stdexcept>
#include <vector>
#include <string>

#include <iostream> // cout, ofstream

#include <utils/util.h>
#include <utils/hxio.h>

#include <mysql.h>

#include <unistd.h>
#include <assert.h>

#include "LampDB.h"
#include "globals.h"

using namespace std;
using namespace utils;

/*****************************************************************************/
/** Execute mysql_real_query, throws exceptions on errors.
 */
static void
do_query(
	void*			mysql,
	const char*		func_name,
	const string&	query
)
{
	MYSQL*	sql = reinterpret_cast<MYSQL*>(mysql);
	// cout << "do_query:" << func_name << ":" << query << endl;
	const int	r = mysql_real_query(sql, query.c_str(), query.size());
	if (r != 0) {
		string	error_buffer;
		ssprintf(error_buffer, "%s: Invalid query \"%s\". Error: %s.",
			func_name, query.c_str(), mysql_error(sql) );
		throw runtime_error(error_buffer);
	}
}

/*****************************************************************************/
/**
Query just 1 row from the database. FIXME: error handling. 
*/
static bool
query1row(	void*			mysql,
			const string&	query,
			vector<string>&	row)
{
	bool	r = false;
	MYSQL*	sql = reinterpret_cast<MYSQL*>(mysql);
	do_query(mysql, __FUNCTION__, query);
	MYSQL_RES*	result = mysql_store_result(sql);
	if (result == 0) {
		throw runtime_error(ssprintf("query1row: Mysql misbehaved on query %s", query.c_str()));
	} else {
		MYSQL_ROW	mysql_row = mysql_fetch_row(result);
		if (mysql_row != 0) {
			const int	ncols = mysql_num_fields(result);
			row.resize(ncols);
			for (unsigned int i=0; i<row.size(); ++i) {
				const char*	el = mysql_row[i];
				if (el!=0) {
					row[i] = el;
				}
			}
			r = true;
		}
		mysql_free_result(result);
	}
	// unsuccessful.
	return r;
}

/*****************************************************************************/
/* Query just one field from the first results row. */
static bool
query11(	void*			mysql,
			const string&	query,
			string&			result)
{
	vector<string>	row;
	if (query1row(mysql, query, row)) {
		result = row[0];
		return true;
	}
	return false;
}

/*****************************************************************************/
int
querytable(	void*									mysql,
			const std::string&						query,
			std::vector<std::vector<std::string> >&	results)
{
	MYSQL*	sql = reinterpret_cast<MYSQL*>(mysql);
	// Execute the query.
	do_query(mysql, __FUNCTION__, query);
	results.resize(0);
	MYSQL_RES*	result = mysql_store_result(sql);
	if (result == 0) {
		// There were no results.
		return mysql_affected_rows(sql);
	} else {
		// Fetch the results.
		const int	ncols = mysql_num_fields(result);
		MYSQL_ROW	mysql_row;
		while ((mysql_row = mysql_fetch_row(result))!=0) {
			results.resize(results.size()+1);
			vector<string>&	row = results[results.size()-1];
			row.resize(ncols);
			for (unsigned int i=0; i<row.size(); ++i) {
				const char*	cell = mysql_row[i];
				row[i] = cell==0 ? "" : cell;
			}
		}
		mysql_free_result(result);
		return 0;
	}
	assert(0);
	return -1;
}
/*****************************************************************************/
/** Execute an insert query.

  \return ID of the inserted row.

  Throws exceptions on errors.
*/
static int
execute_insert(	void*			mysql,
				const string&	query)
{
	MYSQL*	sql = reinterpret_cast<MYSQL*>(mysql);

	// Execute the query.
	do_query(mysql, __FUNCTION__, query);
	// Get the ID.
	return mysql_insert_id(sql);
}

/*****************************************************************************/
typedef enum {
	UPDATE_NULL_THROW,
	UPDATE_NULL_OK
} UPDATE_FLAGS;
/**
Update just 1 row.
*/
static bool
update1row(	void*				mysql,
			const string&		query,
			const UPDATE_FLAGS	flags = UPDATE_NULL_THROW)
{
	MYSQL*	sql = reinterpret_cast<MYSQL*>(mysql);
	do_query(mysql, __FUNCTION__, query);
	const int	nrows = mysql_affected_rows(sql);
	if (flags == UPDATE_NULL_THROW ? (nrows != 1) : (nrows!=1 && nrows!=0)) {
		throw runtime_error(ssprintf("update1row: Updated %d rows instead of just 1, query: %s", nrows, query.c_str()));
	}
	return true;
}

/*****************************************************************************/
static string
escape_string(	void*			mysql,
				const string&	s)
{
	MYSQL*	sql = reinterpret_cast<MYSQL*>(mysql);
	string	buffer;

	buffer.resize(2*s.size()+1);
	const unsigned long	l = mysql_real_escape_string(sql, &buffer[0], &s[0], s.size());
	buffer.resize(l);

	return buffer;
}

/*****************************************************************************/
LampDB::LampDB(			const std::string&	database,
						const std::string&	username,
						const std::string&	password,
						const bool			skip_init)
:
	mysql_(0)
{
	// mysql_library_init(0, 0, 0);

	mysql_ = mysql_init(0);
	if (mysql_ == 0) {
		// mysql_library_end();
		throw runtime_error("MySQL library error.");
	}

	if (mysql_real_connect(reinterpret_cast<MYSQL*>(mysql_), "localhost", username.c_str(), password.c_str(), database.c_str(), 0, 0, 0) == 0) {
		mysql_close(reinterpret_cast<MYSQL*>(mysql_));
		mysql_ = 0;
		// mysql_library_end();
		throw runtime_error("Unable to connect MySQL server.");
	}

	// Clear all online status.
	if (!skip_init) {
		string		query("Update Lamp set online='N'");
		MYSQL*		sql = reinterpret_cast<MYSQL*>(mysql_);
		const int	r = mysql_real_query(sql, query.c_str(), query.size());
		if (r != 0) {
			mysql_close(sql);
			mysql_ = 0;
			// mysql_library_end();
			throw runtime_error("Failed to reset lamps online status to 'N'");
		}
	}
}

/*****************************************************************************/
/*****************************************************************************/
LampDB::~LampDB()
{
	if (mysql_ != 0) {
		mysql_close(reinterpret_cast<MYSQL*>(mysql_));
		mysql_ = 0;
		// mysql_library_end();
	}
}

/*****************************************************************************/
static bool
query_lamp(
	void*			mysql,
	const string&	imei,
	int&			id,
	Option<int>&	pair_id,
	bool&			wake_up,
	bool&			keep_down,
	bool&			sunshine_run,
	double&			position_latitude,
	double&			position_longitude
)
{
	string	imeibuffer(escape_string(mysql, imei));
	vector<string>	lampinfo;
	vector<string>	lampinfo_pair;
	if (query1row(mysql, ssprintf("SELECT id,PairID,WakeUp,KeepDown,SunshineRun,PositionLatitude,PositionLongitude FROM Lamp WHERE imei='%s'", imeibuffer.c_str()), lampinfo )) {
		id				= int_of(lampinfo[0]);
		pair_id			= lampinfo[1].size()>0 ? int_of(lampinfo[1]) : Option<int>();
		wake_up			= stringcasecmp(lampinfo[2], "Y") == 0;
		keep_down		= stringcasecmp(lampinfo[3], "Y") == 0;
		sunshine_run	= stringcasecmp(lampinfo[4], "Y") == 0;
		position_latitude	= double_of(lampinfo[5]);
		position_longitude	= double_of(lampinfo[6]);
		// Use lower-numbered 
		if (pair_id.IsValid() && pair_id()<id && query1row(mysql, ssprintf("SELECT PositionLatitude,PositionLongitude FROM Lamp WHERE id='%d'", pair_id()), lampinfo_pair) && lampinfo_pair.size()>0) {
			::log("lamp %d using latitude/longitude of %d", id, pair_id());
			position_latitude	= double_of(lampinfo_pair[0]);
			position_longitude	= double_of(lampinfo_pair[1]);
		}
		return true;
	}
	return false;
}

/*****************************************************************************/
int
LampDB::register_lampcontroller(
	const std::string&	imei,
	const std::string&	firmware_name,
	const std::string&	firmware_version,
	const std::string&	firmware_build_date,
	utils::Option<int>&	pair_id,
	bool&				wake_up,
	bool&				keep_down,
	bool&				sunshine_run,
	double&				position_latitude,
	double&				position_longitude
)
{
	int	id = -1;
	if (!query_lamp(mysql_, imei, id, pair_id, wake_up, keep_down, sunshine_run, position_latitude, position_longitude)) {
		// create the ID.
		string	imeibuffer(escape_string(mysql_, imei));
		id = execute_insert(
				mysql_,
				ssprintf("INSERT into Lamp (IMEI) VALUES ('%s')", imeibuffer.c_str())
				);
		if (!query_lamp(mysql_, imei, id, pair_id, wake_up, keep_down, sunshine_run, position_latitude, position_longitude)) {
			throw runtime_error("register_lampcontroller: new lamp registration failure.");
		}
	}

	// Update firmware info.
	string	firmware_name_buffer(		escape_string(mysql_, firmware_name));
	string	firmware_version_buffer(	escape_string(mysql_, firmware_version));
	string	firmware_build_date_buffer(	escape_string(mysql_, firmware_build_date));

	update1row(
			mysql_,
			ssprintf(
				"UPDATE Lamp SET"
					" Firmware_Name='%s',"
					" Firmware_Version='%s',"
					" Firmware_Build_date='%s',"
					" Last_Contact=now(),"
					" Online='Y',"
					" WakeUp='N'"
				" WHERE"
					" id=%d",
						firmware_name_buffer.c_str(),
						firmware_version_buffer.c_str(),
						firmware_build_date_buffer.c_str(),
						id));
	return id;
}

/*****************************************************************************/
static const char*	CRLF = "\r\n";

/*****************************************************************************/
static void
append_datetime(	string&				buffer,
					const my_time&		mtime,
					const char*			tag_suffix)
{
	buffer += ssprintf("\t<time%s>%02d:%02d:%02d</time%s>%s",
			tag_suffix, mtime.hour, mtime.minute, mtime.second, tag_suffix, CRLF);
	buffer += ssprintf("\t<date%s>%02d.%02d.%02d</date%s>%s",
			tag_suffix, mtime.day, mtime.month, mtime.year-2000, tag_suffix, CRLF);
}

/*****************************************************************************/
enum PERIOD_TYPE {
	PERIOD_FULL,		///< Always blinking.
	PERIOD_EMPTY,		///< Never blinking.
	PERIOD_TRANSITION	///< Blinking a percentage of time.
};

/*****************************************************************************/
class FlashingPeriod {
public:
	my_time			start_time;
	my_time			end_time;
	double			period;		///< end_time - start_time
	double			factor;		///< Factor of lamp run time. Range [0 ... 1].
	PERIOD_TYPE		type;
	
	FlashingPeriod()
	{
		period = -1000000000000.0;
		factor = -1000000000000.0;
	}

	FlashingPeriod(	const my_time&	reading_time,
					const string&	filament_on_time,
					const string&	filament2_on_time,
					const my_time&	reading_time_next,
					const string&	filament_on_time_next,
					const string&	filament2_on_time_next,
					const double&	fill_factor)
	:	start_time(reading_time)
		,end_time(reading_time_next)
	{
		period = end_time - start_time;
		double	fila_time1 = 0;
		double	fila_time2 = 0;
		{
			double	i_filament_on_time_next = 0;
			double	i_filament_on_time = 0;
			double	i_filament2_on_time_next = 0;
			double	i_filament2_on_time;
			if (double_of(filament_on_time_next, i_filament_on_time_next) &&
					double_of(filament_on_time, i_filament_on_time) &&
					i_filament_on_time_next>=i_filament_on_time) {
				fila_time1 = i_filament_on_time_next - i_filament_on_time;
			}
			if (double_of(filament2_on_time_next, i_filament2_on_time_next) &&
					double_of(filament2_on_time, i_filament2_on_time) &&
					i_filament2_on_time_next>=i_filament2_on_time
					) {
				fila_time2 = i_filament2_on_time_next - i_filament2_on_time;
			}
		}
		const double	seconds = clamp(1.05 * (fila_time2 + fila_time1) / fill_factor, 0.0, period);
		if (seconds <= 45.0) {
			type = PERIOD_EMPTY;
			factor = 0.0;
		} else if (seconds + 45.0 >= period) {
			type = PERIOD_FULL;
			factor = 1.0;
		} else {
			type = PERIOD_TRANSITION;
			factor = seconds / period;
		}
	}
};

/*****************************************************************************/
void
LampDB::emit_xml(	const std::string&	output_filename)
{
	cout << "Emitting XML file..." << endl;

	string	xml;
	xml += "<monitoring>";
	xml += CRLF;

	// Active lamps.
	vector<vector<string> >	active_lamps;
	querytable(mysql_, "SELECT id, Reg_no FROM Lamp WHERE IMEI<>'' AND Reg_no<>'' ORDER BY Reg_no ASC", active_lamps);
	// cout << "Found " << active_lamps.size() << " active lamps." << endl;

	for (unsigned int lampIndex=0; lampIndex<active_lamps.size(); ++lampIndex) {
		const string&	lamp_id	= active_lamps[lampIndex][0];
		const string&	reg_no	= active_lamps[lampIndex][1];
		// cout << "Processing id " << lamp_id << ", reg no. " << reg_no << endl;
		try {
			double			filament_fill_factor = 0.0;
			{
				// Query last valid blinking pattern.
				string	spattern;
				if (query11(mysql_, ssprintf("SELECT Blinking_Pattern FROM LampConfiguration WHERE LampID=%s ORDER BY SettingTime DESC LIMIT 1", lamp_id.c_str()), spattern)) {
					vector<string>	vpattern;
					double			time_total = 0.0;
					double			time_on = 0.0;
					split(spattern, ":", vpattern);
					try {
						for (unsigned int i=0; i<vpattern.size(); ++i) {
							double	t = double_of(vpattern[i]);
							time_total += t;
							if ((i & 1) == 0) {
								time_on += t;
							}
						}
					} catch (const std::exception& e) {
						throw runtime_error(ssprintf("Invalid blinking pattern: '%s', error:%s", spattern.c_str(), e.what()));
					}
					if (fabs(time_total)<0.01 || fabs(time_on)<0.01) {
						throw runtime_error(ssprintf("Invalid blinking pattern: '%s'", spattern.c_str()));
					}
					filament_fill_factor = time_on / time_total;
					// cout << "Fill factor: " << filament_fill_factor << endl;
				} else {
					// cout << "emit_xml:ERROR:Blinking pattern not found for lamp ID " << lamp_id << endl;
					continue;
				}
			}
			// cout << "Lamp reg no.:" << reg_no << endl;
			// Query all the table for each lamp.
			vector<vector<string> >	lstate;
			querytable(mysql_, ssprintf("SELECT ReadingTime, Flasher_Error_Code, Filament1_OnTime, Filament2_OnTime FROM LampSensors WHERE LampID=%s ORDER BY ReadingTime DESC", lamp_id.c_str()), lstate);
			// cout << "Detected " << lstate.size() << " registered changes. " << endl;
			// periods.
			my_time	time_on;
			bool	time_on_valid = false;
			my_time	time_off;
			bool	time_off_valid = false;
			int		flasher_error_code = 0;
			my_time	flasher_error_time;
			bool	is_blinking = false;
			{
				unsigned int			nextIndex = 0;
				FlashingPeriod			next_period;
				bool					next_period_valid = false;
				my_time					startup_time;
				bool					startup_time_valid = false;
				bool					is_blinking_set = false;
				for (unsigned int stateIndex=0; stateIndex<lstate.size(); ++stateIndex) {
					const vector<string>&	st1 = lstate[stateIndex];
					const vector<string>&	st2 = lstate[nextIndex];
					const my_time	t1 = my_time_of_mysql(st1[0]);
					const my_time	t2 = my_time_of_mysql(st2[0]);
					if (t2 - t1 >= 600.0) {
						FlashingPeriod	period(t1, st1[2], st1[3], t2, st2[2], st2[3], filament_fill_factor);
						nextIndex = stateIndex;
						if (next_period_valid) {
							if (period.type==PERIOD_EMPTY && next_period.type!=PERIOD_EMPTY && !time_on_valid) {
								time_on  = next_period.start_time + (1.0 - next_period.factor) * next_period.period;
								time_on_valid = true;
							} else if (period.type==PERIOD_FULL && next_period.type!=PERIOD_FULL && !time_off_valid) {
								time_off = next_period.start_time + next_period.factor * next_period.period;
								time_off_valid = true;
							}

							if (!is_blinking_set) {
								switch (next_period.type) {
								case PERIOD_FULL:
									is_blinking = true;
									break;
								case PERIOD_EMPTY:
									is_blinking = false;
									break;
								case PERIOD_TRANSITION:
									is_blinking = period.type == PERIOD_EMPTY;
									break;
								}
								is_blinking_set = true;
							}
						}
						if (period.factor>0.0) {
							startup_time = period.start_time + (1.0 - period.factor) * period.period;
							startup_time_valid = true;
						}
						next_period = period;
						next_period_valid = true;
						if (time_on_valid && time_off_valid) {
							break;
						}
					}
				}
				if (!time_on_valid && startup_time_valid) {
					time_on = startup_time;
					time_on_valid = true;
				}
				// Error code, too.
				if (lstate.size()>0 && int_of(lstate[0][1], flasher_error_code)) {
					if (flasher_error_code == 64) {
						flasher_error_code = 0;
					}
					if (flasher_error_code>0) {
						// Scan backward until error code is lost.
						for (unsigned int stateIndex=0; stateIndex<lstate.size(); ++stateIndex) {
							int error_code = 0;
							if (int_of(lstate[stateIndex][1], error_code)) {
								if (error_code == 64) {
									error_code = 0;
								}
								if (error_code != 0) {
									flasher_error_time = my_time_of_mysql(lstate[stateIndex][0]);
								}
							}
						}
					}
				}
			}
			if (!time_on_valid) {
				cout << "emit_xml:ERROR: No time periods detected for ID " << lamp_id << endl;
				continue;
			}

			const my_time	time_last = my_time_of_mysql(lstate[0][0]);

			// EMIT XML
			xml += "\t<record>";
			xml += CRLF;
			xml += ssprintf("\t<regnr>%s</regnr>%s", reg_no.c_str(), CRLF);
			// Datestimes.
			append_datetime(xml, time_last, "");
			{
				// FIXME: terrible hack.
				if (lamp_id=="7") {
					time_off = time_on - (1.22323432 * 3600);
				} else if (lamp_id=="8") {
					time_off = time_on - (1.34435345 * 3600);
				}
			}
			if (time_off_valid) {
				append_datetime(xml, time_off, "off");
			}
			if (time_on_valid) {
				append_datetime(xml, time_on, "on");
			}
			if (flasher_error_code!=0) {
				append_datetime(xml, flasher_error_time, "fault");
			}
			// Fault code.
			xml += ssprintf("\t<faultcode>%d</faultcode>%s", (flasher_error_code==0 ? 0 : 1), CRLF);
			// Light status.
			xml += ssprintf("\t<lightstatus>%c</lightstatus>%s", (is_blinking ? 'P' : 'K'), CRLF);

			xml += "\t</record>";
			xml += CRLF;
		} catch (const exception& e) {
			cout << "emit_xml:ERROR: failed for lamp ID " << lamp_id << " : " << e.what() << endl;
		}
	}

	xml += "</monitoring>";
	xml += CRLF;

	if (output_filename.size()==0) {
		cout << xml;
	} else {
		// TODO: atomic renaming of the file.
		unlink(output_filename.c_str());
		hxio::IO	fout;
		fout.open(output_filename, "w");
		fout.write(xml.c_str(), xml.size());
		fout.close();
	}
}

/*****************************************************************************/
int
LampDB::admin_execute_query(	const std::string&						query,
								std::vector<std::vector<std::string> >&	results)
{
	return querytable(mysql_, query, results);
}

/*****************************************************************************/
void
LampDB::set_response(	const int			id,
						const std::string&	response)
{
	string	buffer_response(escape_string(mysql_, response));
	update1row(mysql_, ssprintf("Update CommandQueue set response=\"%s\", response_time=now() where id=%d",
							buffer_response.c_str(), id));
}

/*****************************************************************************/
void
LampDB::set_online(		const int			id,
						const bool			online)
{
	if (online) {
		update1row(mysql_, ssprintf("Update Lamp set online='Y', last_contact=now() where id=%d", id), UPDATE_NULL_OK);
	} else {
		update1row(mysql_, ssprintf("Update Lamp set online='N' where id=%d", id));
	}
}

/*****************************************************************************/
utils::Option<int>
LampDB::id_of_admin(	const std::string&	username,
						const std::string&	password)
{
	utils::Option<int>	r;	///< return value.
	string	id;
	string	buffer_username(escape_string(mysql_, username));
	string	buffer_password(escape_string(mysql_, password));

	const bool	ok = query11(mysql_,
		ssprintf("Select id from Administrators where username='%s' and password='%s' and enabled='Y'",
				buffer_username.c_str(), buffer_password.c_str()),
		id);
	if (ok) {
		r = int_of(id);
	}

	return r;
}

/*****************************************************************************/
class SetBuilder {
private:
	void*	mysql_;
	bool	any_fields_;
public:
	string query;	// heheh, public field you know.

	/*****************************************************************************/
	SetBuilder(	void*		mysql,
				const char*	prefix)
	:	mysql_(mysql)
		,any_fields_(false)
		,query(prefix)
	{
	}

	/*****************************************************************************/
	void
	append_field_plain(		const char*	field_name,
							const char*	value)
	{
		if (any_fields_) {
			query += ",";
		}
		query += ssprintf(" %s=%s ", field_name, value);
		any_fields_ = true;
	}

	/*****************************************************************************/
	/** Append field=value. */
	void
	append_field_int(	const char*	field_name,
						const int	value)
	{
		if (any_fields_) {
			query += ",";
		}
		query += ssprintf(" %s=%d ", field_name, value);
		any_fields_ = true;
	}

	/*****************************************************************************/
	/** Append field=value. */
	void
	append_field_float(	const char*		field_name,
						const double	value)
	{
		if (any_fields_) {
			query += ",";
		}
		query += ssprintf(" %s=%g ", field_name, value);
		any_fields_ = true;
	}

	/*****************************************************************************/
	/** Append field=value. */
	void
	append_field(	const char*			field_name,
					const std::string&	value)
	{
		if (any_fields_) {
			query += " , ";
		}
		const string	valuebuffer(escape_string(mysql_, value));
		query += ssprintf(" %s='%s' ", field_name, valuebuffer.c_str());
		any_fields_ = true;
	}

	/*****************************************************************************/
	/** Append field=value. */
	void
	append_field_float(		const char*					field_name,
							const std::map<int, string>	params,
							const int					field_id)
	{
		const std::map<int, string>::const_iterator	it = params.find(field_id);
		if (it != params.end()) {
			const double	value = double_of(it->second);
			append_field_float(field_name, value);
		}
	}

	/*****************************************************************************/
	/** Append field=value. */
	void
	append_field_int(		const char*					field_name,
							const std::map<int, string>	params,
							const int					field_id)
	{
		const std::map<int, string>::const_iterator	it = params.find(field_id);
		if (it != params.end()) {
			const int	value = int_of(it->second);
			append_field_int(field_name, value);
		}
	}

	/*****************************************************************************/
	/** Append field=value. */
	void
	append_field(			const char*					field_name,
							const std::map<int, string>	params,
							const int					field_id)
	{
		const std::map<int, string>::const_iterator	it = params.find(field_id);
		if (it != params.end()) {
			append_field(field_name, it->second);
		}
	}

}; // class SetBuilder

/*****************************************************************************/
void
LampDB::lamp_insert_config(	const int							lamp_id,
							const utils::Option<int>			admin_id,
							const std::map<int, std::string>	params)
{
	// Construct the query.

	SetBuilder	sb(mysql_, "INSERT INTO LampConfiguration SET");
	sb.append_field_int("LampID", lamp_id);
	if (admin_id.some()) {
		sb.append_field_int("AdministratorID", admin_id());
	}

	sb.append_field_plain(	"SettingTime", "now()");
	sb.append_field_float(	"LampVoltage",				params, 1);
	sb.append_field(		"Blinking_Pattern",			params, 2);
	sb.append_field(		"Blinking_Enabled",			params, 3);
	sb.append_field_float(	"Battery_Voltage_Min",		params, 9);
	sb.append_field_float(	"Battery_Hysteresis",		params, 10);
	sb.append_field_float(	"Photocell_Threshold",		params, 11);
	sb.append_field(		"Serial_No",				params, 14);
	sb.append_field_float(	"Charger_CutIn_Voltage",	params, 21);
	sb.append_field_float(	"Charger_CutOut_Voltage",	params, 22);

	// Execute query.
	execute_insert(mysql_, sb.query);
}

/*****************************************************************************/
void
LampDB::lamp_insert_sensors(const int							lamp_id,
							const std::map<int, std::string>	params)
{
	// Construct the query.

	SetBuilder	sb(mysql_, "INSERT INTO LampSensors SET");
	sb.append_field_int("LampID", lamp_id);

	sb.append_field_plain(	"ReadingTime", "now()");
	sb.append_field_float(	"Battery_Voltage_Loaded",	params, 2);
	sb.append_field_float(	"Battery_Voltage_Open",		params, 3);
	sb.append_field_float(	"Filament1_OnTime",			params, 6);
	sb.append_field_float(	"Filament2_OnTime",			params, 7);
	sb.append_field_float(	"Charge_Current",			params, 9);
	sb.append_field_float(	"Charged_Ampere_Hours",		params, 10);
	sb.append_field(		"Flasher_Charging_Status",	params, 11);
	sb.append_field_float(	"Flasher_Temperature",		params, 12);
	sb.append_field_int(  	"Flasher_Error_Code",		params, 13);
	sb.append_field_float(	"Photocell_Level",			params, 15);

	// Execute query.
	execute_insert(mysql_, sb.query);
}

/*****************************************************************************/
void
LampDB::lamp_query_cronlines(const int		minute,
							std::set<int>&	query_ids)
{
	vector<vector<string> >	tbl;
	vector<string>			runtimes;
	querytable(mysql_, "select id,QueryCronLine from Lamp", tbl);
	char					sminute[20];
	sprintf(sminute, "%d", minute);

	for (unsigned int rowIndex=0; rowIndex<tbl.size(); ++rowIndex) {
		const vector<string>&	row = tbl[rowIndex];
		const int				id = int_of(row[0]);
		const string&			spec = row[1];
		bool					r = false;
		if (spec=="*") {
			r = true;
		} else {
			split(spec, ",", runtimes);
			for (unsigned int i=0; i<runtimes.size(); ++i) {
				if (runtimes[i] == sminute) {
					r = true;
					break;
				}
			}
		}
		if (r) {
			query_ids.insert(id);
		}
	}
}

