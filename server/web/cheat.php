<?php
header("Content-Type: application/json");

$link = mysqli_connect("127.0.0.1", "ps4trainer", "hk294ienekmal", "ps4trainer");

if(mysqli_connect_errno()) {
	die("mysqli_connect_errno: " . mysqli_connect_error());
}

// grab specific cheat data if titleid, region, or version is set
if(isset($_GET["titleid"]) || isset($_GET["region"]) || isset($_GET["version"])) {
	$query = "SELECT * FROM cheats WHERE ";
	$first = true;
	
	if(isset($_GET["titleid"])) {
		$titleid = mysqli_real_escape_string($link, $_GET["titleid"]);
		$query .= "titleid=\"$titleid\"";
		$first = false;
	}
	
	if(isset($_GET["region"])) {
		if(!$first) {
			$query .= " AND";
		}
		$region = mysqli_real_escape_string($link, $_GET["region"]);
		$query .= " region=\"$region\"";
		$first = false;
	}
	
	if(isset($_GET["version"])) {
		if(!$first) {
			$query .= " AND";
		}
		$version = mysqli_real_escape_string($link, $_GET["version"]);
		$query .= " version=\"$version\"";
	}
	
	$result = mysqli_query($link, $query);

	// display the first results cheatdata field
	$row = mysqli_fetch_row($result);
	if($row) {
		printf("%s\n", preg_replace("/\s+/", "", $row[4]));
	} else {
		printf("no such cheat\n");
	}

	mysqli_free_result($result);
	mysqli_close($link);
	exit();
}


// list all cheats
$result = mysqli_query($link, "SELECT * FROM cheats");

printf("[ ");
$row = mysqli_fetch_row($result);
while($row) {
	printf("{ \"id\": %s, \"titleid\": \"%s\", \"region\": \"%s\", \"version\": \"%s\" }", $row[0], $row[1], $row[2], $row[3]);

	$row = mysqli_fetch_row($result);
	if($row) {
		printf(",");
	}
}
printf(" ]\n");

mysqli_free_result($result);
mysqli_close($link);

?>
