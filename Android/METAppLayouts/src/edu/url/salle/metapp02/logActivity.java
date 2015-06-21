package edu.url.salle.metapp02;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

/**
*Mostrara las tramas que son enviadas y recibidas en la comunicacion entre nuestro robot y el 
*telefono. Indicando el tipo de trama y la fecha de envio o recepcion de la misma.Extiende de activity
*/


public class logActivity extends Activity{
	TextView tv ;
	String log;
	
	
	
	/**
	*La funcion onCreate sera la encargada de inicializar la activity con el layout que deseamos en este caso
	*el layout log que se ha dise–ado especificamente para esta activity. Sera la encargada de inicializar
	*el textView y ejecutar la funcion LeerRegistro que no permitira visualizar el log.
	*@see LeerRegistro();
	*/
	
	
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.log);
        
	ImageButton volver;
	
	volver = (ImageButton) findViewById(R.id.button10);
	volver.setOnClickListener(new View.OnClickListener() {
       public void onClick(View v) {
  
    	   volver(null);
		
       }

    });
	
	
	tv= (TextView)findViewById(R.id.text);
	LeerRegistro();
	
}
	
	
	  
	private CharSequence toString(String line) {
		// TODO Auto-generated method stub
		return null;
	}

	
	/**
	*La funcion volver ejecutara un intent que nos permitira volver al main principal en cual 
	*se encuentra la pantalla de seleccion de modo.Se encargara de interrumpir los threads activos 
	*y ademas de escribir la en el log de comunicacion que se va a salir de la activity
	*@param View view La vista actual
	*@see ModificarRegistro()
	*/

	public void volver(View view) {
		   
		Intent intent = new Intent(this, 
               MainActivity.class);
		
startActivity(intent);
finish();

}
	
	
	/**
	*La clase SocketListener establece un socket de recepcion para poder recibir las tramas
	*procedentes del robot arduino en el sistema Android. Se utilizara como puerto receptor 
	*el 4563.Implementa Runnable
    *@throws IOException e Si no puede crear el socket de recepcion
	*/  
	
	class SocketListener implements Runnable
    {
          String str;
          public void run()
          {
                DatagramSocket socket;
                DatagramPacket packet;
                byte[] buf = new byte[256];
                System.out.println ("Thread running");
                try
                {
                     
                	  socket = new DatagramSocket (4563);
                      while (true)
                      {                          
                            packet = new DatagramPacket (buf, buf.length);
                            socket.receive (packet);
                            System.out.println ("Received packet");
                            String s = new String (packet.getData());
                            Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                      }
                }
                catch (IOException e)
                {
                      Log.e(getClass().getName(), e.getMessage());
                }
          }
    }
	 /**
	    *La funcion onKeyDown nos permite capturar la accion de tocar el boton volver de android, configurando las 
	    *acciones que deseamos que lleve a cabo. En nuestro caso queriamos volver a la activity anterior y ademas que
	    *destruya la activity que se esta ejecutando y mandar una trama con sendData() para informar al robot que hemos
	    *salido de esta activity
	    *@see sendData() volver()
	    *@param keyCode es el codigo de la tecla que pulsamos
	    *@param  KeyEvent event sera el evento de pulsar una tecla con el dedo
	    */ 	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if ((keyCode == KeyEvent.KEYCODE_BACK))
		{
				
			volver(null);
			return true;

		}
		
		return super.onKeyDown(keyCode, event);
	}
	
	/**
	*La funcion ModificarRegistro sera la encargada de a–adir en un fichero de texto para tramas
	*recibidas y enviadas entre Android y Arduino para que posteriormente puedan ser visualizadas 
	*en la activity de Log de comunicaciones
	*@throws Exception ex Si no puede crear el archivo de texto en el caso de que no exista o en  el caso 
	*de que exista que no se pueda sobreecribir
	*/
	
	 public boolean ModficarRegistro(){
			try
			{
			    OutputStreamWriter fout=
			        new OutputStreamWriter(
			            openFileOutput("prueba_int.txt", Context.MODE_APPEND));
			    Date curDate = new Date();
			    SimpleDateFormat formato = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			    
			    String hora = formato.format(curDate);
			    String cab=hora+log;
			    cab=cab+"\n";
			    
			    fout.write(cab);
			    fout.close();
			}
			catch (Exception ex)
			{
			    Log.e("Ficheros", "Error al escribir fichero a memoria interna");
			}
			return false;
			
			
			
			
		}
	 
	 /**
		*La funcion LeerRegistro sera la encargada de visualizar por pantalla el fichero de texto que
		*con anteriodad habiamos creado para almacenar las tramas recibidas y enviadas entre Android y Arduino
		*@throws Exception ex Si no se puede leer el fichero
		*@throws Exception e Si no se puede abrir el fichero y almacenar en un stream para poder posteriormente leerlo
		*/
	 
		public boolean LeerRegistro(){
		try
		{
		    BufferedReader fin =
		        new BufferedReader(
		            new InputStreamReader(
		                openFileInput("prueba_int.txt")));
		    StringBuilder text = new StringBuilder();
		    
		    String texto="";
		    try
            {
                while ((texto = fin.readLine()) != null)
                
                	
                	text.append(texto);
                text.append('\n');
                
                
                
            }catch (Exception e) 
            {
                e.printStackTrace();
            }
		   
		    tv.setText(text);
		  
		    fin.close();
		}
		catch (Exception ex)
		{
		    Log.e("Ficheros", "Error al leer fichero desde memoria interna");
		}
		return false;
		}	 
	 
	 
}
