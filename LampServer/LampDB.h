	// vim: shiftwidth=4
// vim: ts=4
#ifndef LampServer_LampDB_h_
#define LampServer_LampDB_h_

#include <string>		// std::string
#include <map>			// std::map
#include <set>			// std::set

#include <utils/util.h>	// utils::Option

#if (0)
class Powerlight {
public:
	unsigned int	NormalIntensity;
	unsigned int	DefaultIntensity;
	double			NightLevel;
	std::vector<std::pair<double, double> >	SunriseLevels;
	std::vector<std::pair<double, double> >	SunsetLevels;

	/** Is it daytime? */
	bool
	IsDayTime(
		const double	time_of_day) const;
	/** Is it sunrise time? */
	bool
	IsSunriseTime(
		const double	time_of_day) const;
	/** Is it sunset time? */
	bool
	IsSunsetTime(
		const double	time_of_day) const;
	
	/** Print data to the log. */
	void
	ToLog() const;
}; // class Powerlight.
#endif

/** LampServer database. */
class LampDB {
private:
	void*	mysql_;
public:
	/** Connects to the localhost MySQL database.
	 */
	LampDB(	const std::string&	database,
			const std::string&	username,
			const std::string&	password,
			const bool			skip_init);

	~LampDB();

	/** Set response for the given command.
	*/
	void
	set_response(	const int			id,
					const std::string&	response);

	/** Query ID by IMEI. If there isn't any, create one.
	Update firmware info, too.
	
	\param[in]	imei				Identification string.
	\param[in]	firmware_name		Firmware name.
	\param[in]	firmware_version	Firmware version.
	\param[in]	firmware_build_date	Firmware build date.
	\param[out]	pair_id				Pair lamp id in the pair, if any.
	\param[out]	wake_up				Field \c Lamp.WakeUp. Should the device be waken up?
	\param[out]	keep_down			Field \c Lamp.KeepDown. Should the device be sent periodical shutdown requests?
	\param[out]	sunshine_run		Field \c Lamp.SunshineRun.
	\param[out]	position_latitude	Latitude of position. This is given by the lamp with the lower ID in the pair.
	\param[out]	position_longitude	Longitude of position. This is given by the lamp with the lower ID in the pair.
	\return ID corresponding to the given IMEI.
	*/
	int
	register_lampcontroller(
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
	);

	/** Output XML file based on current database settings.
	\param[in]	output_filename	Name of the file, or empty string for standard output.
	 */
	void
	emit_xml(					const std::string&	output_filename);

	/** Execute a query. Only for administrators.
	 \return Number of rows affected by UPDATE, DELETE or INSERT.
	 */
	int
	admin_execute_query(		const std::string&						query,
								std::vector<std::vector<std::string> >&	results);

	/** Set online status of the lamp. Also updates the last_contact field iff online=true.
	*/
	void
	set_online(		const int			id,
					const bool			online);

	/** Query valid ID by administrator info. */
	utils::Option<int>
	id_of_admin(	const std::string&	username,
					const std::string&	password);

	/** Insert new configuration record into database. */
	void
	lamp_insert_config(	const int							lamp_id,
						const utils::Option<int>			admin_id,
						const std::map<int, std::string>	params);

	/** Insert new sensor record into database. */
	void
	lamp_insert_sensors(const int							lamp_id,
						const std::map<int, std::string>	params);

	/** Select cronlines from the Lamp table.
		\param[in]	minute			Current minute
		\param[out]	query_id_list	Set of id's that should be queried for sensors.
	*/
	void
	lamp_query_cronlines(const int		minute,
						std::set<int>&	query_ids);

#if (0)
	/** Query powerlight data, if any. */
	utils::Option<Powerlight>
	lamp_query_powerlight(
		const int	lamp_id
	);
#endif
}; // class LampDB

#endif /* LampServer_LampDB_h_ */

