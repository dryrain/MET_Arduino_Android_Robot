package edu.url.salle.metapp02;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;

import android.widget.ImageButton;
import android.widget.TextView;

import android.widget.Toast;

public class accelActivity extends Activity implements SensorEventListener{
	int x,y,z;
	
	private SensorManager sSensorManager; 
	private Sensor mAccelerometer;
	public float angle = 0;
	  TextView text2,text4,text6;
	
	
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.accel);
        sSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE); 
        mAccelerometer = sSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        
        x=1;
        y=0;
        z=0;
      
        ImageButton Bvolver;
		
		Bvolver = (	ImageButton) findViewById(R.id.button1);
		Bvolver.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   
        	   sendData();
        	   
                // Perform action on click
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
				toast1.show();
        	   volver(null);
				
           }

        });
		
		text2 = (TextView) findViewById(R.id.text2);
		text4 = (TextView) findViewById(R.id.text4);
		text6 = (TextView) findViewById(R.id.text6);
        
		

}
	

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


	public void volver(View view) {	   
		Intent intent = new Intent(this, 
        MainActivity.class);
		startActivity(intent);
}



	
	
	
	public boolean sendData(){
		try {
			final DatagramSocket socket = new DatagramSocket ();
			InetAddress address;
			//address = InetAddress.getByName ("172.20.10.9");
			address = InetAddress.getByName ("192.168.43.214");
			
			byte[] buf = new byte[256];
	        String s = "A2553MY"; //TESTING
	        buf = s.getBytes ();
			        
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
                	  //socket = new DatagramSocket (55056);
                      while (true)
                      {                          
                            packet = new DatagramPacket (buf, buf.length);
                            socket.receive (packet);
                            System.out.println ("Received packet");
                            String s = new String (packet.getData());
                            //Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();// Context contexto = this;
                      }
                }
                catch (IOException e)
                {
                      Log.e(getClass().getName(), e.getMessage());
                }
          }
    }


	
	
}
