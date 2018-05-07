package com.feelingtouch.bannerad;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.feelingtouch.bannerad.load.BannerAdTool;
import com.feelingtouch.bannerad.util.IntentUtil;
import com.feelingtouch.bannerad.util.StringUtil;
import com.feelingtouch.rpc.ads.model.GameAdBanner;

public class SlideAd extends LinearLayout {
	private PopupWindow _slideAd;
	private Activity _ctx;
	private GameAdBanner _ad;
	private Bitmap _icon;
	private Bitmap _banner;

	private boolean _isIconAd = false;

	public SlideAd(Context context, AttributeSet attrs) {
		super(context, attrs);
		_ctx = (Activity) context;

		initView(context, attrs);
	}

	public void setAdType() {
		_isIconAd = true;
	}

	public SlideAd(Context context) {
		super(context);
	}

	public void init(GameAdBanner ad) {
		_ad = ad;
		try {
			loadIcon();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public void refresh() {
		try {
			_ad = BannerAdTool.getRandomAd();
			loadIcon();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	ImageView image;
	TextView text;
	TextView name;
	Button downloadButton;
	RelativeLayout background;

	private void initView(final Context context, AttributeSet attrs) {
		try {
			_slideAd = new PopupWindow(context, attrs);

			background = (RelativeLayout) inflate(context, R.layout.slide_ad,
					null);
			background.setBackgroundDrawable(ImageManager.item_bg);
			background.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					download();
				}
			});

			image = (ImageView) background.findViewById(R.id.game_icon);
			text = (TextView) background.findViewById(R.id.game_desc);
			name = (TextView) background.findViewById(R.id.game_name);

			downloadButton = (Button) background.findViewById(R.id.download);
			if (ImageManager.button_bg != null)
				downloadButton.setBackgroundDrawable(ImageManager.button_bg);
			downloadButton.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					download();
				}
			});

			_slideAd.setContentView(background);
			initSize();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public void setSize(int width, int height) {
		if (_slideAd != null) {
			_slideAd.setWidth(width);
			_slideAd.setHeight(height);
		}
	}

	public void setDipSize(int dipWidth, int dipHeight) {
		if (_slideAd != null) {
			_slideAd.setWidth(dip2px(_ctx, dipWidth));
			_slideAd.setHeight(dip2px(_ctx, dipHeight));
		}
	}

	public void setDipHeight(int height) {
		if (_slideAd != null) {
			_slideAd.setHeight(dip2px(_ctx, height));
		}
	}

	public void setDipWidth(int width) {
		if (_slideAd != null) {
			_slideAd.setWidth(dip2px(_ctx, width));
		}
	}

	public void setHeight(int height) {
		if (_slideAd != null) {
			_slideAd.setHeight(height);
		}
	}

	public void setWidth(int width) {
		if (_slideAd != null) {
			_slideAd.setWidth(width);
		}
	}

	public void loadIcon() {
		Thread t = new Thread() {
			public void run() {
				if (_ad != null) {
					_icon = BannerAdTool.getIcon(_ad.iconLink);
					_banner = BannerAdTool.getIcon(_ad.bannerLink);
					_handler.sendEmptyMessage(SET_DATA);
				}
			}
		};
		t.start();
	}

	private void initSize() {
		DisplayMetrics dm = new DisplayMetrics();
		_ctx.getWindowManager().getDefaultDisplay().getMetrics(dm);
		_slideAd.setHeight(100);
		_slideAd.setWidth(dm.widthPixels);
	}

	public void show(int maginTop) {
		try {
			if (_ad != null) {
				_slideAd.showAsDropDown(this, 0, maginTop);
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public void show(int x, int y) {
		try {
			if (_ad != null) {
				_slideAd.showAsDropDown(this, x, y);
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public void dismiss() {
		if (_slideAd != null) {
			_slideAd.dismiss();
		}
	}

	private Handler _handler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SET_DATA:
				setData();
				break;
			default:
				break;
			}
		}
	};

	private void setData() {
		try {
			// 图标类型广告
			if (_isIconAd) {
				_slideAd.setWidth(80);
				_slideAd.setHeight(80);

				BitmapDrawable bd = new BitmapDrawable(_icon);
				background.setBackgroundDrawable(bd);

				image.setVisibility(View.GONE);
				text.setVisibility(View.GONE);
				name.setVisibility(View.GONE);
				downloadButton.setVisibility(View.GONE);
				return;
			}

			// Banner图片广告
			if (StringUtil.isNotEmpty(_ad.bannerLink) && _banner != null
					&& !_banner.isRecycled()) {
				BitmapDrawable bd = new BitmapDrawable(_banner);
				background.setBackgroundDrawable(bd);

				image.setVisibility(View.GONE);
				text.setVisibility(View.GONE);
				name.setVisibility(View.GONE);
				downloadButton.setVisibility(View.GONE);

			} else {
				// 图标配文字类型广告
				background.setBackgroundDrawable(ImageManager.item_bg);
				image.setVisibility(View.VISIBLE);
				text.setVisibility(View.VISIBLE);
				name.setVisibility(View.VISIBLE);
				downloadButton.setVisibility(View.VISIBLE);
				image.setImageBitmap(_icon);
				name.setText(_ad.gameName);
				text.setText(_ad.desc);
			}
		} catch (OutOfMemoryError e) {
			_ad = null;
		}
	}

	public void download() {
		dismiss();
		IntentUtil.toDownloadPlatform(_ctx, _ad.packageName, _ad.marketLink,
				_ad.httpLink);
	}

	public void setIsIcon(boolean isIcon) {
		_isIconAd = isIcon;
	}

	protected static final int SET_DATA = 19;

	public int dip2px(Context context, float dpValue) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (dpValue * scale + 0.5f);
	}

}
