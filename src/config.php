<?php
$username = 'root';
$password = 'nctfuchathc';
$dbname = 'chat';
$conn = mysqli_connect('mysql', $username, $password, $dbname);

if (mysqli_connect_errno()) {
    printf("Failed to connect to MySQL: " . mysqli_connect_error());
}

?>