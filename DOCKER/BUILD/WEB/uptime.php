<?php
$str = @file_get_contents('/proc/uptime');
$num = floatval($str);
$secs = fmod($num, 60);
$num = intdiv($num, 60);
$mins = $num % 60;
$num = intdiv($num, 60);
$hours = $num % 24;
$num = intdiv($num, 24);
$days = $num;

$data = array(
  "days" => $days,
  "hours" => $hours,
  "mins" => $mins,
  "secs" => round($secs)
);

header('Content-Type: application/json');
echo json_encode($data);
?>

