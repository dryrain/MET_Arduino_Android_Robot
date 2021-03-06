package edu.url.salle.metapp02;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import android.R.color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
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
*como puede ser la temperatura, si tiene un obstaculo proximo,etc. Dicha clase extiende de Activity
*y tambien implementa OnGesturePerfomed para detectar gestos tactiles y SensorEventListener que 
*detectara cuando el acelerometro del telefono movil cambia de posicion


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
	private static ImageButton Bled;
	private GestureLibrary libreria;
	 private static TextView  textTemp ;
	Activity activityTest;
	String rxPacket;
	String log;
	Handler handler = new Handler();
	Thread t;
    
	/**
	*La funcion onCreate sera la encargada de inicializar la activity con el layout que deseamos en este caso
	*el layout cremoto que se ha dise�ado especificamente para esta activity.Sera ademas la encargada de inicializar
	*el thread de envio y captura de datos, y de captar eventos de los UI elementos.
	*/
	
	protected void onCreate(Bundle savedInstanceState) {
	
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
   
				tramaType='X'; //Kill the arduino process
				sendData();	
        	    volver(null);
				
	       }

        });
		Bmodo = (Button) findViewById(R.id.button2);
		Bmodo.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
   

            
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
   
            	
				updown=0;
				marchas();
			
        	
				}	
			

           
        });
		Blessmarcha = (ImageButton) findViewById(R.id.button7);
		Blessmarcha.setOnClickListener(new View.OnClickListener() {
	           public void onClick(View v) {
	   
	        	   
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
            	  
            	   freno1.setBackgroundColor(Color.RED);
            	   
            	   freno2.setBackgroundColor(Color.RED);
            	  frenar();
            	  // arrancar();
                   return true;
               }

               case MotionEvent.ACTION_UP:
               {
            	   freno1.setBackgroundColor(0xFFFFFFF);
            	   freno2.setBackgroundColor(0xFFFFFFF);
            	   arrancar();
            	  //frenar();
                   return true;
               }

	               default:
	            	
	                   return false;
	                   
	           }
	        }});
		
		Bled = (ImageButton) findViewById(R.id.button3);
		Bled.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {
            		
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
*@see sendData()
	*/

public void frenar() {
		
	oldSpeed = velocidad;
	velocidad=0;
	sendData();
}
/**
*La funcion arrancar podra de nuevo en movimiento nuestro robot a la velocidad que marchaba una vez
*antes de ser parado.Utilizara la funcion sendData para informar al robot de nuestra orden.
*@see sendData()
*/

public void arrancar() {
	
	velocidad=oldSpeed;
	sendData();
}


/**
*La funcion luces permitira enceder las luces de nuestro robot.Utilizara la funcion 
*sendData para informar al robot de nuestra orden.
*@see sendData()
*/

public void luces() {
		
		if (activateLED=='N'){
			activateLED='Y';	
		}else{
			activateLED='N';
		}
		sendData();
}
	
/**
*La funcion marchas permitira elegir la marcha a la que deseamos que nuestro robot se
*mueva. Hemos definido tres marchas positivas y tres negativas.Ademas mostrar la marcha a la
*que vamos graficamente en el layout. Utilizara la funcion sendData para informar al robot de 
*nuestra orden.
*@see sendData()
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
*@see sendData()
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
		
		log=" Tx: XMODCR";
		ModificarRegistro(); 
		
		
		startActivity(intent);
		
		finish();
		
}

	/**
	*La funcion onGesturePerformed sera la encargada de la deteccion del gesto tactil de una
	*serie de figura geometricas que el robot puede realizar.Cuando se realiza un gesto mostrara
	*un toast diciendo si ha encontrado la figura y si por el contrario no se reconoce.
	*Una vez detectado uno de estos gestos ejecutara la funcion sendData para que el robot lleve a cabo dicho movimiento.
	*@see sendData()
	*@param GestureOverlayView ov que es la zona de deteccion que hemos activado en nuestro layout
	*@param  Gesture gesture que sera el gesto tactil que realizamos con el dedo
	*/
	
@Override
public void onGesturePerformed(GestureOverlayView ov, Gesture gesture) {
	ArrayList<Prediction> predictions = libreria.recognize(gesture);
	

	if(predictions.get(0).score>5){ //si la puntuacion de reconocimiento es mayor a 5, entonces se ha reconocido una figura.
		switch(predictions.get(0).name.length()){ //miramos la longitud de los caracteres de la figura: circulo=7, cuadrado=8 y triangulo=9
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
	

/**
*La funcion onKeyDown detecta cuando el usuario presiona una tecla en este caso el boton de atras
*y una vez detectado el boton ejecutara la funcion volver() para volver al menu principal y mandara 
*la trama con sendData()para informar al robot que hemos salido de esta activity
*@see sendData() volver()
*@param keyCode es el codigo de la tecla que pulsamos
*@param  KeyEvent event sera el evento de pulsar una tecla con el dedo
*/

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if ((keyCode == KeyEvent.KEYCODE_BACK))
		{
			tramaType='X'; //Kill the arduino process
			sendData();	
			volver(null);
			return true;

		}
		
		return super.onKeyDown(keyCode, event);
	}

/*
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
	*/
	
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
    /**
	*El metodo onAccuracyChanged sera llamado cuando el precision o nivel de exactitud de un
	*sensor ha cambiado
	*llevar a cabo.
	*@param SensorEvent event es el evento que cambia.
	*@param int accuray valor o nivel de exactitud del sensor.
	*/
    public void onAccuracyChanged(Sensor sensor, int accuracy) { 
        // TODO Auto-generated method stub 
         
    } 

    /**
	*La funcion onSensorChanged actualizara el valor 3 variables en nuestro caso ax,ay,az 
	*en funcion de los valores obtenidos por el acelerometro y en base a estos se determinara
	*un angulo de giro que sera enviado al robot mediante la funcion sendData. Esta funcion se ejecuta
	*cuando un el valor de un sensor cambia. Hemos de comentar que considerado 6 angulos de giro 3 a la izauierda 
	*y otros 3 a la derecha que el robot podra
	*llevar a cabo.
	*@see sendData()
	*@param SensorEvent event es el evento que cambia.
	*/
    
    
    public void onSensorChanged(SensorEvent event) { 
        // TODO Auto-generated method stub 
        if (event.sensor != mAccelerometer) 
            return; 
         
        float aX = event.values[0]; 
        float aY = event.values[1]; 
        
        
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
        
      
    }
    
    /**
   	*La clase parseRXstring es la encargada de traducir la trama recibida por parte 
   	*del sistema arduino y actualizar el estado de nuestros elementos UI como el TextView
   	*de la temperatura, los imageView de deteccion o presencia de obstaculos,etc
   	*@param String data: es el string de datos que recibimos por parte de Arduino
   	*@throws Exception e Si ese parsea mal algun dato de la trama recibida por parte de arduino
   	*/
    private boolean parseRXstring(String data)
    {
    	
    	//C34N0M0N
    	//Type-Temp1-Temp0-Led-Colision-M/Auto-Velocidad
    	
    	String temperature="30";
    	String LEDsActive = null;
    	int colision = 0;  
    	String modo = null;  
    	int velocidad = 0;
		try {
			String type = data.substring(0, 1);
			temperature = data.substring(1, 3);
			LEDsActive = data.substring(3, 4);
			colision = Integer.parseInt(data.substring(4, 5));
			modo = data.substring(5, 6);
			velocidad = Integer.parseInt(data.substring(6, 7));
		} catch (Exception e) {
			// TODO: handle exception
		}
    	
    	 
    	
    	try {
			textTemp.setText(temperature);
			//Detecting colision
			if (colision == 0) {
				frenodelantero.setBackgroundColor(Color.BLACK);
				freno1.setBackgroundColor(Color.BLACK);
				freno2.setBackgroundColor(Color.BLACK);
			} else if (colision == 1) {
				frenodelantero.setBackgroundColor(Color.RED);

			} else if (colision == 2) {
			
				freno1.setBackgroundColor(Color.RED);

			} else {
				
				freno2.setBackgroundColor(Color.RED);
			}
			if (velocidad == 0) {
				accel1.setBackgroundColor(Color.BLACK);
				accel2.setBackgroundColor(Color.BLACK);
				accel3.setBackgroundColor(Color.BLACK);
				textAcel.setText(Integer.toString(velocidad));

			} else if ((velocidad == 1)) {

				accel1.setBackgroundColor(Color.GREEN);
				accel2.setBackgroundColor(Color.BLACK);
				accel3.setBackgroundColor(Color.BLACK);
				textAcel.setText("1");

			} else if ((velocidad == 2)) {
				
				accel1.setBackgroundColor(Color.GREEN);
				accel2.setBackgroundColor(Color.GREEN);
				accel3.setBackgroundColor(Color.BLACK);

				textAcel.setText("2");

			} else if ((velocidad == 3)) {
				
				accel1.setBackgroundColor(Color.GREEN);
				accel2.setBackgroundColor(Color.GREEN);
				accel3.setBackgroundColor(Color.GREEN);
				textAcel.setText("3");

			} else if ((velocidad == 4)) {
			
				accel1.setBackgroundColor(Color.RED);
				accel2.setBackgroundColor(Color.BLACK);
				accel3.setBackgroundColor(Color.BLACK);
				textAcel.setText("-1");

			} else if ((velocidad == 5)) {
				
				accel1.setBackgroundColor(Color.RED);
				accel2.setBackgroundColor(Color.RED);
				accel3.setBackgroundColor(Color.BLACK);

				textAcel.setText("-2");

			} else if ((velocidad == 6)) {
				
				accel1.setBackgroundColor(Color.RED);
				accel2.setBackgroundColor(Color.RED);
				accel3.setBackgroundColor(Color.RED);

				textAcel.setText("-3");

			}
			if (LEDsActive.equals("Y")) {
				Bled.setBackgroundColor(Color.YELLOW);
			} else if (LEDsActive.equals("N")) {
				Bled.setBackgroundColor(Color.WHITE);

			}
			if (modo.equals("M")) {

				Bmodo.setText("Manual");
			} else {
				Bmodo.setText("Automatico");
			}
			
		} catch (Exception e) {
			// TODO: handle exception
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
            
            if(tramaType=='X'){
				socket.close();
				
				
				
			}
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
	*el 4560. Implementa Runnable
	*@throws IOException e Si no puede crear el socket de recepcion
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
                           
                           packet= new DatagramPacket(buf,8);
                    	   socket.receive (packet);
                            
                            rxPacket = new String (packet.getData());
                          
                          activityTest.runOnUiThread(new Runnable(){
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
	
	
	/**
	*La funcion ModificarRegistro sera la encargada de a�adir en un fichero de texto para tramas
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