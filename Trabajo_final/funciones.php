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
          <form action="respuesta.php" method="post">
            <p>Seleccione la acción a realizar</p>
            <table class="table" style="color: #AAAAAA;">
              <tr>
                <th><input type="radio" name="optradio" value="A" checked>Activar Alarma</th>
                <th><input type="radio" name="optradio" value="B">Desactivar Alarma</th>
              </tr>
                <tr>
                  <th><input type="radio" name="optradio" value="C">Abrir Garage</th>
                  <th><input type="radio" name="optradio" value="D">Cerrar Garage</th>
                </tr>
            </table>
              <p>Digite contraseña</p>
              <input type="password" style="background: #808080; color: black;" class="form-control"  name="password" required><br>
              <input type="submit" class="btn btn-primary btn-lg" text="Enviar">
          </form>
        </div>
      </div>
  </div>
</body>
</html>
