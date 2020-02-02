<?php include "DBinfo.inc"; ?>
<?php 


//Put username into variable 
$UserName = $_GET['username']; 
//Check that username is unique
$connection = CreateConnection();

$sql = "SELECT UserName FROM Users WHERE UserName = '$UserName'"; 
$result = mysqli_query($connection, $sql); 
$userData = json_encode(mysqli_fetch_array($result, MYSQL_ASSOC));

$sesionString = "";
//Create new entry with password
if(mysqli_num_rows($result) < 1)
{//if username is available
	//echo "UserName is available";
	$newPassword = $_GET['Password'];

	$dateInfo = getdate();
	$day = ReformatDateStrings($dateInfo['mday']);
	$month = ReformatDateStrings($dateInfo['mon']);
	$year = ReformatDateStrings($dateInfo['year']);
	$hour = ReformatDateStrings($dateInfo['hours']);
	$min = ReformatDateStrings($dateInfo['minutes']);
	$sec = ReformatDateStrings($dateInfo['seconds']);

	$dateString = "$year$month$day$hour$min$sec";
	//Date format:  20110126143000 YYYYMMDDHHMMSS
	$sessionID = "'".hash('ripemd160', "$UserName $Password $minutes $seconds", false)."'";;
	//echo "$sec   POOP";
	//echo $dateString;
	$sql = "INSERT INTO Users (UserName, Password, Rank, Wins, Losses, Kills, Deaths, SessionString, KeepAliveDate) VALUES ('$UserName', '$newPassword', 0, 0, 0, 0, 0, '$sessionID', '$dateString')";
	//$sql = "INSERT INTO Users (UserName, Password, Rank, Wins, Losses, Kills, Deaths) VALUES ('$UserName', '$newPassword', 0, 0, 0, 0, 0)";
	$result = mysqli_query($connection, $sql);
	if($result)
	{
		$sql = "SELECT UserName, SessionString FROM Users WHERE UserName = '$UserName'";
		$result = mysqli_query($connection, $sql);
		echo json_encode(mysqli_fetch_array($result, MYSQL_ASSOC));
	}
	UpdateKeepAlive($sessionString, $connection);
}
else
{//UserName is not available
	die("Username not available");
}
//Log in
//$sessionID = ;

//Create session ID

?>
