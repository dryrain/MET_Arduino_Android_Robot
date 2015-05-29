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

import android.R.color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
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
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

/**
*La clase remotoActivity nos permite controlar remotamente nuestro robot, permitiendo elegir a que
*velocidad, direccion, sentido queremos que se mueva taambien nos permitira elegir entre 2 modos de 
*funcionamiento: el automatico y el manual. Ademas nos aportara informacion sobre el mismo
*como puede ser la temperatura, si tiene un obstaculo proximo,etc.


*/
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
	char tramaType = 'C';

	char old_angulo='9';
	
	private TextView text;
	Button Bmodo;
	private static ImageView frenodelantero,freno1,freno2;
	private static ImageView accel1,accel2,accel3;
	private static TextView textAcel;
	//TextView  textTemp ;
	ImageButton Bled;
	private GestureLibrary libreria;
	 private static TextView  textTemp ;
	Activity activityTest;
	String rxPacket;
	Handler handler = new Handler();
	Thread t;
    
	
	protected void onCreate(Bundle savedInstanceState) {
	//protected void onCreate(View v) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.cremoto);
        sSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE); 
        mAccelerometer = sSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        //Creating the thread
        
       t = new Thread (new SocketListener ());
        
        t.start();
        
        ImageButton Bvolver;
		ImageButton Baddmarcha;
		ImageButton Blessmarcha;
		
		activityTest = this;
		
		
		ImageButton Bfreno;
		frenodelantero = (ImageView) findViewById(R.id.linear1);
		freno1 = (	ImageView) findViewById(R.id.linear2);
		freno2 = (	ImageView) findViewById(R.id.linear3);
		accel1 = (	ImageView) findViewById(R.id.view4);
		accel2 = (	ImageView) findViewById(R.id.view5);
		accel3 = (	ImageView) findViewById(R.id.view6);
		textTemp = (TextView) findViewById(R.id.text);
		textAcel = (TextView) findViewById(R.id.text5);
		
		
		Bvolver = (	ImageButton) findViewById(R.id.button1);
		Bvolver.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
				//toast1.show();
				
				tramaType='X'; //Kill the arduino process
				sendData();		
        	    volver(null);
				
	       }

        });
		Bmodo = (Button) findViewById(R.id.button2);
		Bmodo.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   

            //Toast toast2 =Toast.makeText(getApplicationContext(),"Id clicleada modo ", Toast.LENGTH_SHORT);
			//toast2.show();
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
   
            	//Toast toast3 =Toast.makeText(getApplicationContext(),"Id clicleada modo ", Toast.LENGTH_SHORT);
				//toast3.show();
				updown=0;
				marchas();
			
        	
				}	
			

           
        });
		Blessmarcha = (ImageButton) findViewById(R.id.button7);
		Blessmarcha.setOnClickListener(new View.OnClickListener() {
	           public void onClick(View v) {
	   
	        	   
	                // Perform action on click
	            	//Toast toast3 =Toast.makeText(getApplicationContext(),"Id clicleada modo ", Toast.LENGTH_SHORT);
					//toast3.show();
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
            	  // freno1.setBackgroundColor(0xFF3333FF);
            	   freno1.setBackgroundColor(Color.RED);
            	   
            	   freno2.setBackgroundColor(Color.RED);
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
            	//Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada Luces ", Toast.LENGTH_SHORT);
				//toast1.show();	
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
	/**
	*La funcion frenar permitira parar nuestro robot.Utilizara la funcion 
*sendData para informar al robot de nuestra orden.
	*/

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

/**
*La funcion luces permitira enceder las luces de nuestro robot.Utilizara la funcion 
*sendData para informar al robot de nuestra orden.
*/

public void luces() {
		//Implementar	
		if (activateLED=='N'){
			activateLED='Y';	
		}else{
			activateLED='N';
		}
		sendData();
}
	
/**
*La funcion marchas permitira elegir la marcha a la que deseamos que nuestro robot se
*mueva. Hemos definido tres marchas positivas y tres negativas.Utilizara la funcion 
*sendData para informar al robot de nuestra orden.
*/
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

/**
*La funcion modo permite seleccionar que el robot funcione en modo manual (pudiendo elegir sentido 
*y velocidad) o en modo automatico.Utilizara la funcion sendData para informar al robot de nuestra 
*orden.
*/

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

	
	/**
	*La funcion volver ejecutara un intent que nos permitira volver al main principal en cual 
	*se encuentra la pantalla de seleccion de modo.
	*/
	
	public void volver(View view) {   
		Intent intent = new Intent(this, 
        MainActivity.class);
		startActivity(intent);
		finish();
		t.interrupt();
		
		
		
}

	/**
	*La funcion onGesturePerformed sera la encargada de la deteccion del gesto tactil de una
	*serie de figura geometricas que el robot puede realizar. Una vez detectado uno de estos
	*gestos ejecutara la funcion sendData para que el robot lleve a cabo dicho movimiento.
	*/
	
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
       // remotoActivity.handler;
    } 

    @Override 
    protected void onPause() { 
        super.onPause(); 
        sSensorManager.unregisterListener(this, mAccelerometer); 
        
    
    } 

    public void onAccuracyChanged(Sensor sensor, int accuracy) { 
        // TODO Auto-generated method stub 
         
    } 

    /**
	*La funcion onSensorChanged actualizara el valor 3 variables en nuestro caso ax,ay,az 
	*en funcion de los valores obtenidos por el acelerometro y en base a estos se determinara
	*un angulo de giro que sera enviado al robot mediante la funcion sendData. Hemos de comentar
	*que considerado 6 angulos de giro 3 a la izauierda y otros 3 a la derecha que el robot podra
	*llevar a cabo.
	*/
    
    
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
		//System.out.println ("El angulo es " + angle);
		
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
    	String temperature = data.substring(1,3);
    	String LEDsActive = data.substring(3,4);
    	int colision = Integer.parseInt(data.substring(4,5));  
    	String modo = data.substring(5,6);  
    	int velocidad = Integer.parseInt(data.substring(6,7));
    	
    	 System.out.println ("DENTRO");
    	 //System.out.println (temperature);
    	//Change temperature
    	
    	
    	 textTemp.setText(temperature);
    	
    	//textAcel = (TextView) findViewById(R.id.text5);
    	
    	
    	//Detecting colision
    	if (colision==0){
    		frenodelantero.setBackgroundColor(Color.BLACK);
    		freno1.setBackgroundColor(Color.BLACK);
     	    freno2.setBackgroundColor(Color.BLACK);
    	}else if (colision == 1){
    		frenodelantero.setBackgroundColor(Color.RED);
     	    
    	}else if (colision == 2){
    		//do sth
    		freno1.setBackgroundColor(Color.RED);
     	    
    		
    	}else{
    		//do sth
    		
     	    freno2.setBackgroundColor(Color.RED);
    	}
    	
    	if (velocidad==0){
    		accel1.setBackgroundColor(Color.BLACK);
    		accel2.setBackgroundColor(Color.BLACK);
     	    accel3.setBackgroundColor(Color.BLACK);
     	   textAcel.setText(Integer.toString(velocidad));
     	    
    	}else if ((velocidad == 1)){
    		
    		accel1.setBackgroundColor(Color.GREEN);
    		accel2.setBackgroundColor(Color.BLACK);
     	    accel3.setBackgroundColor(Color.BLACK);
     	    textAcel.setText("1");
     	    
   		
    	}else if ((velocidad == 2)){
    		//do sth
    		accel1.setBackgroundColor(Color.GREEN);
    		accel2.setBackgroundColor(Color.GREEN);
    		accel3.setBackgroundColor(Color.BLACK);
    		
         	textAcel.setText("2");
         	    
       		
    		
    	}else if((velocidad == 3)){
    		//do sth
    		accel1.setBackgroundColor(Color.GREEN);
    		accel2.setBackgroundColor(Color.GREEN);
    		accel3.setBackgroundColor(Color.GREEN);
         	textAcel.setText("3");
         	    
       		
    	}else if((velocidad == 4)){
    		//do sth
    		accel1.setBackgroundColor(Color.RED);
    		accel2.setBackgroundColor(Color.BLACK);
    		accel3.setBackgroundColor(Color.BLACK);
    		textAcel.setText("-1");
         	    
       		
    	}else if((velocidad == 5 )){
    		//do sth
    		accel1.setBackgroundColor(Color.RED);
    		accel2.setBackgroundColor(Color.RED);
    		accel3.setBackgroundColor(Color.BLACK);
    		
         	    	textAcel.setText("-2");
         	    
       		
    	}else if((velocidad == 6)){
    		//do sth
    		accel1.setBackgroundColor(Color.RED);
    		accel2.setBackgroundColor(Color.RED);
    		accel3.setBackgroundColor(Color.RED);
    		
         	textAcel.setText("-3");
         	    
       		
    	}
    	
    	
    	if(LEDsActive.equals("Y")){
    		Bled.setBackgroundColor(Color.YELLOW);
    	}else if(LEDsActive.equals("N")){
    		Bled.setBackgroundColor(Color.WHITE);
    		
    	}
    	if(modo.equals("M")){
    		
    		Bmodo.setText("Manual");
    	}else{
    		Bmodo.setText("Automatico");
    	}
    	
    	System.out.println(temperature);
    	System.out.println(modo);
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
			address = InetAddress.getByName ("172.20.10.9");
			//address = InetAddress.getByName ("192.168.43.214");
			byte[] buf = new byte[256];
			String stemp;
					
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

			if(tramaType=='X'){
				socket.close();
				
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
			
			//System.out.println (stemp);
			
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
                byte[] buf = new byte[8];
                System.out.println ("Thread running");
                try
                {
                     socket = new DatagramSocket (4560);
                     socket.setReuseAddress(true);
                	
                      while (true)
                      {                          
                            //packet = new DatagramPacket (buf, buf.length);
                           packet= new DatagramPacket(buf,8);
                    	  socket.receive (packet);
                            System.out.println ("Received packet");
                            rxPacket = new String (packet.getData());
                            //Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                            System.out.println (rxPacket);
                            
                          activityTest.runOnUiThread(new Runnable(){
                            	public void run(){
                            		parseRXstring(rxPacket);
                            	}
                            
                            
                            
                            
                           /* 
                            
                            handler.post(new Runnable() {
                                public void run() {
                                	parseRXstring(rxPacket);
                                }
                            });
                            */		
                            	//}
                            });
                            	 
                            
                      }
                }
                catch (IOException e)
                {
                      Log.e(getClass().getName(), e.getMessage());
                }
          }
    }
	
	
}