package com.wiyun.engine.skeleton;

import android.content.Intent;

import com.feelingtouch.bannerad.firstpage.BannerFirstPage;

public class BannerAdActivity extends BannerFirstPage {

	@Override
	public void setScreenProtrait() {
		this._isPortrait = false;
	}

	@Override
	public void startToGameActivity() {
		Intent i = new Intent(BannerAdActivity.this, Skeleton.class);
		startActivity(i);
		finish();
	}

}