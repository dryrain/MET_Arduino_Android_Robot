
package edu.url.salle.classes;

import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;



/**
*Esta clase sera la encargada de la deteccion de los gestos tactiles, que permitira que el robot realice un circulo, triangulo
*o cuadrado segun el gesto que se haya introducido


*/

public class GestureListener extends GestureDetector.SimpleOnGestureListener
{
    
       public static String currentGestureDetected;
       
      // Override s all the callback methods of GestureDetector.SimpleOnGestureListener
      @Override
      public boolean onSingleTapUp(MotionEvent ev) {
    //      currentGestureDetected=ev.toString();
       
        return true;
      }
      @Override
      public void onShowPress(MotionEvent ev) {
   //       currentGestureDetected=ev.toString();
        
      }
      @Override
      public void onLongPress(MotionEvent ev) {
  //        currentGestureDetected=ev.toString();
       
      }
      @Override
      public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
  //        currentGestureDetected=e1.toString()+ "  "+e2.toString();
    	  Log.e("hola","x: "+String.valueOf(distanceX)+"   y: "+String.valueOf(distanceX));
        return true;
      }
      @Override
      public boolean onDown(MotionEvent ev) {
  //        currentGestureDetected=ev.toString();
        
        return true;
      }
      @Override
      public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
       //   currentGestureDetected=e1.toString()+ "  "+e2.toString();
    	 // Log.e("hola",String.valueOf(velocityX));
        return true;
      }
}