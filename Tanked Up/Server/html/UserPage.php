<html>
<head>
<title>Listing Users</title>
</head>
<body>
<?php
$dbhost = 'tank-db-instance-2.cptxqfhuqqxb.us-east-1.rds.amazonaws.com';
$dbuser = 'admin';
$dbpass = 'vCtd5RUfVGgKHEchm94R';
$connection = mysql_connect($dbhost, $dbuser, $dbpass);
if(! $connection ) {
	die('Could not connect: ' .mysql_error());
}
echo 'Connected successfully';

$sql = "CREATE DATABASE TankDB";
if(mysqli_query($connection, $sql)) {
	echo "Database created successfully\n";
} else {
	die("Error creating database: " . mysqli_error($connection) ."\n");
}

mysql_select_db("TankDB");

$sql = "CREATE TABLE Users( ".
	"UserID INT NOT NULL AUTO_INCREMENT, ".
	"UserName VARCHAR(100) NOT NULL, ".
	"Password VARCHAR(40) NOT NULL, ".
	"PRIMARY KEY ( UserID ));";

$retval = mysql_query($sql, $connection);

if(! $retval){
	die('Coult not create table: ' .mysql_error());
}
echo "Table created successfully\n";

mysql_close($connection);
?>
</body>
</html>
