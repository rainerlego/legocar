package com.legocar.testingversion.graphics2D;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Shader.TileMode;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

@SuppressLint("NewApi")
public class SpeedView extends ControlView{
	
	final static private float TOUCHOFFSET=(float) 32.0;

	
	public SpeedView(Context context) {
		super(context);
	}
	
	public SpeedView(Context context, AttributeSet attrs) {
        super(context, attrs);
	}
 
	
	@SuppressLint("DrawAllocation")
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		// Draw background
		Paint paintGrandient = new Paint();
	    paintGrandient.setShader(new LinearGradient(0,0, 0,getHeight(), Color.BLACK, Color.GRAY, TileMode.MIRROR));
	    canvas.drawPaint(paintGrandient);
	    
	    // Draw slider
		float left = LINEWIDTH;
		float right = left+ getCanvasWidth();
		float top = LINEWIDTH;	
		float bottom =top+getSliderY();	
	    Paint paint = new Paint();
	    paint.setColor(Color.WHITE);
		canvas.drawRect(left, top, right, bottom, paint);
	}
	
	
	
	private float getSliderY(){
		return (float) (getCanvasHeight()*(1.0-(getPercentage()/100.0))/2.0);
	}
	private boolean canSlide(float y){
		return Math.abs(y-getSliderY())>TOUCHOFFSET;
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		final int action=event.getAction();
		final float y=event.getY()-LINEWIDTH;
		//TODO
		Log.i("TouchEvent", "y " +y);

		switch (action) {
		case MotionEvent.ACTION_DOWN:
			if(canSlide(y)){
				return false;
			}
			break;
		case MotionEvent.ACTION_MOVE:
			if(y<0){
				updatePercentage((float) 100.0);
			}else if(y>getCanvasHeight()){
				updatePercentage((float) 0.0);
			}else {
				updatePercentage((float) (100.0*(1.0-2.0*y/getCanvasHeight())));
			}
			break;
		case MotionEvent.ACTION_UP:
			if(canSlide(y)){
				return false;
			}
			if(y<0){
				updatePercentage((float) 100.0);
			}else if(y>getCanvasHeight()){
				updatePercentage((float) 0.0);
			}else {
				updatePercentage((float) (100.0*(1.0-2.0*y/getCanvasHeight())));
			}
			break;
		default:
			return false;
		}
		return true;	
	}

}
