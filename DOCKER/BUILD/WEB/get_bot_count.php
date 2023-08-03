<?php
# Includes the configuration file
include "config/config.php";

# Establishes a connection to the database
# Uses variables from "config/config.php"
# "SET NAMES utf8" is necessary to be Unicode-friendly
$dbConnection = new PDO("mysql:host=$dbHost;dbname=$dbName", $dbUser, $dbPass, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));

// Fetch the updated value of "botconnected"
$statement = $dbConnection->prepare("SELECT * FROM hosts where status = 'connected'");
$statement->execute();
$bot = $statement->rowCount();

$statement = $dbConnection->prepare("SELECT * FROM hosts");
$statement->execute();
$allbots = $statement->rowCount();

$botdisconnected = $allbots - $bot;

// Return the updated value as plain text
echo $bot . '/' . $botdisconnected;
?>