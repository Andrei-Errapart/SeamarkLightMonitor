<?
// vim: shiftwidth=2
// vim: ts=2

	include "mysql.php";

	function fix_comma($s)
	{
		return str_replace(".", ",", $s);
	}

	
	function fix_date($s)
	{
		$t = strtotime($s);
		return date("d.m.Y H:i:s", $t);
	}

	$vars = $_GET;
	$lamp_id = round($vars["id"], 0);

	header("Content-Type: text/plain");
	header("Content-Disposition: attachment; filename=\"history_$lamp_id.csv\"");

	if ($lamp_id<1) {
		exit(0);
	}

	$result = mysql_query("SELECT " .
		"ReadingTime, Battery_Voltage_Loaded, Battery_Voltage_Open, " .
		"Filament1_OnTime, Filament2_OnTime, " .
		"Charge_Current, Charged_Ampere_Hours, Flasher_Charging_Status, " .
		"Flasher_Temperature, Flasher_Error_Code, Photocell_Level " .
		"FROM LampSensors WHERE LampID=$lamp_id ORDER BY ReadingTime desc");

	echo "ReadingTime;Battery_Voltage_Loaded;Battery_Voltage_Open;Filament1_OnTime;Filament2_OnTime;Charge_Current;Charged_Ampere_Hours;Flasher_Charging_Status;Flasher_Temperature;Flasher_Error_Code;Photocell_Level\n";
	while ($row = mysql_fetch_assoc($result)) {
		$reading_time							= $row['ReadingTime'];
		$battery_voltage_loaded		= fix_comma($row['Battery_Voltage_Loaded']);
		$battery_voltage_open			= fix_comma($row['Battery_Voltage_Open']);
		$filament1_ontime 				= fix_comma($row['Filament1_OnTime']/3600.0);
		$filament2_ontime 				= fix_comma($row['Filament2_OnTime']/3600.0);
		$charge_current						= fix_comma($row['Charge_Current']);
		$charged_ampere_hours			= fix_comma($row['Charged_Ampere_Hours']);
		$flasher_charging_status	= fix_comma($row['Flasher_Charging_Status']);
		$flasher_temperatur				= fix_comma($row['Flasher_Temperature']);
		$error_code 							= fix_comma($row['Flasher_Error_Code']);
		$photocell_level					= fix_comma($row['Photocell_Level']);
		if ($error_code == 64) {
			// FIXME: fix it for multi-filament lamps.
			$error_code = 0;
		}
		// echo "$reading_time\n";
		echo "$reading_time;$battery_voltage_loaded;$battery_voltage_open;$filament1_ontime;$filament2_ontime;$charge_current;$charged_ampere_hours;$flasher_charging_status;$flasher_temperatur;$error_code;$photocell_level\n";
	}

?>
