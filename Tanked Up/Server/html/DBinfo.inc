<?php

define('DB_SERVER', 'tank-db-instance-2.cptxqfhuqqxb.us-east-1.rds.amazonaws.com');
define('DB_USERNAME', 'admin');
define('DB_PASSWORD', 'vCtd5RUfVGgKHEchm94R');
define('DB_DATABASE', 'TankDB');

function CheckSession($sessionString, $in_connection)
{
	$dateInfo = getdate(); 

	$day = ReformatDateStrings($dateInfo['mday']);
	$month = ReformatDateStrings($dateInfo['mon']);
	$year = ReformatDateStrings($dateInfo['year']);
	$hour = ReformatDateStrings($dateInfo['hours']);
	$min = ReformatDateStrings($dateInfo['minutes']);
	$sec = ReformatDateStrings($dateInfo['seconds']);

	$tempDate = date("Y-m-d H:i:s");
	$time = strtotime($tempDate);
	$time = $time - (30 * 60);//subtract seconds. Multiplying by 60 changes it to minutes
	$tempDate = date("Y-m-d H:i:s", $time);
	$tempDate = "'".$tempDate."'";

	$sql = "SELECT UserName, KeepAliveDate FROM Users WHERE SessionString = $sessionString AND KeepAliveDate > $tempDate"; 
	$result = mysqli_query($in_connection, $sql);
	//echo "PPO".mysqli_error($in_connection);
	//$KeepAliveDate = json_encode(mysqli_fetch_array($result, MYSQL_ASSOC)); 
	//echo $KeepAliveDate;
	if(mysqli_num_rows($result) < 1)
	{
		return "FALSE";
	}

	UpdateKeepAlive($sessionString);
	return "TRUE";

}

function UpdateKeepAlive($sessionString, $in_connection)
{
	$dateInfo = getdate();

	$day = ReformatDateStrings($dateInfo['mday']);
	$month = ReformatDateStrings($dateInfo['mon']);
	$year = ReformatDateStrings($dateInfo['year']);
	$hour = ReformatDateStrings($dateInfo['hours']);
	$min = ReformatDateStrings($dateInfo['minutes']);
	$sec = ReformatDateStrings($dateInfo['seconds']);

	$dateString = "$year$month$day$hour$min$sec";
	$dateString = "'".$dateString."'";
	$sql = "UPDATE Users SET KeepAliveDate = $dateString WHERE SessionString = $sessionString";
	mysqli_query($in_connection, $sql);
}

function CreateConnection()
{
	//Create conection
	$connection = mysqli_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD);
	//Check connection
	if(!$connection){
	        die("Connection failed: " . mysqli_connect_error());
	}

	mysqli_select_db($connection, "TankDB");

	return $connection;
}

function ReformatDateStrings($in_String)
{
        if(strlen($in_String) <= 1)
        {
                //echo "Reformated";
                $in_String = "0" . "$in_String";
        }
        else
        {
                //echo "Not formated";
        }
        return $in_String;
}

?>