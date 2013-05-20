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
		// Draw car
		float left = 0;
		float right = getCanvasWidth();
		float top = getCanvasHeight()/2;	
		float bottom = getCanvasHeight();	
	    Paint paint = new Paint();
	    paint.setColor(Color.BLUE);
		canvas.drawRect(left, top, right, bottom, paint);
		Path path = new Path();
		path.moveTo(getCanvasWidth()/2,0);
		path.lineTo(left, top);
		path.lineTo(right, top);
		path.close();
		canvas.drawPath(path, paint);
		//FIXME
	    canvas.rotate((float) (getPercentage()/2.0));
	}
	
}
