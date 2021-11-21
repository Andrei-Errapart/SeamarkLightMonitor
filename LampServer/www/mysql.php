<?
// vim: shiftwidth=2
// vim: ts=2

// Setup

	$db = mysql_connect("localhost", "LampServer", "ServerLamp");
	mysql_select_db("LampServer", $db);

	/** Execute query and return first column from the first row. */
  function query11($q) {
		global $db;
	  $result = mysql_query($q, $db);
		$row = mysql_fetch_row($result);
		mysql_free_result($result);
		return $row[0];
	}

	/** Execute query and return first row. */
  function query1($q) {
		global $db;
	  $result = mysql_query($q, $db);
		$row = mysql_fetch_assoc($result);
		mysql_free_result($result);
		return $row;
	}

	function do_query($q, $db) {
		$result = mysql_query($q, $db);
		echo "<p>$q</p>\n";
		return $result;
	}

?>
