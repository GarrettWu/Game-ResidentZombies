package com.feelingtouch.bannerad.firstpage;

import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import com.feelingtouch.bannerad.BannerAd;
import com.feelingtouch.bannerad.R;
import com.feelingtouch.bannerad.load.BannerAdTool;

public abstract class BannerFirstPage extends Activity {
	private Handler _handler = new Handler();

	protected boolean _isPortrait;

	private class BannerRunnable implements Runnable {
		private boolean _killed = false;

		public void run() {
			if (!_killed)
				startToGameActivity();
		}

		public void killMe() {
			_killed = true;
		}
	};

	private BannerRunnable _runnable = new BannerRunnable();

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.banner_first_page);
		setScreenProtrait();
		BannerAdTool.init(getApplicationContext(), _isPortrait);

		BannerAd.initBannerAdDialogBmp(getApplicationContext());

		initView();
		concreteInit();
	}

	public void concreteInit() {

	}

	public abstract void setScreenProtrait();

	public abstract void startToGameActivity();

	private void initView() {
		Button facebook = (Button) findViewById(R.id.banner_facebook);
		Button twitter = (Button) findViewById(R.id.banner_twitter);

		final String country = Locale.getDefault().getCountry();
		if (country.contains("CN")) {
			twitter.setVisibility(View.GONE);
			facebook.setBackgroundResource(R.drawable.banner_weibo);
		}
		facebook.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				_runnable.killMe();
				removeCallBacks();
				Uri uri = null;
				if (country.contains("CN")) {
					uri = Uri.parse("http://www.weibo.com/feelingtouch");
				} else {
					uri = Uri
							.parse("http://www.facebook.com/pages/Feelingtouch-Inc/166596376692142");
				}
				Intent it = new Intent(Intent.ACTION_VIEW, uri);
				startActivity(it);
				finish();
			}
		});

		twitter.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				_runnable.killMe();
				removeCallBacks();
				Uri uri = Uri.parse("http://twitter.com/feelingtouch");
				Intent it = new Intent(Intent.ACTION_VIEW, uri);
				startActivity(it);
				finish();
			}
		});

	}

	public void onResume() {
		super.onResume();
		setHandle();
	}

	public void onStop() {
		removeCallBacks();
		super.onStop();
	}

	public void onDestroy() {
		super.onDestroy();
	}

	private void setHandle() {
		_handler.postDelayed(_runnable, 2000);
	}

	private void removeCallBacks() {
		_handler.removeCallbacks(_runnable);
	}
}
