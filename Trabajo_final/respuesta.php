<html>
<head>
  <meta charset="utf-8">
  <title>Sistema de Seguridad</title>
  <link href="css/bootstrap.min.css" rel="stylesheet">
  <link href="css/myStyle.css" rel="stylesheet">

</head>
<body style="background: #110011">
    <div class="container myDiv">
        <div class="row content">
          <div class="col-sm-4 sidenav">
            <img src="escudo.png">
          </div>
           <div class="col-sm-8 sidenav">
             <h1>SQRTY System</h1>
           Command is <?php echo $_POST["optradio"]; ?><br>
           command is <?php echo $_POST["password"]; ?>
           <?php
           $command=$_POST["optradio"];
           $a = "python C:\wamp\www\Trabajo_final\client.py ";
           $b= $a.$command;
           $last_line = system($b, $retval);
           echo '
           </pre>
           <hr />Last line of the output: ' . $last_line .
           '<a href="funciones.php" class="btn btn-primary btn-lg">Continuar</a>';
           ?>
        </div>
      </div>
  </div>
</body>
</html>
