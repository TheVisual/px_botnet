<?php
# Includes the configuration file
include "config/config.php";

# Establishes a connection to the database
# Uses variables from "config/config.php"
# "SET NAMES utf8" is necessary to be Unicode-friendly
$dbConnection = new PDO("mysql:host=$dbHost;dbname=$dbName", $dbUser, $dbPass, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));


// Delete non-connected hosts from the database
$sql = "DELETE FROM hosts WHERE status != 'connected'";
$stmt = $dbConnection->prepare($sql);
$stmt->execute();

// Close the database connection
$dbConnection = null;

header("Location: hosts.php");
exit();
?>