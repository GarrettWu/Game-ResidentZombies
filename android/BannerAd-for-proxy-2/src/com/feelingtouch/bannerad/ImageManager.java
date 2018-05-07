package com.feelingtouch.bannerad;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;

public class ImageManager {
	private static Bitmap _background;
	private static Bitmap _button_bg;
	private static Bitmap _item_bg;
	
	public static BitmapDrawable background = null;
	public static BitmapDrawable button_bg;
	public static BitmapDrawable item_bg;
	

	public static boolean DECODE_FINISH = false;

	public static void decodeBmp(Context ctx) {
		DECODE_FINISH = false;
		_background = BitmapFactory.decodeResource(ctx.getResources(),
				R.drawable.banner_ad_dialog_bg);
		_button_bg = BitmapFactory.decodeResource(ctx.getResources(),
				R.drawable.banner_ad_button_bg);
		_item_bg = BitmapFactory.decodeResource(ctx.getResources(),
				R.drawable.banner_ad_item_bg);
		background = new BitmapDrawable(_background);
		button_bg = new BitmapDrawable(_button_bg);
		item_bg = new BitmapDrawable(_item_bg);
		DECODE_FINISH = true;
	}

	public static void release() {
		if (_background != null) {
			_background.recycle();
		}
		if (_button_bg != null) {
			_button_bg.recycle();
		}
		if (_item_bg != null) {
			_item_bg.recycle();
		}

		System.gc();
		System.gc();
		System.gc();
	}
}
