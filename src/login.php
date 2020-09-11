<?php

require_once('config.php');

session_start();

if (isset($_SESSION['username'])) {
	header("Location: index.php");
}

if (isset($_POST['username']) && isset($_POST['password'])) {

	$username = $_POST['username'];
	$password = $_POST['password'];

	$sql = "SELECT username, password FROM users WHERE username='$username' and password='$password'";
	error_log($sql);
	$result = mysqli_query($conn, $sql);
	$row = mysqli_fetch_array($result, MYSQLI_ASSOC);

	$count = mysqli_num_rows($result);
	
	if ($count == 1) {
		$_SESSION['username'] = $row['username'];
		?>
		<script>
			alert('login success');
			location.href = 'chat.php';
		</script>
		<?php
	} else {
		?>
		<script>
			alert('login failed');
			location.href = 'index.php';
		</script>
		<?php
	}
	?>
	<?php
	mysqli_close($conn);
} else {
	highlight_file(__FILE__);
}

?>
