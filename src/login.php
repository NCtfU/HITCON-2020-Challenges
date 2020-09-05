<?php

if (isset($_POST['username']) && isset($_POST['password'])) {
	echo $_POST['username'] . ' ' . $_POST['password'];
} else {
	highlight_file(__FILE__);
}