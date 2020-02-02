<html>
<head>
<title>HELLO</title>
</head>
<body>
<?php include "DBinfo.inc"; ?>
<?php


//Create conection
$connection = CreateConnection();

//Create table
//$sql = "INSERT INTO Users (UserName, Password, Rank, Wins, Losses, Kills, Deaths) VALUES ('Robert', 'Dieterich', 1, 2,02, 3000, 0)";
//$sql = "DELETE FROM Users WHERE UserID = 5";
//$sql = "UPDATE Users SET UserID = 3 WHERE UserID = 5";
//if(mysqli_query($connection, $sql))
//{
//	echo "Inserted data successfully";
//}

//if(!$result) 
//{
//	echo 'Could not run query: ' . mysql_error();
//	exit;
//}

$sql = "SELECT * FROM Users";
$result = mysqli_query($connection, $sql);
if(!$result)
{
	echo("Query failed" . mysql_error());
}
//print_r($result);
if(mysqli_num_rows($result) > 0) 
{
	while ($row = mysqli_fetch_assoc($result)) 
	{
		print_r(json_encode($row));
		//echo "$row";
		//print_r($row);
	}
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
</body> 
</html>
