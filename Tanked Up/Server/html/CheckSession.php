
<?php
include "DBinfo.inc";
$connection = CreateConnection();
$sessionID = $_GET['SessionID'];
$sessionID = "'".$sessionID."'";
$SessionValid = CheckSession($sessionID, $connection);

echo $SessionValid;
if($SessionValid == "TRUE")
{
	UpdateKeepAlive($sessionID, $connection);
}
//echo "TRUE";

mysqli_close($connection);
?>
