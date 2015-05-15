package edu.url.salle.metapp02;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;

import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.gesture.Gesture;
import android.gesture.GestureLibraries;
import android.gesture.GestureLibrary;
import android.gesture.GestureOverlayView;
import android.gesture.Prediction;
import android.gesture.GestureOverlayView.OnGesturePerformedListener;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;


public class remotoActivity extends Activity implements OnGesturePerformedListener,SensorEventListener{
	
	//Wifi settings
	public Socket sender;
    public BufferedReader br;
    public PrintStream bw;
    private SensorManager sSensorManager; 
	private Sensor mAccelerometer;
	public float angle = 0;
	int oldSpeed;
	
	//Variables trama control remoto
	int velocidad=0; //Tres velocidades 1, 2 ,3, -1, -2, -3 
	int updown=0; //Dirección
	int modo=0; //Manual/Automatico
	char angulo = '3';//Posiciones accel mobil -> 12345 ( 3 = posicion central )
	char gesturetrigger='N';
	char activateLED ='N';

	char old_angulo='9';
	
	private TextView text;
	Button Bmodo;
	ImageView freno1,freno2;
	private GestureLibrary libreria;
 
	protected void onCreate(Bundle savedInstanceState) {
			
        super.onCreate(savedInstanceState);
        setContentView(R.layout.cremoto);
        sSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE); 
        mAccelerometer = sSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        //Creating the thread
        Thread t = new Thread (new SocketListener ());
        t.start();
        
        ImageButton Bvolver;
		ImageButton Baddmarcha;
		ImageButton Blessmarcha;
		ImageButton Bled;
		ImageButton Bfreno;
		freno1 = (	ImageView) findViewById(R.id.linear2);
		freno2 = (	ImageView) findViewById(R.id.linear3);
		
		Bvolver = (	ImageButton) findViewById(R.id.button1);
		Bvolver.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
				toast1.show();
        	   volver(null);
				
	       }

        });
		Bmodo = (Button) findViewById(R.id.button2);
		Bmodo.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   

            Toast toast2 =Toast.makeText(getApplicationContext(),"Id clicleada modo ", Toast.LENGTH_SHORT);
			toast2.show();
        	if (modo==0){modo=1;}else
        	{
        		if (modo==1){modo=0;}
        		
        	}
        	modo();	
	       }
           

        });
		Baddmarcha = (ImageButton) findViewById(R.id.button6);
		text = (TextView) findViewById(R.id.text4);
		
		Baddmarcha.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   
            	Toast toast3 =Toast.makeText(getApplicationContext(),"Id clicleada modo ", Toast.LENGTH_SHORT);
				toast3.show();
				updown=0;
				marchas();
			
        	
				}	
			

        });
		Blessmarcha = (ImageButton) findViewById(R.id.button7);
		Blessmarcha.setOnClickListener(new View.OnClickListener() {
	           public void onClick(View v) {
	   
	        	   
	                // Perform action on click
	            	Toast toast3 =Toast.makeText(getApplicationContext(),"Id clicleada modo ", Toast.LENGTH_SHORT);
					toast3.show();
					updown=1;
					marchas();
				
	        	
					}	
				

	        });
		Bfreno = (ImageButton) findViewById(R.id.button5);
		Bfreno.setOnTouchListener(new OnTouchListener() {
	        public boolean onTouch(View v, MotionEvent event) {
	        	
	        	
	        	
	           switch (event.getAction())
	           {
	           case MotionEvent.ACTION_DOWN:
               {
            	   freno1.setBackgroundColor(0xFF3333FF);
            	   freno2.setBackgroundColor(0xFF3333FF);
            	   frenar();
                   return true;
               }

               case MotionEvent.ACTION_UP:
               {
            	   freno1.setBackgroundColor(0xFFFFFFF);
            	   freno2.setBackgroundColor(0xFFFFFFF);
            	   arrancar();
                   return true;
               }

	               default:
	            	 //  freno1.setBackgroundColor(0xFFFFFFF);
	            	//   freno2.setBackgroundColor(0xFFFFFFF);
	                   return false;
	                   
	           }
	        }});
		
		Bled = (ImageButton) findViewById(R.id.button3);
		Bled.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada Luces ", Toast.LENGTH_SHORT);
				toast1.show();	
				luces();
	       }
        });
		libreria = GestureLibraries.fromRawResource(this, R.raw.gestures);
		if (!libreria.load()) {
			finish();
		}
		GestureOverlayView gesturesView = (GestureOverlayView) findViewById(R.id.gestures);
		gesturesView.addOnGesturePerformedListener(this);	
	}


public void frenar() {
		//Implementar
	oldSpeed = velocidad;
	velocidad=0;
	sendData();
}


public void arrancar() {
	//Implementar
	velocidad=oldSpeed;
	sendData();
}

	
public void luces() {
		//Implementar	
		if (activateLED=='N'){
			activateLED='Y';	
		}else{
			activateLED='N';
		}
		sendData();
}
	
	
public void marchas() {		   
	if(velocidad>=-3 && velocidad<=3 ){
		if(updown==0 && velocidad<3 ){
			velocidad=velocidad+1;
	    	text.setText(""+velocidad);
    	}
		
		else if (updown==1 && velocidad>-3){
			velocidad=velocidad-1;
			text.setText(""+velocidad);
    	}	
		sendData();
	}	
}
	public void modo() {
		   
		if(modo==0 ){
			Bmodo.setText("Manual");
    	}
		else 
		{
			Bmodo.setText("Automatico");
    	}
		sendData(); 	
	}	


	
	public void volver(View view) {
		   
		Intent intent = new Intent(this, 
               MainActivity.class);
		
startActivity(intent);

}

	
	
	

@Override
public void onGesturePerformed(GestureOverlayView ov, Gesture gesture) {
	ArrayList<Prediction> predictions = libreria.recognize(gesture);
	

	if(predictions.get(0).score>5){ //si la puntaci�n de reconocimiento es mayor a 5, entonces se ha reconocido una figura.
		switch(predictions.get(0).name.length()){ //miramos la longitud de los car�cteres de la figura: circulo=7, cuadrado=8 y triangulo=9
			case 7:
				
				Toast toast1 =Toast.makeText(getApplicationContext(),"Circulo ", Toast.LENGTH_SHORT);
				toast1.show();
				gesturetrigger='C';
			break;						
			case 8:
				Toast toast2 =Toast.makeText(getApplicationContext(),"Cuadrado ", Toast.LENGTH_SHORT);
				toast2.show();
				gesturetrigger='S';
			break;
			case 9:
				Toast toast3 =Toast.makeText(getApplicationContext(),"Triangulo ", Toast.LENGTH_SHORT);
				toast3.show();
				gesturetrigger='T';
			break;			
		}	
		
	}else{
		Toast toast4 =Toast.makeText(getApplicationContext(),"No se reconoce ", Toast.LENGTH_SHORT);
		toast4.show();		
		gesturetrigger='N';
	}	
	sendData();
}		
	



	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
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
        //this.text6.setText(" "+event.values[2]);
        
        angle = (float) (Math.atan2(aX, aY)/(Math.PI/180));  
        
        if (angle <= 100 && angle >= 80){
			angulo = '1';
		}else if (angle < 80 &&  angle >= 40 ){
			angulo = '2';
		}else if (angle < 40 &&  angle >= 0 ){
			angulo = '3';
		}else if (angle < 140 &&  angle > 100 ){
			angulo = '4';
		}else if (angle <= 180 &&  angle > 140 ){
			angulo = '5';
		}
        
        if (angulo!=old_angulo){
        	sendData();
        }
        
        old_angulo=angulo;
        
      //  this.text6.setText(" "+angle);
		System.out.println ("El angulo es " + angle);
		
    }
	
	public boolean sendData(){
		try {
			final DatagramSocket socket = new DatagramSocket ();
			InetAddress address;
			address = InetAddress.getByName ("172.20.10.9");
			//address = InetAddress.getByName ("192.168.43.214");
			byte[] buf = new byte[256];
			String stemp;
					
			switch (velocidad) {
			case -1:
				stemp='C'+"4"+angulo;
				break;
			case -2:
				stemp='C'+"5"+angulo;
				break;
			case -3:
				stemp='C'+"6"+angulo;
				break;
			default:
				stemp='C'+Integer.toString(velocidad)+angulo;
				break;
			}			

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
			
			buf = stemp.getBytes ();
//			String s = "C1113MY";
//			switch (entrada){
//			case 1:
//				s = "C1113MY";
//				buf = s.getBytes ();
//				break;
//			default:
//				s = "C2553MY";
//				buf = s.getBytes ();
//			
//			}
			//byte[] buf = new byte[256];
	       // String s = "C2553MY"; //TESTING
	        //buf = s.getBytes ();
			        
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
                            //Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                      }
                }
                catch (IOException e)
                {
                      Log.e(getClass().getName(), e.getMessage());
                }
          }
    }
	
	
}