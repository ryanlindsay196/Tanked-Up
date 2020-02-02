<?php include "DBinfo.inc"; ?>

<?php

$connection = CreateConnection();


//$dateInfo = getdate();
//$month = ReformatDateStrings($dateInfo['mon']);
//$sec = ReformatDateStrings($dateInfo['seconds']);
//$mday = ReformatDateStrings($dateInfo['mday']);
//$hrs = ReformatDateStrings($dateInfo['hours']);
//$tempDate = date("Y-m-d H:i:s");
//$time = strtotime($tempDate);
//$time = $time - (30 * 60);//subtract seconds. Multiplying by 60 changes it to minutes
//$tempDate = date("Y-m-d H:i:s", $time);
////echo $tempDate;
//$tempDate = "'".$tempDate."'";


//Check that the Session is valid
$SessionID = $_GET['SessionID'];
CheckSession($SessionID, $connection);
//echo $SessionID;
$sql = "SELECT UserID FROM Users WHERE SessionString = '$SessionID' AND KeepAliveDate > '$tempDate'";
$result = mysqli_query($connection, $sql);
$userID = intval(mysqli_fetch_array($result, MYSQL_ASSOC)['UserID']);
if(!$result)
{
	die("No valid user found with sessionID".mysqli_error($connection));
}

$SessionID ="'". $_GET['SessionID']."'";
//Update Users.KeepAliveDate
UpdateKeepAlive($SessionID, $connection);


//If no valid matches
$sql = "SELECT * FROM Matches WHERE HostID = '$userID' OR ClientID = '$userID'";
$result = mysqli_query($connection, $sql);
if(mysqli_num_rows($result) == 1)
{
	die("User already in match");
}
else if(mysqli_num_rows($result) > 1)
{
	die("User is in multiple matches. This should not happen");
}

//Find if user is in queue
$sql = "SELECT * FROM Queue WHERE UserID = '$userID'";
$result = mysqli_query($connection, $sql);

//If user is not in queue, add it
if(mysqli_num_rows($result) == 0)
{
	//Insert the UserID and users IP address into the table
	$ip = "'".$_SERVER['REMOTE_ADDR']."'";
	$sql = "INSERT INTO Queue (UserID, IsInMatch, IPAddress) VALUES ('$userID', 0, $ip)";
	$result = mysqli_query($connection, $sql);
	if($result)
	{
		echo "Added user to queue::UserID = ".$userID;
	}
	else
	{
		echo "User not in queue and could not add to queue::".mysqli_error($connection)."::$userID";
		//echo "Could not add user to queue" . mysqli_error($connection);
	}
}
else
{//User is in queue already
	//echo "You are already logged in";
	echo "User already in queue";
}


//DEBUG: Display Queue
$sql = "SELECT * FROM Queue";
$result = mysqli_query($connection, $sql);
$return_arr = array();

//while($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
//	array_push($return_arr, $row);
//}

//echo json_encode($return_arr);


//$sql = "
mysqli_close($connection);
?>
