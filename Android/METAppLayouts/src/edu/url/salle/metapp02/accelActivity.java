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
public class accelActivity extends Activity implements SensorEventListener{
	int x,y,z;
	String tramaType = "A";
	String log;
	private SensorManager sSensorManager; 
	private Sensor mAccelerometer;
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
        sSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE); 
        mAccelerometer = sSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
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
        	   
                // Perform action on click
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
				//toast1.show();
        	   volver(null);
				
           }

        });
		tramaType="A";
		sendData();
		text2 = (TextView) findViewById(R.id.text2);
		text4 = (TextView) findViewById(R.id.text4);
		text6 = (TextView) findViewById(R.id.text6);
        
		

}
	


	@Override 
    protected void onResume() { 
        super.onResume(); 
        sSensorManager.registerListener(this, mAccelerometer, Sensor.TYPE_ACCELEROMETER); 
        //activityTest1.hand;
    } 

    @Override 
    protected void onPause() { 
        super.onPause(); 
        sSensorManager.unregisterListener(this, mAccelerometer); 
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
	    	
	    	//C34N0M0N
	    	//Type-Temp1-Temp0-Led-Colision-M/Auto-Velocidad
	    	String type = data.substring(0,1);
	    	String posX = data.substring(1,6);
	    	String posY = data.substring(7,12);
	    	String posZ=data.substring(13,18);
	    	int x = Integer.parseInt(data.substring(2,3));
	    	
	   int y = Integer.parseInt(data.substring(7,8));
	    int z = Integer.parseInt(data.substring(13,14));
	    	
	    	text2.setText(posX);
	    	text4.setText(posY);
	    	text6.setText(posZ);
	    	/*
	    	if (x<0){
	    		xpos.setBackgroundColor(Color.BLACK);
	    		xneg.setBackgroundColor(Color.RED);
	    	}else if (x > 0){
	    		xpos.setBackgroundColor(Color.RED);
	    		xneg.setBackgroundColor(Color.BLACK);
	    	}
	    	if (y<0){
	    		ypos.setBackgroundColor(Color.BLACK);
	    		yneg.setBackgroundColor(Color.RED);
	     	    
	    	}else if (y> 0){
	    		ypos.setBackgroundColor(Color.RED);
	    		yneg.setBackgroundColor(Color.BLACK);
	     	    
	    	}
	    	if (z<0){
	    		zpos.setBackgroundColor(Color.BLACK);
	    		zneg.setBackgroundColor(Color.RED);
	     	    
	    	}else if (z> 0){
	    		zpos.setBackgroundColor(Color.RED);
	    		zneg.setBackgroundColor(Color.BLACK);
	     	    
	    	}
	    	
	    	*/
	    	
	    	System.out.println(posX);
	    	System.out.println(posY);
	    	System.out.println(posZ);
			return false;
	    }	

	
	 /**
 	*La clase SendData establece un socket de emision para poder enviar tramas
 	*desde el sistema Android al robot arduino.Ademas realizara el parseado de la
 	*informacion en un formato que sea entendido por nuestro robot.Para poder realizar 
 	*el envio se debera crea un thread. Se utilizara como puerto emisor 55056
 	*/
	public boolean sendData(){
		try {
			final DatagramSocket socket = new DatagramSocket ();
			socket.setReuseAddress(true);
			InetAddress address;
			//address = InetAddress.getByName ("172.20.10.9");
			address = InetAddress.getByName ("192.168.43.214");
			
			byte[] buf = new byte[256];
	        //String s = "A2553MY"; //TESTING
	       // buf = s.getBytes ();
	        
	     
			String stemp;
			stemp=tramaType;
		
			
			System.out.println (stemp);
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
                             // System.out.println ("About to send message");
                              socket.send (packet);
                              //System.out.println ("Sent message");
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
                      socket = new DatagramSocket (4560);
                      socket.setReuseAddress(true);
                	  //socket = new DatagramSocket (55056);
                      while (true)
                      {                          

                            packet = new DatagramPacket (buf, buf.length);
                            socket.receive (packet);
                            System.out.println ("Received packet");
                            String s = new String (packet.getData());
                            //Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();// Context contexto = this;

                            //packet = new DatagramPacket (buf, buf.length);
                           packet= new DatagramPacket(buf,20);
                    	  socket.receive (packet);
                            System.out.println ("Received packet");
                            rxPacket = new String (packet.getData());
                            //Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                            //System.out.println (rxPacket);
                          activityTest1.runOnUiThread(new Runnable(){
                            	public void run(){
                            		//parseRXstring(rxPacket);
                            		log=" Rx: "+rxPacket;
                            		ModificarRegistro();
                            	}
                            });
                           /* hand.post(new Runnable() {
                                public void run() {
                                	parseRXstring(rxPacket);
                                }
                            });*/
                            

                      }
                }
                catch (IOException e)
                {
                      Log.e(getClass().getName(), e.getMessage());
                }
          }
    }


	@Override
	public void onSensorChanged(SensorEvent event) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}



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
		    cab=cab+"                       \n\n";
		    
		    
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
