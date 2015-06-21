package edu.url.salle.metapp02;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import edu.url.salle.metapp02.remotoActivity.SocketListener;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import android.graphics.Color;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;

import android.widget.ImageButton;

import android.widget.ImageView;

import android.widget.TextView;

import android.widget.Toast;
/**
*La clase accelActivity nos permite visualizar los valores que obtenemos del acelerometro de nuestro robot y
*ademas nos dara una idea de como se encuentra posicionado el mismo mediante una serie de indicadores luminosos.

*/
//public class accelActivity extends Activity implements SensorEventListener{
	public class accelActivity extends Activity {
	 int x,y,z;
	 String tramaType = "A";
	 String log;
	//private SensorManager sSensorManager; 
	//private Sensor mAccelerometer;
	 public float angle = 0;
	 private static TextView text2,text4,text6;
	 private static ImageView xpos,xneg,ypos,yneg,zpos,zneg;
	 String rxPacket;
	 Activity activityTest1;
     Thread t;
     Handler hand = new Handler();
	
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.accel);
      //  sSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE); 
      //  mAccelerometer = sSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        x=1;
        y=0;
        z=0;
      
        ImageButton Bvolver;
        xpos = (ImageView) findViewById(R.id.view10);
		xneg = (	ImageView) findViewById(R.id.view13);
		ypos = (	ImageView) findViewById(R.id.view11);
		yneg = (	ImageView) findViewById(R.id.view12);
		zpos = (	ImageView) findViewById(R.id.view14);
		zneg = (	ImageView) findViewById(R.id.view15);
        
        
        
        t = new Thread (new SocketListener ());
        
        t.start();
        
        activityTest1 = this;
		Bvolver = (	ImageButton) findViewById(R.id.button1);
		Bvolver.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   
        	   	tramaType="X"; //Kill the arduino process
				sendData();
				volver(null);
				
           }

        });
		tramaType="A";
		sendData();
		text2 = (TextView) findViewById(R.id.text2);
		text4 = (TextView) findViewById(R.id.text4);
		text6 = (TextView) findViewById(R.id.text6);
        
		

}
	

/*
	@Override 
    protected void onResume() { 
        super.onResume(); 
        sSensorManager.registerListener(this, mAccelerometer, Sensor.TYPE_ACCELEROMETER); 
       
    } 

    @Override 
    protected void onPause() { 
        super.onPause(); 
        sSensorManager.unregisterListener(this, mAccelerometer); 
    } 
    */
    /**
	*La funcion volver ejecutara un intent que nos permitira volver al main principal en cual 
	*se encuentra la pantalla de seleccion de modo.Se encargara de interrumpir los threads activos 
	*y ademas de escribir la en el log de comunicacion que se va a salir de la activity
	*@param View view La vista actual
	*@see ModificarRegistro()
	*/
	public void volver(View view) {	   
		t.interrupt();
		t=null;
	
		Intent intent = new Intent(this, 
        MainActivity.class);
		
		log=" Tx: XMODAC";
		ModificarRegistro(); 
		
		startActivity(intent);
		
		finish();
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
			tramaType="X"; //Kill the arduino process
			sendData();	
			volver(null);
			return true;

		}
		
		return super.onKeyDown(keyCode, event);
	}
	/**
   	*La clase parseRXstring es la encargada de traducir la trama recibida por parte 
   	*del sistema arduino y actualizar el estado de nuestros elementos UI como el TextView
   	*de la temperatura, los imageView de deteccion o presencia de obstaculos,etc
   	*/


	 private boolean parseRXstring(String data)
	    {
	    	
	    	String posX = null;
	    	String posY = null;
	    	String posZ= null;
	    	
				int x = 0;
				int y = 0;
				int z = 0;
				try {
					//A00.09 1-0.09
					//Type-SignoX-PosX-Space-SignoY-PosY-Space-SignoZ-PosZ
					String type = data.substring(0, 1);
					
					posX = data.substring(2, 7);
					posY = data.substring(9, 14);
					posZ = data.substring(16, 21);
					
					x = Integer.parseInt(data.substring(1, 2));
					y = Integer.parseInt(data.substring(8, 9));
					z = Integer.parseInt(data.substring(15, 16));
					text2.setText(posX);
					text4.setText(posY);
					text6.setText(posZ);
					
				} catch (Exception e) {
					// TODO: handle exception
				}
				if (x == 1) {
					xpos.setBackgroundColor(Color.BLACK);
					xneg.setBackgroundColor(Color.RED);
				} else if (x == 0) {
					xpos.setBackgroundColor(Color.RED);
					xneg.setBackgroundColor(Color.BLACK);
				}
				if (y == 0) {
					ypos.setBackgroundColor(Color.BLACK);
					yneg.setBackgroundColor(Color.RED);

				} else if (y == 0) {
					ypos.setBackgroundColor(Color.RED);
					yneg.setBackgroundColor(Color.BLACK);

				}
				if (z == 0) {
					zpos.setBackgroundColor(Color.BLACK);
					zneg.setBackgroundColor(Color.RED);

				} else if (z == 0) {
					zpos.setBackgroundColor(Color.RED);
					zneg.setBackgroundColor(Color.BLACK);

				}
			
			return false;
	    }	

	
	 /**
 	*La clase SendData establece un socket de emision para poder enviar tramas
 	*desde el sistema Android al robot arduino.Ademas realizara el parseado de la
 	*informacion en un formato que sea entendido por nuestro robot.Para poder realizar 
 	*el envio se debera crea un thread. Se utilizara como puerto emisor 55056
 	*@see ModificarRegistro()
	    *@throws IOException e  Si el socket no se puede crear el socket o no se puede enviar la trama
	    *@throws UnknownHostException e Si el socket no se puede crear el socket
	    *@throws SocketException e  Si el socket no se puede crear el socket
 	*/
	public boolean sendData(){
		try {
			final DatagramSocket socket = new DatagramSocket ();
			socket.setReuseAddress(true);
			InetAddress address;
			address = InetAddress.getByName ("172.20.10.9");
			
			
			byte[] buf = new byte[256];
	      
			String stemp;
			stemp=tramaType;
		
			
			
			buf = stemp.getBytes ();
			log=" Tx: "+stemp;
			ModificarRegistro();
	        
	        
			        
	        final DatagramPacket packet = new DatagramPacket (buf, buf.length, address, 55056);
			
	        new Thread ()
            {
                  public void run ()
                  {
                        try
                        {
                             
                              socket.send (packet);
                              
                        }
                        catch (IOException e1)
                        {
                              // TODO Auto-generated catch block
                              e1.printStackTrace();
                        }
                        socket.close();
                  }

            }.start ();
	        
            return true;
            
		}catch (SocketException e) {e.printStackTrace();
		}catch (UnknownHostException e) {e.printStackTrace();
		}catch (IOException e) {e.printStackTrace();
		}      
		return false;
	}
	
	
	
	/**
	*La clase SocketListener establece un socket de recepcion para poder recibir las tramas
	*procedentes del robot arduino en el sistema Android. Se utilizara como puerto receptor 
	*el 4562.Implementa Runnable
    *@throws IOException e Si no puede crear el socket de recepcion
	*/ 
	
	
	class SocketListener implements Runnable
    {
          String str;
          public void run()
          {
                DatagramSocket socket;
                DatagramPacket packet;
                byte[] buf = new byte[22];
                System.out.println ("Thread running");
                try
                {
                      socket = new DatagramSocket (4562);
                      socket.setReuseAddress(true);
                	 
                      while (true)
                      {                          

                            
                           packet= new DatagramPacket(buf,22);
                    	  socket.receive (packet);
                            rxPacket = new String (packet.getData());
                            activityTest1.runOnUiThread(new Runnable(){
                            	public void run(){
                            		parseRXstring(rxPacket);
                            		log=" Rx: "+rxPacket;
                            		ModificarRegistro();
                            	}
                            });
                          
                            

                      }
                }
                catch (IOException e)
                {
                      Log.e(getClass().getName(), e.getMessage());
                }
          }
    }
/*
	
	@Override
	public void onSensorChanged(SensorEvent event) {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}
*/
	/**
	*La funcion ModificarRegistro sera la encargada de a–adir en un fichero de texto para tramas
	*recibidas y enviadas entre Android y Arduino para que posteriormente puedan ser visualizadas 
	*en la activity de Log de comunicaciones
	*@throws Exception ex Si no puede crear el archivo de texto en el caso de que no exista o en  el caso 
	*de que exista que no se pueda sobreecribir
	*/

	public boolean ModificarRegistro(){
		try
		{
		    OutputStreamWriter fout=
		        new OutputStreamWriter(
		            openFileOutput("prueba_int.txt", Context.MODE_APPEND));
		    Date curDate = new Date();
		    SimpleDateFormat formato = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		    
		    String hora = formato.format(curDate);
		    String esp;
		    //esp=hora+" Tx: ";
		    esp=hora;
		    String cab=esp+log;
		    cab=cab+"                             \n\n";
		    
		    
		    fout.write(cab+"\n");
		    fout.close();
		}
		catch (Exception ex)
		{
		    Log.e("Ficheros", "Error al escribir fichero a memoria interna");
		}
		return false;
		
		
		
		
	}	
	
}
