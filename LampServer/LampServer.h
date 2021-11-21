// vim: shiftwidth=4
// vim: ts=4
#include <vector>
#include <list>
#include <memory>

#include <utils/util.h>
#include <utils/TcpServer.h>
#include <utils/CMR.h>

#include "LampDB.h"
#include "LampConfig.h"

class LampServer; // forward.

/*****************************************************************************/
// One of our beloved clients.
class LampConnection : public utils::TcpConnection {
	friend class LampServer;
private:
	LampServer&				server_;
	LampDB&					db_;
	utils::CMRDecoder		cmrdecoder_;
public:
	utils::Option<int>		id;					///< Lamp ID, if any received.
	utils::Option<int>		admin_id;			///< Administrator interface ID, if any.
	utils::Option<int>		pair_id;			///< Id of the other lamp in the pair, if any.
	bool					keep_down;			///< Should we send shutdown on connect and every sensorcounter request?
	long_long_t				keepdown_shutdown_time;	///< Shutdown scheduled by \c KeepDown, -1 if none.
	long_long_t				received_ping_time;	///< Received ping time, Microseconds given by utils::timer::current_time().
	long_long_t				sent_ping_time;		///< Sent ping time, microsceonds given by utils::timer::current_time().
	utils::Option<int>		forward_admin_id;	///< Last forward source administrators ID.
	long_long_t				forward_time;		///< Last forward command time, microseconds.

	/* The following is valid only if \c id has been set. */
	bool					sunshine_run;				///< Sunshine run: Should the lamp be turned on for a day?
	double					position_latitude;			///< Latitude of position.
	double					position_longitude;			///< Longitude of position.

	utils::my_time			sunrise_time;				///< Local time of sunrise.
	utils::my_time			sunset_time;				///< Local time of sunset.
	utils::my_time			sunshine_run_time_begin;	///< Sunshine run: When to turn lamp on.
	utils::my_time			sunshine_run_time_end;		///< Sunshine run: When to turn lamp off.
	bool					started_on_sunshine;		///< Was the lamp started during sunshine_run?

	LampConnection(	LampServer&		server,
					LampDB&			db);
	virtual ~LampConnection();

	/** Log a message preceded by time in format "HHMMSS". */
	void
	log(			const char*		fmt, ...);

	/** Write CMR::LAMPNET command to the client. */
	void emit(	const std::string&	msg);

	/** Emit ping. */
	void emit_ping();

	/** Update sunrise/sunset times, i.e. sunrise_time, sunset_time, sunshine_run_time_begin and sunshine_run_time_end. */
	void
	update_sunrise_sunset(
		const utils::my_time&		current_time
	);

	virtual void
	handle_connect();

	void
	do_admin_command(	const std::string&	cmd,
						const std::string&	op,
						const std::string&	params);

	void
	do_lamp_command(	const std::string&	cmd,
						const std::string&	op,
						const std::string&	params);

	virtual void
	handle_read(		const std::vector<unsigned char>&		data);

	virtual void
	handle_error(		short				what);
}; // class LampConnection

/*****************************************************************************/
/** Our beloved lampserver. */
class LampServer {
friend class LampConnection;
public:
	/** Find LampConnection given a lamp id in the table. */
	LampConnection*
	find_connection(	const int	lamp_id);
private:
	/** Type of XML monitoring update. */
	typedef enum {
		MONITORUPDATE_STARTUP,	///< monitor.xml updated on startup.
		MONITORUPDATE_SUNRISE,	///< monitor.xml updated on sunrise.
		MONITORUPDATE_SUNSET,	///< monitor.xml updated on sunset.
		MONITORUPDATE_TIMEOUT,	///< monitor.xml updated after 24h timeout.
	} MONITORUPDATE;

	LampConfig								cfg_;
	std::auto_ptr<utils::TcpPortListener>	listener_;
	std::auto_ptr<LampDB>					db_;
	std::auto_ptr<utils::Timer>				command_timer_;
	std::auto_ptr<utils::Timer>				manager_timer_;
	utils::my_time							manager_timer_lasttime_;///< Last time manager_timer was run.
	utils::my_time							monitor_time_;			///< Last XML updating time.
	MONITORUPDATE							monitor_update_;		///< Type of XML update.

	utils::TcpConnection*
	make_LampConnection(	std::string&	peer_name,
							int				peer_port);

	void
	manager_timer(	short	event);

public:
	/** Run the lampserver. */
	void
	run(int argc, char** argv);
}; // class LampServer

