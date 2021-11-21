<!--
vim: shiftwidth=2
vim: ts=2
-->
<?
	include "mysql.php";

	function make_lamp_url($lamp_id)
	{
		return "http://195.50.203.61:8010/TORNID?Lan=" . $lamp_id;
	}

	/*
	$db = mysql_connect("localhost", "LampServer", "ServerLamp");
	mysql_select_db("LampServer", $db);

	/// Execute query and return first column from the first row.
  function query11($q) {
		global $db;
	  $result = mysql_query($q, $db);
		$row = mysql_fetch_row($result);
		mysql_free_result($result);
		return $row[0];
	}

	/// Execute query and return first row.
  function query1($q) {
		global $db;
	  $result = mysql_query($q, $db);
		$row = mysql_fetch_assoc($result);
		mysql_free_result($result);
		return $row;
	}

	function plot_result_box($text, $color) {
		?>
		<table border="1">
			<tr>
				<td>
					<font color="<?=$color?>">
					<?=$text?>
					</font>
				</td>
			</tr>
		</table>
		<?
	}

	function do_query($q, $db) {
		$result = mysql_query($q, $db);
		echo "<p>$q</p>\n";
		return $result;
	}
	*/

	/*
	function do_command($q, $oktext) {
		global $db;
		$result = mysql_query("INSERT INTO CommandQueue (command_time, command) values (now(), '" . mysql_real_escape_string($q, $db) . "')", $db);
		$id = query11("select last_insert_id()");
		$t0 = gettimeofday(true);
		for ($i=0; $i<5; ++$i) {
			// sleep 0.2 seconds
			usleep(200 * 1000);

			// any response?
			$result = mysql_query("select response,response_time from CommandQueue where id=$id", $db);
			$row = mysql_fetch_assoc($result);
			$response = $row['response'];
			$response_time = $row['response_time'];
			if ($response && $response_time) {
				if ($response == "OK") {
					plot_result_box($oktext, "black");
				} else {
					plot_result_box("Viga : " . $response, "red");
				}
				return;
			}
		}
		$t1 = gettimeofday(true);
		$dt = $t1 - $t0;
		plot_result_box("Server ei tööta. Päring $id aeg: $dt sekundit.", "red");
	}
	*/
	function is_late($mysql_time)
	{
		$mtime = strtotime($mysql_time);
		$hour_ago = time() - 3600 - 3600;
		return $hour_ago > $mtime; //? "TRUE" : "FALSE";
	}
?>

<html>
	<head>
		<META HTTP-EQUIV="Refresh" CONTENT="300">
		<title>Liinitulede monitooring</title>
	</head>

	<!-- <body bgcolor="#1E77D3"> -->
	<body bgcolor="#FFFFFF">
		<h1><a href="http://www.meri.ee/">Merem&otilde;&otilde;dukeskus O&Uuml;</a></h1>
		<h1>Navigatsioonim&auml;rkide seire</h1>
		<p>
		<?
			// do_command("blah kraam", "proovime suvateksti");
			/*
			$vars = $_GET;
			$action = $vars["action"];

			if ($action == "upload") {
				$id = $vars["id"];
				$lamp_voltage			= $vars["Lamp_Voltage"];
				$plinking_pattern	= $vars["Plinking_Pattern"];
				do_command("tsync record Lamp $id = "
					. ($lamp_voltage ? "1=$lamp_voltage " : "")
					. ($plinking_pattern ? "2=$plinking_pattern " : ""),
					$vars["message"]);
			}
			*/
		?>
		<p>
		Serveri kellaaeg: <?=date('Y-m-d H:i:s') ?>
		<p>
		<table border="1">
		<?
			$result = mysql_query("SELECT ID, IMEI, Firmware_Build_date, Phone_no, Reg_no, Description, Owner, Blinking_URL, Plinking_ISO, Last_Contact, Online FROM Lamp ORDER BY id ASC");
			if ($result) {
				?>
					<tr>
						<td>ID</td>
						<td>Reg. Nr.</td>
						<td>Olek</td>
						<td>Omanik</td>
						<td>Kirjeldus</td>
						<td>Valgusallika tööpinge</td>
						<td>Plinkimine</td>
						<td>Viimane kontakt</td>
						<td>Seadistuste muutmine</td>
						<td>Andmete lugemine</td>
						<td>Aku koormusega</td>
						<td>Aku vabalt</td>
						<td>1. valgusallikas</td>
						<td>2. valgusallikas</td>
						<td>Laadimisvool</td>
						<td>Laadimine</td>
						<td>Temp.</td>
						<td>Hämara l&uuml;lituslävi</td>
						<td>Hämara- andur</td>
					</tr>
				<?
				while ($row = mysql_fetch_assoc($result)) {
					$reg_no = $row['Reg_no'];
					$lamp_voltage = $row['Lamp_Voltage'];
					$plinking_pattern = $row['Plinking_Pattern'];
					// $last_contact = $row['Last_Contact'];
					$is_delayed = is_late($row['Last_Contact']);
					$online = $row['Online']=='Y';
					$lamp_id = $row['ID'];
					$imei = $row['IMEI'];
					$blinking_url = $row['Blinking_URL'];

					$settings = query1("SELECT " .
													"AdministratorID, " .
													"SettingTime, LampVoltage, Blinking_Pattern, Blinking_Enabled, " .
													"Battery_Voltage_Min, Battery_Hysteresis, Photocell_Threshold, " .
													"Serial_No, Charger_CutIn_Voltage " .
												"FROM LampConfiguration WHERE LampID=$lamp_id ORDER BY SettingTime desc LIMIT 1");


					$sensors = query1("SELECT " .
												"ReadingTime, Battery_Voltage_Loaded, Battery_Voltage_Open, " .
												"Filament1_OnTime, Filament2_OnTime, " .
												"Charge_Current, Charged_Ampere_Hours, Flasher_Charging_Status, " .
												"Flasher_Temperature, Flasher_Error_Code, Photocell_Level " .
												"FROM LampSensors WHERE LampID=$lamp_id ORDER BY ReadingTime desc LIMIT 1");

					// $error_code
					if ($sensors && $online) {
						$error_code = $sensors['Flasher_Error_Code'];
						if ($error_code == 64) {
							// FIXME: 
							$error_code = 0;
						}
						if ($error_code == 0) {
							$error_code = "<font color=\"green\">Korras</font>";
						} else {
							$error_code = "<font color=\"red\">Veakood: $error_code</font>";
						}
					} else if ($imei) {
						if ($is_delayed) {
							$error_code = "<font color=\"red\">Teadmata</font>";
						} else {
							$error_code = "<font color=\"green\">Korras</font>";
						}
					} else {
						$error_code = "Ehitamisel";
					}

					?>
						<tr>
							<!-- Yldandmed -->
							<td><?=$row['ID']?></td>
							<td><?=$reg_no?></td>
							<td><?=$error_code?> <font color="<?=($online || !$is_delayed) ? "green" : "red"?>"><?=$online ? 'Online' : ($is_delayed ? 'Offline' : 'Standby')?></font></td>
							<td><?=$row['Owner']?>&nbsp;</td>
							<td>
								<? if ($reg_no) { ?>
									<?=$row['Description']?>&nbsp;
								<? } else { ?>
									<a href="<?=make_lamp_url($reg_no)?>"><?=$row['Description']?>&nbsp;</a>
								<? } ?>
								</td>
							<!-- Seadistused -->
							<?
							$photocell_threshold = "";
							if ($settings) {
							?>
								<td><?=$settings['LampVoltage']?> V</td>
								<td><?=$row['Plinking_ISO']?> <?=$settings['Blinking_Pattern']?></td>
								<td><?=$row['Last_Contact']?></td>
								<td><?=$settings['SettingTime']?></td>
								<? $photocell_threshold = $settings['Photocell_Threshold']; ?>
							<? } else { ?>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td><?=$row['Last_Contact']?></td>
								<td>&nbsp;</td>
							<? } ?>
							<!-- Andurid -->
							<?
							if ($sensors) {
							?>
								<td><?=$sensors['ReadingTime']?> <a href="readings.php?id=<?=$lamp_id?>">Ajalugu (Excel)</a></td>
								<td><?=$sensors['Battery_Voltage_Loaded']?>&nbsp;V</td>
								<td><?=$sensors['Battery_Voltage_Open']?>&nbsp;V</td>
								<td><?=round($sensors['Filament1_OnTime']/3600.0, 1)?>&nbsp;tundi</td>
								<td><?=round($sensors['Filament2_OnTime']/3600.0, 1)?>&nbsp;tundi</td>
								<td><?=$sensors['Charge_Current']?> A</td>
								<td><?=$sensors['Charged_Ampere_Hours']?>&nbsp;Ah</td>
								<td><?=$sensors['Flasher_Temperature']?>&nbsp;C</td>
								<? if ($blinking_url) { ?>
								<td><a href="<?=$blinking_url?>"><?=$photocell_threshold?>&nbsp;V</a></td>
								<? } else { ?>
								<td><?=$photocell_threshold?>&nbsp;V</td>
								<? } ?>
								<td><?=$sensors['Photocell_Level']?>&nbsp;V</td>
							<? } else { ?>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
								<td>&nbsp;</td>
							<? } ?>
						</tr>
					<?
				}
			} else {
				?>
				<tr>
					<td>Andmebaasi viga!</td>
				</tr>
				<?
			}
		?>
		</table>

		<br>
		<br>
		<hr>
		<i>Teostaja: <a href="http://www.errapartengineering.com/">Errapart Engineering O&Uuml;</a></i>
	</body>
</html>

