package edu.url.salle.metapp02;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Toast;

public class laberintoActivity extends Activity{
	
	
	 ImageView a1,a2,a3,a4,a5;
	 ImageView b1,b2,b3,b4,b5;
	 ImageView c1,c2,c3,c4,c5;
	 ImageView d1,d2,d3,d4,d5;
	 ImageView e1,e2,e3,e4,e5;
	 
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.laberinto);
        
        
        ImageButton volver;
        ImageButton play;
        
        a1 = (	ImageView) findViewById(R.id.butm1);
		a2 = (	ImageView) findViewById(R.id.butm2);
		b2 = (	ImageView) findViewById(R.id.butm7);
		c2 = (	ImageView) findViewById(R.id.butm12);
		c3 = (	ImageView) findViewById(R.id.butm13);
		c4 = (	ImageView) findViewById(R.id.butm19);
		e4 = (	ImageView) findViewById(R.id.butm24);
		e5 = (	ImageView) findViewById(R.id.butm25);
    	
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
    	public void volver(View view) {
    		   
    		Intent intent = new Intent(this, 
                   MainActivity.class);
    		
    startActivity(intent);

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
    	
    	
    	public boolean sendData(){
    		try {
    			final DatagramSocket socket = new DatagramSocket ();
    			InetAddress address;
    			address = InetAddress.getByName ("172.20.10.9");
    			
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
        
}



