package com.feelingtouch.bannerad;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;

import com.feelingtouch.bannerad.load.GameADContacts;
import com.feelingtouch.bannerad.util.IntentUtil;

public class GameShowDialog extends Dialog {
	protected Context _ctx;
	protected Button _buttonClose;
	protected Button _buttonDownload;
	protected RelativeLayout _layout;

	private String _marketlink;
	private String _httpLink;
	private String _packageName;
	private CloseEvent _event = null;

	public GameShowDialog(Context context) {
		super(context, R.style.full_screen_dialog);
		int w = getWindow().getWindowManager().getDefaultDisplay().getWidth();
		int h = getWindow().getWindowManager().getDefaultDisplay().getHeight();
		this.setContentView(R.layout.banner_game_show_dialog);
		this.getWindow().setLayout((int) (w * 0.8f), (int) (h * 0.8f));
		_ctx = context;

	}

	public void initData(String marketlink, String httpLink,
			String packageName, Drawable showAdDrawable) {
		_marketlink = marketlink;
		_httpLink = httpLink;
		_packageName = packageName;
		initView(showAdDrawable);
	}

	private void initView(Drawable showAdDrawable) {
		_layout = (RelativeLayout) findViewById(R.id.game_show_root);
		_layout.setBackgroundDrawable(showAdDrawable);
		_buttonClose = (Button) findViewById(R.id.game_show_close);
		_buttonDownload = (Button) findViewById(R.id.game_show_download);

		_buttonClose.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				dismiss();
				if (_event != null) {
					_event.closeActionFinish();
				}
			}
		});

		_buttonDownload.setOnClickListener(new View.OnClickListener() {

			@SuppressLint("WorldReadableFiles")
			@Override
			public void onClick(View v) {
				IntentUtil.toDownloadPlatform(_ctx, _packageName, _marketlink,
						_httpLink);
				_ctx.getSharedPreferences(
						GameADContacts.GAME_AD_VERSION_PREFERENCE,
						Context.MODE_WORLD_READABLE).edit()
						.putBoolean(GameADContacts.GAME_AD_ISCLICKED, true)
						.commit();
				dismiss();
			}
		});
	}

	public void showWithAction(CloseEvent event) {
		_event = event;
		super.show();
	}

	public void show() {
		super.show();
	}

	public void dismiss() {
		super.dismiss();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

}
