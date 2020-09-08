<?php
$username = 'chat';
$password = 'nctfuchathc';
$dbname = 'chat';
$conn = mysqli_connect('mysql', $username, $password, $dbname);

if (mysqli_connect_errno()) {
    die("Failed to connect to MySQL: " . mysqli_connect_error());
}

?>