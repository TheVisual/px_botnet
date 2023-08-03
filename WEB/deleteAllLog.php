<?php
# Includes the configuration file
include "config/config.php";

# Establishes a connection to the database
# Uses variables from "config/config.php"
# "SET NAMES utf8" is necessary to be Unicode-friendly
$dbConnection = new PDO("mysql:host=$dbHost;dbname=$dbName", $dbUser, $dbPass, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));

# Deletes all rows from the "output" table
$statement = $dbConnection->prepare("DELETE FROM output");
$statement->execute();

# Kills database connection
$statement->connection = null;

# Redirects back to the output page
header("Location: output.php");
exit();
?>