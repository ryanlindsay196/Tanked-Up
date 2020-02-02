<?php include "DBinfo.inc"; ?>
<?php


$connection = CreateConnection();

//Create table //$sql = "INSERT INTO Users (UserName, Password, Rank, Wins, Losses, Kills, Deaths) VALUES ('Robert', 'Dieterich', 1, 2,02, 3000, 0)"; $sql = "DELETE FROM Users WHERE UserID > 4"; 
//$sql = "UPDATE Users SET UserID = 3 WHERE UserID = 5"; 
//$sql = "ALTER TABLE Users DROP KeepAliveDate"; 
//$sql = "ALTER TABLE Users ADD SessionString VARCHAR(64)";//, KeepAliveDate DATETIME NOT NULL DEFAULT '2011-01-24 14:30:00' AFTER Deaths"; 
//$sql = "ALTER TABLE Users ADD COLUMN KeepAliveDate DATETIME NOT NULL DEFAULT 20110126143000 AFTER SessionString"; 
if(mysqli_query($connection, $sql)) 
{ //	
echo "Inserted data successfully";
}
//else
//{
//	echo "Query Failed";
//}

//if(!$result) 
//{
//	echo 'Could not run query: ' . mysql_error();
//	exit;
//}
//$return_arr = array();

$sql = "SELECT * FROM Users ORDER BY Kills Desc";
//$sql = "DELETE FROM Users WHERE UserID > 4";
//$sql = "SELECT * FROM Users WHERE UserID > 1";
//$sql = "SELECT * FROM LeaderBoard";
$result = mysqli_query($connection, $sql);

$return_arr = array();

//Loop through Users data
while($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
//	$row_array['UserID'] = $row['UserID'];//Loop through users
//	$row_array['UserName'] = $row['UserName'];

	//array_push($return_arr, $row_array);
	array_push($return_arr, $row);
}

echo json_encode($return_arr);

$sql = "SELECT * FROM LeaderBoard";
$result = mysqli_query($connection, $sql);
echo "<br><hr>";
$return_arr = array();
//Loop through LeaderBoard data
while($row = mysqli_fetch_array($result, MYSQL_ASSOC)) {
	$row_array['StageID'] = $row['StageID'];
	$row_array['Time'] = $row['Time'];
	$row_array['Score'] = $row['Score'];
	$row_array['UserID'] = $row['UserID'];

	array_push($return_arr, $row_array);
}

echo json_encode($return_arr);

if(!$result)
{
	echo("Query failed" . mysql_error());
}

//if(mysqli_query($connection, $sql))
//{
//	echo "Inserted data successfully";
//}
//else
//{
//	echo "error inserting data: " . mysqli_error($connection);
//}

mysqli_close($connection); 
?>
