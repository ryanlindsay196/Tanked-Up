<?php include "DBinfo.inc"; ?>
<?php
$connection = CreateConnection();

$sessionString = $_GET['SessionID'];

//Find current userID from session string
$sql = "SELECT UserID FROM Users WHERE SessionString = '$sessionString'";
$result = mysqli_query($connection, $sql);
$userID = mysqli_fetch_array($result)['UserID'];

//Check if current user is host or client
$sql = "SELECT * FROM Matches WHERE HostID = '$userID' OR ClientID = '$userID'";
$result = mysqli_query($connection, $sql);
$row = mysqli_fetch_array($result);
$isHost = true;
if($row['ClientID'] == $userID)
{
	$isHost = false;
}

//If player is host, set isHostConnected to true
if($isHost)
{
	$sql = "UPDATE Matches SET HostConnected = 'true'";
}
//If player is client, set isClientConnected to true;
if(!$isHost)
{
	$sql = "UPDATE Matches SET ClientConnected = 'true'";
}
$result = mysqli_query($connection, $sql);

//Requery current match
$sql = "SELECT * FROM Matches WHERE HostID = '$userID' OR ClientID = '$userID'";
$result = mysqli_query($connection, $sql);
$row = mysqli_fetch_array($result);

//If both players are connected, return true
if($row['HostConnected'] && $row['ClientConnected'])
{
	echo "connected";
}
//If not all players are connected, return false
else {
	echo "not connected";
}

?>