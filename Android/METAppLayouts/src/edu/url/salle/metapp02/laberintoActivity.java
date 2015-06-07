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

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Toast;
/**
*Esta clase es la encargada de mostrar el camino que sigue nuestro robot para resolver 
*el laberinto, ademas tambien mostrara los lugares no explorados y los caminos no validos
*con ayuda de un conjunto de imageView.

*/
public class laberintoActivity extends Activity{
	
	//
	 ImageView a1,a2,a3,a4,a5;
	 ImageView b1,b2,b3,b4,b5;
	 ImageView c1,c2,c3,c4,c5;
	 ImageView d1,d2,d3,d4,d5;
	 ImageView e1,e2,e3,e4,e5;
	 ImageView p1,p2,p3,p4,p5,p6,p7,p8,p9,p10;
	 ImageView p11,p12,p13,p14,p15,p16,p17,p18,p19,p20;
	 ImageView p21,p22,p23,p24,p25,p26,p27,p28,p29,p30;
	 ImageView p31,p32,p33,p34,p35,p36,p37,p38,p39,p40;
	 
	 String log;
	 
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.laberinto);
        
        
        ImageButton volver;
        ImageButton play;
        
        a1 = (	ImageView) findViewById(R.id.butm1);
		a2 = (	ImageView) findViewById(R.id.butm2);
		a3 = (	ImageView) findViewById(R.id.butm3);
		a4 = (	ImageView) findViewById(R.id.butm4);
		a5 = (	ImageView) findViewById(R.id.butm5);
		b1 = (	ImageView) findViewById(R.id.butm6);
		b2 = (	ImageView) findViewById(R.id.butm7);
		b3 = (	ImageView) findViewById(R.id.butm8);
		b4 = (	ImageView) findViewById(R.id.butm9);
		b5 = (	ImageView) findViewById(R.id.butm10);
		c1 = (	ImageView) findViewById(R.id.butm11);
		c2 = (	ImageView) findViewById(R.id.butm12);
		c3 = (	ImageView) findViewById(R.id.butm13);
		c4 = (	ImageView) findViewById(R.id.butm14);
		c5 = (	ImageView) findViewById(R.id.butm15);
		d1 = (	ImageView) findViewById(R.id.butm16);
		d2 = (	ImageView) findViewById(R.id.butm17);
		d3 = (	ImageView) findViewById(R.id.butm18);
		d4 = (	ImageView) findViewById(R.id.butm19);
		d5 = (	ImageView) findViewById(R.id.butm20);
		e1 = (	ImageView) findViewById(R.id.butm21);
		e2 = (	ImageView) findViewById(R.id.butm22);
		e3 = (	ImageView) findViewById(R.id.butm23);
		e4 = (	ImageView) findViewById(R.id.butm24);
		e5 = (	ImageView) findViewById(R.id.butm25);
		
		p1 = (	ImageView) findViewById(R.id.pared1);
		p2 = (	ImageView) findViewById(R.id.pared2);
		p3 = (	ImageView) findViewById(R.id.pared3);
		p4 = (	ImageView) findViewById(R.id.pared4);
		p5 = (	ImageView) findViewById(R.id.pared5);
		p6 = (	ImageView) findViewById(R.id.pared6);
		p7 = (	ImageView) findViewById(R.id.pared7);
		p8 = (	ImageView) findViewById(R.id.pared8);
		p9 = (	ImageView) findViewById(R.id.pared9);
		p10 = (	ImageView) findViewById(R.id.pared10);
		p11 = (	ImageView) findViewById(R.id.pared11);
		p12 = (	ImageView) findViewById(R.id.pared12);
		p13 = (	ImageView) findViewById(R.id.pared13);
		p14 = (	ImageView) findViewById(R.id.pared14);
		p15 = (	ImageView) findViewById(R.id.pared15);
		p16 = (	ImageView) findViewById(R.id.pared16);
		p17 = (	ImageView) findViewById(R.id.pared17);
		p18 = (	ImageView) findViewById(R.id.pared18);
		p19 = (	ImageView) findViewById(R.id.pared19);
		p20 = (	ImageView) findViewById(R.id.pared20);
		p21 = (	ImageView) findViewById(R.id.pared21);
		p22 = (	ImageView) findViewById(R.id.pared22);
		p23 = (	ImageView) findViewById(R.id.pared23);
		p24 = (	ImageView) findViewById(R.id.pared24);
		p25 = (	ImageView) findViewById(R.id.pared25);
		p26 = (	ImageView) findViewById(R.id.pared26);
		p27 = (	ImageView) findViewById(R.id.pared27);
		p28 = (	ImageView) findViewById(R.id.pared28);
		p29 = (	ImageView) findViewById(R.id.pared29);
		p30 = (	ImageView) findViewById(R.id.pared30);
		p31 = (	ImageView) findViewById(R.id.pared31);
		p32 = (	ImageView) findViewById(R.id.pared32);
		p33 = (	ImageView) findViewById(R.id.pared33);
		p34 = (	ImageView) findViewById(R.id.pared34);
		p35 = (	ImageView) findViewById(R.id.pared35);
		p36 = (	ImageView) findViewById(R.id.pared36);
		p37 = (	ImageView) findViewById(R.id.pared37);
		p38 = (	ImageView) findViewById(R.id.pared38);
		p39 = (	ImageView) findViewById(R.id.pared39);
		p40 = (	ImageView) findViewById(R.id.pared40);
	
		
		
    	volver = (ImageButton) findViewById(R.id.button10);
    	volver.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {

        	   
                // Perform action on click
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada volver ", Toast.LENGTH_SHORT);
    			toast1.show();
        	   volver(null);
    			
    			
           // 	(null);
           }

        });
    	play = (ImageButton) findViewById(R.id.button6);
    	play.setOnClickListener(new View.OnClickListener() {
           public void onClick(View v) {

        	   sendData();
        	   
                // Perform action on click
            	Toast toast1 =Toast.makeText(getApplicationContext(),"Id clicleada Play ", Toast.LENGTH_SHORT);
    			toast1.show();
        	   pintar();
    			
    			
           // 	(null);
           }

        });
    	
    }
	
	/**
	*La funcion volver ejecutara un intent que nos permitira volver al main principal en cual 
	*se encuentra la pantalla de seleccion de modo.
	*/
	
    	public void volver(View view) {
    		   
    		Intent intent = new Intent(this, 
                   MainActivity.class);
    		log=" Tx: XMODLA";
    		ModificarRegistro(); // Esto no iria aqui sino cuando se haga un SendData()
    startActivity(intent);
    finish();

    }
    	@Override
    	public boolean onKeyDown(int keyCode, KeyEvent event)
    	{
    		if ((keyCode == KeyEvent.KEYCODE_BACK))
    		{
    			//tramaType='X'; //Kill the arduino process
    			sendData();	
    			volver(null);
    			return true;

    		}
    		
    		return super.onKeyDown(keyCode, event);
    	}   
    	public void pintar() {
 		   
    		  a1.setBackgroundColor(0xFF3333FF);
        	   a2.setBackgroundColor(0xFF3333FF);
        	   b2.setBackgroundColor(0xFF3333FF);
        	   c2.setBackgroundColor(0xFF3333FF);
        	   c3.setBackgroundColor(0xFF3333FF);
        	   c4.setBackgroundColor(0xFF3333FF);
        	   e4.setBackgroundColor(0xFF3333FF);
        	   e5.setBackgroundColor(0xFF3333FF);
    			

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
    			InetAddress address;
    			//address = InetAddress.getByName ("172.20.10.9");
    			address = InetAddress.getByName ("192.168.43.214");
    			
    			byte[] buf = new byte[256];
    	        String s = "L2553MY"; //TESTING
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



