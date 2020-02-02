<?php include "DBinfo.inc"; ?>
<?php
$connection = CreateConnection();

$UserName = $_GET['username'];
$Password = $_GET['Password'];
$UserName = "'".$UserName."'";
$Password = "'".$Password."'";
//echo "$UserName";
//Check for valid user
$sql = "SELECT UserName FROM Users WHERE UserName = $UserName AND Password = $Password";
$result = mysqli_query($connection, $sql);

if($result)
{//if User found in database with matching UserName and Password

        $dateInfo = getdate();
	$minutes = $dateInfo['minutes'];
	$seconds = $dateInfo['seconds'];
	$sessionID = "'".hash('ripemd160', "$UserName $Password $minutes $seconds", false)."'";
	$sql = "UPDATE Users SET SessionString = $sessionID WHERE UserName = $UserName AND Password = $Password";
	$result = mysqli_query($connection, $sql);
	if($result)
	{//SessionString updated in users table
		$sql = "SELECT SessionString FROM Users WHERE UserName = $UserName AND Password = $Password";
		$result = mysqli_query($connection, $sql);
		$sessionQuery = json_encode(mysqli_fetch_array($result, MYSQL_ASSOC));
		echo $sessionQuery;
		$sessionString = json_decode($sessionQuery, true)['SessionString'];
		$sessionString = "'".$sessionString."'";
		//echo $sessionString['SessionString'];
		UpdateKeepAlive($sessionString, $connection);
		//$tempDate = date("Y-m-d H:i:s");
		//$tempDate = "'".$tempDate."'";
		//
        //$day = ReformatDateStrings($dateInfo['mday']);
        //$month = ReformatDateStrings($dateInfo['mon']);
        //$year = ReformatDateStrings($dateInfo['year']);
        //$hour = ReformatDateStrings($dateInfo['hours']);
        //$min = ReformatDateStrings($dateInfo['minutes']);
        //$sec = ReformatDateStrings($dateInfo['seconds']);
		//
		//
        //$dateString = "$year$month$day$hour$min$sec";
		//$dateString = "'".$dateString."'";
		////echo $dateString;
		////echo $sessionString;
		//$tempDate = "'".$tempDate."'";
		////$sql = "UPDATE Users SET KeepAliveDate = $tempDate WHERE SessionString = $sessionString";
		//$sql = "UPDATE Users SET KeepAliveDate = $dateString WHERE SessionString = $sessionString";
	    //$result = mysqli_query($connection, $sql);
		CreateSession($sessionString);
	}
	else
	{
		echo "Session string not updated in users table";
	}
}
else
{
	echo "Username and password not found";
}

?>
