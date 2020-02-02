<?php include "DBinfo.inc"; ?>
<?php

$connection = CreateConnection();

$sql = "SELECT * FROM Matches";
$result = mysqli_query($connection, $sql);
$return_arr = array();
//Loop through Matches data
while($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
	array_push($return_arr, $row);
}
echo json_encode($return_arr);

$sql = "SELECT * FROM Queue";
$result = mysqli_query($connection, $sql);
$return_arr = array();
while($row = mysqli_fetch_array($result, MYSQL_ASSOC))
{
	array_push($return_arr, $row);
}

echo json_encode($return_arr);

mysqli_close($connection);
?>