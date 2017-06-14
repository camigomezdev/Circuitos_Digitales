<html>
<head>
  <meta charset="utf-8">
  <title>Sistema de Seguridad</title>
  <link href="css/bootstrap.min.css" rel="stylesheet">
  <link href="https://fonts.googleapis.com/css?family=Arvo|Fjalla+One" rel="stylesheet">
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
             <?php
             $command=$_POST["password"];
             $a = "python C:\wamp\www\Trabajo_final\client.py ";
             $b= $a.$command;
             $last_line = system($b, $retval);
             echo '<h3>Last line of the output: ' . $last_line . '</h3>';
             if (false) {
                echo '
                <a href="signal.php" class="btn btn-danger btn-lg">Continuar</a>
                ';
             }else{
               echo '
               <a href="funciones.php" class="btn btn-primary btn-lg">Continuar</a>
               ';
             }
             ?>

          </div>
      </div>
  </div>
</body>
</html>
