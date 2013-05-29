package com.legocar.testingversion.graphics2D;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.widget.TextView;

@SuppressLint("NewApi")
public class SteeringView extends ControlView{

	public void setDisplay(TextView display){
		this.display=display;
	}

	
	public SteeringView(Context context) {
		super(context);
	}
	
	public SteeringView(Context context, AttributeSet attrs) {
        super(context, attrs);
	}
 
	
	@SuppressLint("DrawAllocation")
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		//FIXME
	    canvas.rotate((float) (getPercentage()/2.0), getCanvasWidth()/2, getCanvasHeight()/2);
		// Draw car
	    
		float left = (float) (getCanvasWidth()/3.0);
		float right = (float) (2.0*getCanvasWidth()/3.0);
		float top = (float) (getCanvasHeight()/3.0);	
		float bottom = (float) (2.0*getCanvasHeight()/3.0);	
		float middleX = (float) (getCanvasWidth()/2.0);
		float middleY = (float) (getCanvasHeight()/2.0);
	    Paint paint = new Paint();
	    paint.setColor(Color.BLUE);
		canvas.drawRect(left, middleY, right, bottom, paint);
		Path path = new Path();
		path.moveTo(middleX,top);
		path.lineTo(left, middleY);
		path.lineTo(right, middleY);
		path.close();
		canvas.drawPath(path, paint);
		
	    
	}
	
}
