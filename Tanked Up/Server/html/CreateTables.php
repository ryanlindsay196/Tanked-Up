<?php include "DBinfo.inc"; ?>
<?php


$connection = CreateConnection();

$sql = "SELECT * FROM Users WHERE UserID = 1";

$sql = "DROP TABLE Queue";
$result = mysqli_query($connection, $sql);
$sql = "CREATE TABLE Queue( QueueID int NOT NULL AUTO_INCREMENT, UserID int NOT NULL, IsInMatch bool NOT NULL, IPAddress VARCHAR(16), PRIMARY KEY (QueueID), FOREIGN KEY (UserID) REFERENCES Users(UserID) )";		//QUERY
$result = mysqli_query($connection, $sql);
//$sql = "CREATE TABLE LeaderBoard( LeaderboardID int NOT NULL, StageID int NOT NULL, Time int NOT NULL, Score int NOT NULL, UserID int NOT NULL, FOREIGN KEY (UserID) REFERENCES Users(UserID), PRIMARY KEY (LeaderBoardID) )";
//$sql = "INSERT INTO LeaderBoard VALUES ( 1, 1, 60, 6027, 1 )";
$sql = "DROP TABLE Matches";
$result = mysqli_query($connection, $sql);
$sql = "CREATE TABLE Matches( MatchID int NOT NULL AUTO_INCREMENT, HostIP VARCHAR(16), HostConnected bit, ClientConnected bit, HostID int NOT NULL, ClientID int NOT NULL, HostKills int, HostDeaths int, ClientKills int, ClientDeaths int, HostWin bit, ClientWin bit, PRIMARY KEY(MatchID))";
$result = mysqli_query($connection, $sql);

//$sql = "ALTER TABLE Matches ADD HostKills int, HostDeaths int, ClientKills, ClientDeaths int, HostWin bit, ClientWin bit";
//$result = mysqli_query($connection, $sql);

//$sql = "SELECT * from Queue";
//$result = my
//echo json_encode(mysqli_query($connection, $sql));
//$sql = "SELECT * FROM Users WHERE UserID = 1";		//QUERY
//$result = mysqli_query($connection, $sql);
echo json_encode($result).mysqli_error($connection);
mysqli_close($connection);
?>
