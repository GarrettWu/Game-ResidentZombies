package com.feelingtouch.bannerad;

import android.content.Context;
import android.view.View;
import android.widget.Button;

public class ExitBannerAd extends BannerAdDialog {
	protected Button _quitButton;
	protected Button _noButton;

	public ExitBannerAd(Context context) {
		super(context);
		setContentView(R.layout.banner_exit_dialog);
		_adapter.setIsExitDialog(true);
	}

	@Override
	public void initView() {
		super.initView();
		_quitButton = (Button) findViewById(R.id.yes);
		_noButton = (Button) findViewById(R.id.no);
		_noButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				dismiss();
			}
		});
	}

	public void show(final QuitEvent event) {
		if (_quitButton != null) {
			super.show();
			_quitButton.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					dismiss();
					if (event == null) {
						System.exit(0);
					} else {
						event.quit();
					}
				}
			});
		} else {
			if (event == null) {
				System.exit(0);
			} else {
				event.quit();
			}
			System.exit(0);
		}
	}

	public boolean show(final QuitEvent event, final boolean isAutoQuit) {
		if (_quitButton != null) {
			super.show();
			_quitButton.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					dismiss();
					if (event == null) {
						System.exit(0);
					} else {
						event.quit();
					}
				}
			});
			return true;
		} else {
			return false;
		}
	}

}
