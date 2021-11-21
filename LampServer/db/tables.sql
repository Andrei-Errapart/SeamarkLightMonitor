-- Database creation for the LampServer.
-- --------------------------------------------------------------------
drop table if exists Administrators;
create table Administrators (
	-- Row ID
	id		int		not null auto_increment,
	-- User name.
	username	char(255)	not null,
	-- Real name.
	name		char(255)	not null,
	-- Password.
	password	char(255)	not null,
	-- Enabled?
	enabled		char(1)		not null default 'N',
	primary key(id)
); -- Administrators

-- --------------------------------------------------------------------
drop table if exists Lamp;
create table Lamp (
	-- Row ID
	id		int			not null auto_increment,
	-- Is lamp public (yet)?
	IsPublic	char(1)			not null default 'N',
	-- IMEI = Hardware serial no.
	IMEI		char(255)		not null,
	-- FIRMWARE name as given in the protocol.
	Firmware_Name	char(255)		not null,
	-- Firmware version.
	Firmware_Version	char(255)	not null,
	-- Firmware build date (if possible to decode).
	Firmware_Build_date	datetime	not null,
	-- SIM phone number.
	Phone_no	char(255)		not null,
	-- Registration nr. with Estoninan Maritime Administration
	Reg_no		char(255)		not null,
	-- Short description.
	Description	char(255)		not null default '',
	-- Owner, free form.
	Owner		char(255)		not null default '',
	-- URL pointing to the blinking mode description.
	Blinking_URL	char(255)		not null default '',
	-- ISO description of the blinking pattern.
	Plinking_ISO	char(80)		not null default '',
	-- Last contact time (data byte received or connection setup).
	Last_Contact	datetime,
	-- Is it currently considered online?
	Online		char(1)			not null default 'N',
	-- Shall server send wake-up call to the client when contacted?
	WakeUp		char(1)			not null default 'N',
	-- Shall server keep the client in standby?
	KeepDown	char(1)			not null default 'N',
	-- Shall server turn the lamp on during sunshine?
	-- Sunshine time is calculated from the current time, clients latitude and longitude.
	SunshineRun	char(1)			not null default 'N',
	-- Latitude of position, degrees. Default is Tallinn.
	PositionLatitude	double		not null default 59.386511,
	-- Longitude of position, degrees. Default is Tallinn.
	PositionLongitude	double		not null default -24.65111389,
	-- Comma-separated list of the minutes of the lamp sensors query times.
	-- Default is at 5th and 35th minute of a full hour.
	QueryCronLine	varchar(255)		not null default '5,35',
	-- ID of the second light ID in the range light pair.
	PairID				int,
	-- Powerlight: normal (daylight) intensity in the range 0..255. 0=turn off, NULL=no powerlight installed.
	-- See also: PowerlightUpdateFrequency in LampServer.ini.
	PowerlightNormalIntensity	int,
	-- Powerlight: default intensity, which is used after the light intensity hasn't been uploaded
	-- by modem/server in 15 minutes.
	PowerlightDefaultIntensity	int not null default 20,
	-- Powerlight: nighttime relative intensity level, percentage of daylight intensity.
	PowerlightNightLevel		double not null default 100,
	-- Powerlight: intensity during sunrise.
	-- Comma-separated list of 'time=level', where time is in minutes relative to sunrise.
	-- and level is percentage relative to the daytime intensity.
	-- Important: the time interval must be shorter than 15 minutes in order
	--  to avoid the powerlight defaulting to the normal level.
	-- Example: '-10=150,-5=200,5=150,10=100'.
	PowerlightSunriseLevels		varchar(255) not null default '',
	-- Powerlight: see PowerlightSunariseLevels for documentation.
	PowerlightSunsetLevels		varchar(255) not null default '',
	Primary key(id)
); -- Lamp

-- --------------------------------------------------------------------
drop table if exists LampConfiguration;
create table LampConfiguration (
	id			int		not null auto_increment,
	LampID			int		not null,	-- Points to Lamp.id.
	AdministratorID		int,				-- Points to Administrator.id, null if the configuration was entered on boot-up.
	SettingTime		datetime	not null,	-- time when entered. 
	LampVoltage		double		not null,	-- lamp psu voltage, V
	Blinking_Pattern	char(80)	not null,	-- t1_on:t1_off:t2_on:t2_off - times are in milliseconds.
	Blinking_Enabled	char(1)		not null default 'Y',	-- 'Y'/'N'
	Battery_Voltage_Min	double		not null,	-- V
	Battery_Hysteresis	double		not null,	-- V
	Photocell_Threshold	double		not null,	-- arbitrary units in range 0...5.0.
	Serial_No		char(64)	not null,	-- arbitrary text :)
	Charger_CutIn_Voltage	double		not null,	-- V
	Charger_CutOut_Voltage	double		not null,	-- V
	Primary key(id)
); -- LampConfiguration

-- --------------------------------------------------------------------
drop table if exists LampSensors;
create table LampSensors (
	id			int	not null auto_increment,
	LampID			int	not null,		-- Points to Lamp.id.
	ReadingTime		datetime not null,		-- Time when read from the lamp.
	Battery_Voltage_Loaded	double,		-- Battery voltage when loaded.
	Battery_Voltage_Open	double,		-- Battery voltage when open.
	Filament1_OnTime	double,		-- Filament 1 ontime, seconds.
	Filament2_OnTime	double,		-- Filament 2 ontime, seconds.
	Charge_Current		double,		-- Charge current.
	Charged_Ampere_Hours	double,		-- Charging progress, amperhours.
	Flasher_Charging_Status	char(20),	-- Charging status, one of "Y" | "N".
	Flasher_Temperature	double,		-- Temperetaure, degrees.
	Flasher_Error_Code	int,		-- Error code (see SmartFlasher 6-28 User manual for reference).
	Photocell_Level		double,

	primary key(id)
); -- LampConfiguration

-- --------------------------------------------------------------------
drop table if exists PowerlightReadings;
create table PowerlightReadings (
	id			int		not null auto_increment,
	-- Time when read from the lamp.
	ReadingTime		datetime	not null,
	--
	ReadingType		char(1)		not null default 'G',
	-- Current light intensity, -1 if not read.
	CurrentIntensity	int		not null default -1,
	-- Default light intensity, -1 if not read.
	DefaultIntensity	int		not null default -1,
	-- Next current intensity, -1 if not writing.
	NextCurrentIntensity	int		not null default -1,
	-- Next default intensity, -1 if not writing.
	NextDefaultIntensity	int		not null default -1,

	primary key(id)
); -- PowerlightReadings

