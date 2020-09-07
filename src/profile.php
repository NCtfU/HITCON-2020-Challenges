<?php

session_start();
if (!isset($_SESSION['username'])) {
  header("Location: index.php");
}
require_once('config.php');

?>

<!doctype html>
<html lang="en">
  <head>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.3/js/bootstrap.min.js" integrity="sha384-a5N7Y/aK3qNeh15eJKGWxsqtnX/wWdSZSKp+81YjTmS15nvnvxKHuzaWwXHDli+4" crossorigin="anonymous"></script>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.3/css/bootstrap.min.css" integrity="sha384-Zug+QiDoJOrZ5t4lssLdxGhVrurbmBWopoEl+M6BdEfwnCJZtKxi1KgxUyJq13dy" crossorigin="anonymous">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <link rel="stylesheet" href="style/chat.css">
  </head>
  <body>
  <div class="container">
    <div class="row mt-5">
      <div class="col-md-8 offset-md-2 col-sm-8 offset-sm-2 col-12 comments-main pt-4 rounded">

        <?php
          function show_meta_info($name, $info, $msg_num) {
            if ($info['account_type'] === 0) {
              $ac_type = 'Admin';
            } elseif ($info['account_type'] === 1) {
              $ac_type = 'User';
            } else {
              $ac_type = '???';
            }
            ?>
          <div class="row profile mb-4">
            <div class="col-md-4 col-sm-4 col-4 text-center">
              <img src="res/<?= $info['icon']?>" />
            </div>
            <div class="col-md-8 col-sm-8 col-8 rounded info-box mb-2">
              <h2><?= $name?></h2>
              <h6>最後上線：<?= $info['last_active']?> 天前</h6>
              <h6>總留言數：<?= $msg_num?> 則</h6>
              <h6>帳戶類型：<?= $ac_type?></h6>
            </div>
          </div>
          <hr>
          <div class="mt-5"></div>
            <?php
          }

          function show_message($row, $icon) {
            if ($row['deleted'] !== '1') {
              $message = $row['msg'];
            } else {
              $message = '[已刪除訊息] ';
              if ($_SESSION['username'] === $row['name']) {
                $message = $message . $row['msg'];
              }
              $message = '<span class="deleted">' . $message . '</span>';
            }
            ?>
        <ul class="p-0">
          <li>
            <div class="row comments mb-2">
              <div class="col-md-2 col-sm-2 col-3 text-center user-img">
                <img id="profile-photo" src="res/<?= $icon?>" class="rounded-circle" />
              </div>
              <div class="col-md-9 col-sm-9 col-9 comment rounded mb-2">
                <h4 class="m-0"><a href="profile.php?name=<?= $row['name']?>"><?= $row['name']?></a></h4>
                <time class="text-white ml-3"><?= $row['time']?> 天前</time>
                <like></like>
                <p class="mb-0 text-white"><?= $message?></p>
              </div>
            </div>
          </li>
        </ul>
            <?php
          }

          if (isset($_GET['name'])) {
            $name = $_GET['name'];

            // fetch user info
            $sql = "SELECT icon, account_type, last_active FROM chat.users WHERE username = ?";
            $stmt = mysqli_prepare($conn, $sql);
            mysqli_stmt_bind_param($stmt, 's', $name);
            mysqli_stmt_execute($stmt);
            $result = mysqli_stmt_get_result($stmt);
            $info = mysqli_fetch_assoc($result);

            // fetch message history (with injection :-)
            $result = mysqli_query($conn, "SELECT * FROM chat.message WHERE name = '$name'");

            show_meta_info($name, $info, mysqli_num_rows($result));

            while ($row = mysqli_fetch_assoc($result)) {
              show_message($row, $info['icon']);
            }
          }
        ?>

      </div>
    </div>
  </div>
  <p class="ml-3 footer">Icons made by <a href="https://www.flaticon.com/authors/those-icons" title="Those Icons">Those Icons</a> from <a href="https://www.flaticon.com/" title="Flaticon"> www.flaticon.com</a></p>
  </body>
</html>