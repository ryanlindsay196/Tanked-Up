<?php include "DBinfo.inc"; ?>
<?php

function PrintMatchData($in_IP)
{
	//$matchData;
	if($in_IP == $_GET['IP'])
	{
		$matchData['IsHost'] = TRUE;
	}
	else 
	{
		$matchData['IsHost'] = FALSE;
	}

	$matchData['IPAddress'] = $in_IP;

	echo json_encode($matchData);
}

$connection = CreateConnection();

$sessionID = $_GET['SessionID'];

//Find current userID based on sessionID
$sql = "SELECT UserID FROM Users WHERE SessionString = '$sessionID'";
$result = mysqli_query($connection, $sql);
$UserID = intval(mysqli_fetch_array($result, MYSQL_ASSOC)['UserID']);
//echo $UserID."::UserID::";
//Find if player is already in match
$sql = "SELECT * FROM Matches WHERE ClientID = '$UserID' OR HostID = '$UserID' LIMIT 1";
//$sql = "SELECT * FROM Matches LIMIT 2";
$result = mysqli_query($connection, $sql);

//$hostIP = json_encode(mysqli_fetch_array($result, MYSQL_ASSOC))['HostIP'];
$hostIP = mysqli_fetch_row($result)[1];
//$hostIP = $_GET['IP'];
//echo mysqli_num_rows($result)."ROWS";
if(mysqli_num_rows($result) > 0)
{//If match found
	//$hostIP = json_encode($result)['HostIP'];
	PrintMatchData($hostIP);
	//PrintMatchData(json_encode($result['HostIP']);
}
else 
{
	//Get list of all valid users in queue that aren't the current user (POTENTIAL MATCHUPS)

	if($UserID == 0)
	{
		die("UserID $UserID is invalid");
	}

	$sql = "SELECT UserID FROM Queue WHERE UserID != '$UserID'";
	$result = mysqli_query($connection, $sql);
	$ValidUsers = array();
	while($row = mysqli_fetch_array($result, MYSQL_ASSOC)) 
	{
	        array_push($ValidUsers, $row['UserID']);
		//print_r($row['UserID']."::");
	}
	
	//loop through list of valid users and create match with you and another
	//echo mysqli_num_rows($result);
	
	//If session ID is not valid, Send error message to client
	$sql = "SELECT UserID FROM Users WHERE SessionString = '$sessionID'";
	$result = mysqli_query($connection, $sql);
	//echo "1";
	if(!$result)
	{
		die("ERROR::Invalid Session ID");
	}
	else
	{
	//	echo "POOP";
	}
	
	//$hostIP = $_SERVER['REMOTE_ADDR'];
	//echo "LOOL";
	$hostIP = $_GET['IP'];
	//if there are valid users
	if(sizeof($ValidUsers) > 0)
	{//Insert new matches row
		$sql = "INSERT INTO Matches (HostIP, HostID, ClientID) values ('$hostIP', '$UserID', '$ValidUsers[0]')";
		$result = mysqli_query($connection, $sql);
		if($result)
		{
			//echo "Successfully created new match";
			//Remove all users from queue that match the current userID
			$sql = "DELETE FROM Queue Where UserID = $UserID";
			$result = mysqli_query($connection, $sql);
			if(!$result)
			{
				echo "Failed to remove self from queue".mysqli_error($connection)."\n";
			}
			//Remove all users from queue that match the other player in matchup
			$sql = "DELETE FROM Queue WHERE UserID = $ValidUsers[0]";
			$result = mysqli_query($connection, $sql);
			if(!$result)
			{
				echo "Failed to remove other player from queue".mysqli_error($connection);
			}
			PrintMatchData($hostIP);
		}
		else 
		{
			echo "Failed to create new match".mysqli_error($connection);
		}
	
		//echo "New match created::".$UserID."::".$hostIP."::".$ValidUsers[0];
	}
	else 
	{
		echo "NO VALID MATCHUPS";
	}
	//If match found
		//other player still in queue, save host IP address in match table
		//other player not in queue, return host IP address
}
?>
