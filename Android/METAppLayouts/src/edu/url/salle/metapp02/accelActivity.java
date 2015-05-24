package edu.url.salle.metapp02;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
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
import android.view.View;

import android.widget.ImageButton;

import android.widget.ImageView;

import android.widget.TextView;

import android.widget.Toast;

public class accelActivity extends Activity implements SensorEventListener{
	int x,y,z;
	String tramaType = "A";
	
	private SensorManager sSensorManager; 
	private Sensor mAccelerometer;
	public float angle = 0;
	  TextView text2,text4,text6;

	  ImageView image1,image2,image3,image4,image5,image6;
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
/*
    public void onAccuracyChanged(Sensor sensor, int accuracy) { 
        // TODO Auto-generated method stub 
         
    } 

    public void onSensorChanged(SensorEvent event) { 
        // TODO Auto-generated method stub 
        if (event.sensor != mAccelerometer) 
            return; 
         
        float aX = event.values[0]; 
        float aY = event.values[1]; 
        //float aZ = event.values[2]; 
        
        this.text2.setText(" "+event.values[0]);

        this.text4.setText(" "+event.values[1]);

        //this.text6.setText(" "+event.values[2]);
        
        angle = (float) (Math.atan2(aX, aY)/(Math.PI/180));  
        this.text6.setText(" "+angle);
		System.out.println ("El angulo es " + angle);
		sendData();
        
    } 

<<<<<<< Updated upstream

=======
*/

	public void volver(View view) {	   
		Intent intent = new Intent(this, 
        MainActivity.class);
		startActivity(intent);
}




	 private boolean parseRXstring(String data)
	    {
	    	
	    	//C34N0M0N
	    	//Type-Temp1-Temp0-Led-Colision-M/Auto-Velocidad
	    	String type = data.substring(0,1);
	    	String posX = data.substring(1,6);
	    	String posY = data.substring(7,12);
	    	String posZ=data.substring(13,18);
	    	
	    	text2.setText(posX);
	    	text4.setText(posY);
	    	text6.setText(posZ);
	    	
	    	System.out.println(posX);
	    	System.out.println(posY);
	    	System.out.println(posZ);
			return false;
	    }	

	
	
	public boolean sendData(){
		try {
			final DatagramSocket socket = new DatagramSocket ();
			socket.setReuseAddress(true);
			InetAddress address;
			address = InetAddress.getByName ("172.20.10.9");
			//address = InetAddress.getByName ("192.168.43.214");
			
			byte[] buf = new byte[256];
	        //String s = "A2553MY"; //TESTING
	       // buf = s.getBytes ();
	        
	     
			String stemp;
			stemp=tramaType;
			/*		
			switch (velocidad) {
			case -1:
				stemp=tramaType+"4"+angulo;
				break;
			case -2:
				stemp=tramaType+"5"+angulo;
				break;
			case -3:
				stemp=tramaType+"6"+angulo;
				break;
			default:
				stemp=tramaType+Integer.toString(velocidad)+angulo;
				break;
			}			
			
			//Reset trama Type
			tramaType='C';

			//Manual/Auto
			if (modo==0){
				//'M'
				stemp=stemp+'M';
			}else{
				//'A'
				stemp=stemp+'A';
			}
			
			//GestureTrigger
			stemp=stemp+gesturetrigger+activateLED;
			
			//Reset gesture detected
			gesturetrigger='N';
			
			System.out.println (stemp);
			*/System.out.println (stemp);
			buf = stemp.getBytes ();
	        
	        
	        
			        
	        final DatagramPacket packet = new DatagramPacket (buf, buf.length, address, 55056);
			
	        new Thread ()
            {
                  public void run ()
                  {
                        try
                        {
                              System.out.println ("About to send message");
                              socket.send (packet);
                              System.out.println ("Sent message");
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
                            		parseRXstring(rxPacket);
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



	
	
}
