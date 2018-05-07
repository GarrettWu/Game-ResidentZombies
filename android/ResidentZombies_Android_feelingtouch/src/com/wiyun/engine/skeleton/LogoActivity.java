package com.wiyun.engine.skeleton;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Window;
import android.view.WindowManager;

import com.cafgame.residentzombies.R;

public class LogoActivity extends Activity {

	Handler handler;
	
	@Override
    protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.logo);
		
		timer.schedule(task, 2000);
		
		handler = new Handler() {
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case 0:
					Intent i = new Intent(LogoActivity.this, Skeleton.class);
		    		startActivity(i);
		        	LogoActivity.this.finish();
					break;
				default:
					break;
				}
			}
		};
	}
	
	Timer timer = new Timer();  
    TimerTask task = new TimerTask(){  
  
        public void run() {  
        	handler.sendEmptyMessage(0);
        }  
          
    };
}