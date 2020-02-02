<?php
$dbname = "TankDB";
$servername = "tank-db-instance-2.cptxqfhuqqxb.us-east-1.rds.amazonaws.com";
$username = "admin";
$password = "vCtd5RUfVGgKHEchm94R";

if(!mysql_connect($servername, $username, $password))
{
	echo 'Could not connect to mysql';
	exit;
}
mysql_select_db($dbname);
//$sql = "SELECT * FROM TankDB";
$sql = "Show tables in TankDB";//Show tables
$sql = "Show COLUMNS in Matches";//Show columns
$result = mysql_query($sql);

if(!$result)
{
	echo "DB Error, could not list columns\n";
	echo 'MySQL Error: ' . mysql_error();
	exit;
}

if(mysql_num_rows($result) > 0)
{
	while ($row = mysql_fetch_assoc($result))
	{
		//echo "$row \n";
		print_r(json_encode($row));
	}
}

//while ($row = mysql_fetch_row($result))
//{
//	echo "Table: {$row[0]}\n";
//}

mysql_free_result($result);

?>
