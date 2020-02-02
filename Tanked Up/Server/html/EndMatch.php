<?php
include "DBinfo.inc";
$connection = CreateConnection();

//Get data from the current player about match (Kills, deaths, etc)
$kills = $_GET['Kills'];
$deaths = $_GET['Deaths'];
$win = (bool)$_GET['Win'];

$sessionID = $_GET['SessionID'];
$sessionID = "'".$sessionID."'";
//echo $sessionID;
//if playerID associated with the sessionID == hostID, then continue
$sql = "SELECT UserID FROM Users WHERE SessionString = $sessionID";
$result = mysqli_query($connection, $sql);
$playerID = mysqli_fetch_array($result)['UserID'];
//$playerID = "'".$playerID."'";
//echo $playerID;
if($playerID == 0)
{
	die("Not a valid ID");
}

//Query matches table to see if $playerID is the host or client
$isHost = false;
$sql = "SELECT * FROM Matches WHERE HostID = '$playerID' OR ClientID = '$playerID'";
$result = mysqli_query($connection, $sql);
$return_arr = array();
$matchID = 0;
//Loop through Matches data
while($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
	array_push($return_arr, $row);
	$matchID = $row['MatchID'];
	if($playerID == $row['HostID'])
	{
		$isHost = true;
	}
	else 
	{
		$isHost = false;
	}

}

//Update match stats
if($isHost)
{
	$sql = "UPDATE Matches SET HostKills = '$kills', HostDeaths = '$deaths', HostWin = '$win', HostConnected = false WHERE MatchID = '$matchID'";
}
else 
{
	$sql = "UPDATE Matches SET ClientKills = '$kills', ClientDeaths = '$deaths', ClientWin = '$win', ClientConnected = false WHERE MatchID = '$matchID'";
}
$result = mysqli_query($connection, $sql);

$matchResultsValid = true;
//Once both players have uploaded match data, compare what they've uploaded
if($return_arr[0]['HostKills'] != null && $return_arr[0]['ClientKills'] != null && $return_arr[0]['HostDeaths'] != null && $return_arr[0]['ClientDeaths'] != null && $return_arr[0]['HostWin'] != null && $return_arr[0]['ClientWin'] != null)
{
	if($return_arr[0]['ClientKills'] != $return_arr[0]['HostDeaths'])
	{
		//echo "CKills != HDeaths";
		$matchResultsValid = false;
	}
	else if($return_arr[0]['HostKills'] != $return_arr[0]['ClientDeaths'])
	{
		//echo "HKills != CDeaths";
		$matchResultsValid = false;
	}
	else if($return_arr[0]['HostWin'] == $return_arr[0]['ClientWin'])
	{
		//echo "Both players say they won";
		$matchResultsValid = false;
	}
	if(!$matchResultsValid)
	{
		echo "Invalid Match Results";
		die("");
	}
}
else 
{
	//echo $return_arr[0]['HostKills'];
	$matchResultsValid = false;
}
$hostID = $return_arr[0]['HostID'];
$clientID = $return_arr[0]['ClientID'];

//if($return_arr[0]['ClientKills'] != null && $return_arr[0]['HostDeaths'] != null && $return_arr[0]['HostKills'] != "null" && $return_arr[0]['ClientDeaths'] != null && 
//	$return_arr[0]['HostWin'] != null && $return_arr[0]['ClientWin'] != null)
//{
//	$matchResultsValid = true;
//}
//else 
//{
//	$matchResultsValid = false;
//}


//If players uploaded matching match data
if($matchResultsValid)
{
	echo json_encode($return_arr[0]);
	//update their stats 
	$sql = "UPDATE Users SET Kills = Kills + '$kills', Deaths = Deaths + '$deaths' WHERE SessionString = $sessionID";
	//$sql = "UPDATE Users SET Kills = Kills + '$kills', Deaths = Deaths + '$deaths' WHERE UserID = $hostID OR UserID = $clientID";
	$result = mysqli_query($connection, $sql);
	//echo mysqli_error($connection);
	//remove their match from the matches table
	if($return_arr[0]['HostConnected'] == false && $return_arr[0]['ClientConnected'] == false)
	{
		$sql = "DELETE FROM Matches WHERE MatchID = $matchID";
		//$result = mysqli_query($connection, $sql);
		//echo "Deleted match from queue";
	}
}
else 
{
	echo "Needs other player's match data";
}


//Update leaderboard data

?>