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
          <form action="ConexionEstablecida.php" method="post">
            <p>Digite contraseña maestra:</p>
            <input type="password" style="background: #808080; color: black;" class="form-control"  name="password"  required><br>
            <input type="submit" class="btn btn-primary btn-lg" text="Enviar">
          </form>
        </div>
      </div>
    </div>
</body>
</html>
