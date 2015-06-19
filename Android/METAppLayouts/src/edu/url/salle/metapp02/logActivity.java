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
*telefono. Indicando el tipo de trama y la fecha de envio o recepcion de la misma.
*/


public class logActivity extends Activity{
	TextView tv ;
	String log;
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.log);
        
	ImageButton volver;
	
	volver = (ImageButton) findViewById(R.id.button10);
	volver.setOnClickListener(new View.OnClickListener() {
       public void onClick(View v) {

    	   
            // Perform action on click
        	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
			//toast1.show();
    	   volver(null);
			
			
       // 	(null);
       }

    });
	/*
	try {
        Process process = Runtime.getRuntime().exec("logcat -d");
        BufferedReader bufferedReader = new BufferedReader(
        new InputStreamReader(process.getInputStream()));

        StringBuilder log=new StringBuilder();
        String line = ""; 
        
        while ((line = bufferedReader.readLine()) != null) {
          log.append(line+"\n");
        }   
        TextView tv = (TextView)findViewById(R.id.text);
        tv.setText(log.toString());
      } catch (IOException e) {
    }*/
	
	tv= (TextView)findViewById(R.id.text);
	LeerRegistro();
	
}
	
	
	  
	private CharSequence toString(String line) {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	*La funcion volver ejecutara un intent que nos permitira volver al main principal en cual 
	*se encuentra la pantalla de seleccion de modo.
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
	*el 4560
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
                      //socket = new DatagramSocket (4560);
                	  socket = new DatagramSocket (55056);
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
		public boolean LeerRegistro(){
		try
		{
		    BufferedReader fin =
		        new BufferedReader(
		            new InputStreamReader(
		                openFileInput("prueba_int.txt")));
		    StringBuilder text = new StringBuilder();
		    // String texto= fin.readLine();
		    String texto="";
		    try
            {
                while ((texto = fin.readLine()) != null)
                   // line1+=line;
                	
                	text.append(texto);
                text.append('\n');
                
                
                
            }catch (Exception e) 
            {
                e.printStackTrace();
            }
		   
		    tv.setText(text);
		  //  tv.setText(texto);
		   // System.out.println (texto);
		    fin.close();
		}
		catch (Exception ex)
		{
		    Log.e("Ficheros", "Error al leer fichero desde memoria interna");
		}
		return false;
		}	 
	 
	 
}
