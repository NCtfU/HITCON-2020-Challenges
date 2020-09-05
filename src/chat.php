<?php

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
    <script src="script/chat.js"></script>
  </head>
  <body>
  <div class="container">
    <div class="row mt-5 mb-5">
      <div class="col-md-6 offset-md-3 col-sm-6 offset-sm-3 col-12 comments-main pt-4 rounded">

        <?php
          function show_message($row) {
            $msg = $row['msg'];

            // deletion check
            if ($row['deleted'] === '1') {
              $msg = '[已刪除訊息]';
            }

            // image replacement if checksum matched
            if ($msg > 10000) {
              $data = base64_decode($msg);
              if (md5($data) === '7efc02dbad7f35c170ff690c5b288126') {
                $msg = '<img src="data:image/jpg;base64,' . $data . '>';
              } else {
                error_log('Warning: Checksum of the image not matched!');
              }
            }

            $fmt = <<<EOF
        <ul class="p-0">
          <li>
            <div class="row comments mb-2">
              <div class="col-md-2 col-sm-2 col-3 text-center user-img">
                <img id="profile-photo" src="res/%s" class="rounded-circle" />
              </div>
              <div class="col-md-9 col-sm-9 col-9 comment rounded mb-2">
                <h4 class="m-0"><a href="profile.php?name=%s">%s</a></h4>
                <time class="text-white ml-3">%s 天前</time>
                <like></like>
                <p class="mb-0 text-white">%s</p>
              </div>
            </div>
          </li>
        </ul>
EOF;
            printf($fmt, $row['icon'], $row['name'], $row['name'], $row['time'], $msg);
          }

          function show_phonecall($row) {
            $fmt = <<<EOF
        <ul class="p-0">
          <li>
            <div class="row comments mb-2">
              <div class="col-md-9 col-sm-9 col-9 comment phone rounded mb-2">
                <img id="phone-icon" src="res/%s" class="rounded-circle mr-1" />
                <h5 class="m-0">通話已結束</h5><h6 class="ml-2">%s minutes</h6>
                <time class="text-white ml-3">%s 天前</time>
              </div>
            </div>
          </li>
        </ul>
EOF;
            printf($fmt, $row['icon'], $row['call_duration'], $row['time']);
          }

          // fetch messages
          $res = mysqli_query($conn, "SELECT * FROM chat.message");

          while ($row = mysqli_fetch_assoc($res)) {
            if ($row['isMessage'] === '1') {
              show_message($row);
            } else {
              show_phonecall($row);
            }
          }
          mysqli_close($conn);
        ?>

        <div class="row comment-box-main p-3 rounded-bottom">
          <div class="col-md-9 col-sm-9 col-9 pr-0 comment-box">
            <input type="text" class="form-control" placeholder="comment ...." />
          </div>
          <div class="col-md-3 col-sm-2 col-2 pl-0 text-center send-btn">
            <button class="btn btn-info">Send</button>
          </div>
        </div>
      </div>
    <form action='logout.php' method='GET'>
      <button type='submit' class="btn btn-warning">Logout</button>
    </form>
    </div>
  </div>
  <p class="ml-3 footer">Icons made by <a href="https://www.flaticon.com/authors/those-icons" title="Those Icons">Those Icons</a> from <a href="https://www.flaticon.com/" title="Flaticon"> www.flaticon.com</a></p>
  </body>
</html>