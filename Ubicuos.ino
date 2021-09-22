# incluye  < WiFi.h >

// Reemplace con sus credenciales de red
const  char * ssid = " CRISTINA_ERAZO " ;
const  char * contraseña = " * LuisaP1999 * / " ;

// Establecer el número de puerto del servidor web en 80
Servidor WiFiServer ( 80 );

// Variable para almacenar la solicitud HTTP
Encabezado de cadena;

// Variables auxiliares para almacenar el estado de salida actual
String output26State = " off " ;
String output27State = " off " ;

// Asignar variables de salida a pines GPIO
const  int output26 = 26 ;
const  int output27 = 27 ;

// Hora actual
 tiempo actual largo sin firmar = milis ();
// Hora anterior
unsigned  long previousTime = 0 ;
// Definir el tiempo de espera en milisegundos (ejemplo: 2000ms = 2s)
const  long timeoutTime = 2000 ;

 configuración vacía () {
  De serie. comenzar ( 115200 );
  // Inicializar las variables de salida como salidas
  pinMode (salida26, SALIDA);
  pinMode (salida27, SALIDA);
  // Establecer salidas a BAJO
  digitalWrite (salida26, BAJA);
  digitalWrite (salida27, BAJA);

  // Conéctese a la red Wi-Fi con SSID y contraseña
  De serie. imprimir ( " Conectando a " );
  De serie. println (ssid);
  Wifi. comenzar (ssid, contraseña);
  while (WiFi. status ()! = WL_CONNECTED) {
    retraso ( 500 );
    De serie. imprimir ( " . " );
  }
  // Imprime la dirección IP local e inicia el servidor web
  De serie. println ( " " );
  De serie. println ( " WiFi conectado " );
  De serie. println ( " dirección IP: " );
  De serie. println (WiFi. localIP ());
  servidor. comenzar ();
}

 bucle vacío () {
  Cliente WiFiClient = servidor. disponible ();   // Escuche los clientes entrantes

  if (cliente) {                              // Si se conecta un nuevo cliente,
    currentTime = millis ();
    previousTime = currentTime;
    De serie. println ( " Cliente nuevo " );          // imprime un mensaje en el puerto serie
    String currentLine = " " ;                // crea una cadena para contener los datos entrantes del cliente
    while (client. connected () && currentTime - previousTime <= timeoutTime) {   // bucle mientras el cliente está conectado
      currentTime = millis ();
      if (client. available ()) {              // si hay bytes para leer del cliente,
        char c = cliente. leer ();             // lee un byte, luego
        De serie. escribir (c);                    // imprimirlo en el monitor de serie
        encabezado + = c;
        if (c == ' \ n ' ) {                     // si el byte es un carácter de nueva línea
          // si la línea actual está en blanco, tiene dos caracteres de nueva línea seguidos.
          // ese es el final de la solicitud HTTP del cliente, así que envíe una respuesta:
          if (currentLine. length () == 0 ) {
            // Los encabezados HTTP siempre comienzan con un código de respuesta (por ejemplo, HTTP / 1.1 200 OK)
            // y un tipo de contenido para que el cliente sepa lo que viene, luego una línea en blanco:
            cliente. println ( " HTTP / 1.1 200 OK " );
            cliente. println ( " Tipo de contenido: texto / html " );
            cliente. println ( " Conexión: cerrar " );
            cliente. println ();
            
            // enciende y apaga los GPIO
            if (header. indexOf ( " GET / 26 / on " )> = 0 ) {
              De serie. println ( " GPIO 26 activado " );
              output26State = " encendido " ;
              digitalWrite (salida26, ALTA);
            } else  if (header. indexOf ( " GET / 26 / off " )> = 0 ) {
              De serie. println ( " GPIO 26 desactivado " );
              output26State = " apagado " ;
              digitalWrite (salida26, BAJA);
            } else  if (header. indexOf ( " GET / 27 / on " )> = 0 ) {
              De serie. println ( " GPIO 27 activado " );
              output27State = " encendido " ;
              digitalWrite (salida27, ALTA);
            } else  if (header. indexOf ( " GET / 27 / off " )> = 0 ) {
              De serie. println ( " GPIO 27 desactivado " );
              output27State = " apagado " ;
              digitalWrite (salida27, BAJA);
            }
            
            // Mostrar la página web HTML
            cliente. println ( " <! DOCTYPE html> <html> " );
            cliente. println ( " <head> <meta name = \" viewport \ " content = \" width = device-width, initial-scale = 1 \ " > " );
            cliente. println ( " <link rel = \" icon \ " href = \" data :, \ " > " );
            // CSS para diseñar los botones de encendido / apagado
            // Siéntase libre de cambiar los atributos de color de fondo y tamaño de fuente para que se ajusten a sus preferencias
            cliente. println ( " <style> html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} " );
            cliente. println ( " .button {color de fondo: # 4CAF50; borde: ninguno; color: blanco; relleno: 16px 40px; " );
            cliente. println ( " decoración de texto: ninguna; tamaño de fuente: 30 px; margen: 2 px; cursor: puntero;} " );
            cliente. println ( " .button2 {background-color: # 555555;} </style> </head> " );
            
            // Encabezado de la página web
            cliente. println ( " <body> <h1> Servidor web ESP32 </h1> " );
            
            // Muestra el estado actual y los botones ON / OFF para GPIO 26  
            cliente. println ( " <p> GPIO 26 - Estado " + output26State + " </p> " );
            // Si output26State está desactivado, muestra el botón ON       
            if (output26State == " off " ) {
              cliente. println ( " <p> <a href= \" /26/on \"> <button class = \" button \ " > ON </button> </a> </p> " );
            } más {
              cliente. println ( " <p> <a href= \" /26/off \"> <button class = \" button button2 \ " > APAGADO </button> </a> </p> " );
            } 
               
            // Muestra el estado actual y los botones ON / OFF para GPIO 27  
            cliente. println ( " <p> GPIO 27 - Estado " + output27State + " </p> " );
            // Si output27State está apagado, muestra el botón ON       
            if (output27State == " off " ) {
              cliente. println ( " <p> <a href= \" /27/on \"> <button class = \" button \ " > ON </button> </a> </p> " );
            } más {
              cliente. println ( " <p> <a href= \" /27/off \"> <button class = \" button button2 \ " > APAGADO </button> </a> </p> " );
            }
            cliente. println ( " </body> </html> " );
            
            // La respuesta HTTP termina con otra línea en blanco
            cliente. println ();
            // Salir del bucle while
            romper ;
          } else { // si tienes una nueva línea, borra currentLine
            currentLine = " " ;
          }
        } else  if (c! = ' \ r ' ) {   // si tienes algo más que un carácter de retorno de carro,
          currentLine + = c;      // agregarlo al final de currentLine
        }
      }
    }
    // Limpiar la variable de encabezado
    encabezado = " " ;
    // Cerrar la conexión
    cliente. detener ();
    De serie. println ( " Cliente desconectado " );
    De serie. println ( " " );
  }
}
