package com.feelingtouch.bannerad;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;

import com.feelingtouch.bannerad.load.BannerAdTool;
import com.feelingtouch.felad.FelDebug;

public class BannerAd {
	private static ExitBannerAd _exitDialog;
	private static SlideAd _slideAd;
	private static GameShowDialog _gameShowDialog;

	// 必须先调用initBannerAdDialogBmp之后再调用initBannerAdDialog
	public static void initBannerAdDialogBmp(Context ctx) {
		try {
			ImageManager.decodeBmp(ctx);
		} catch (OutOfMemoryError e) {
			e.printStackTrace();
			ImageManager.release();
			BannerAdTool.prob = 0;
		} catch (Exception e) {
			ImageManager.release();
			BannerAdTool.prob = 0;
		}
	}

	public static void initExitAdDialog(Context ctx) {
		try {
			_exitDialog = new ExitBannerAd(ctx);
			Thread t = new Thread() {
				public void run() {
					while (true) {
						if (BannerAdTool.INIT_DATA_FINISH) {
							break;
						}
						try {
							Thread.sleep(1000);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
					if (BannerAdTool.getList() != null
							&& BannerAdTool.getList().size() > 0) {
						_exitDialog.initDialog(BannerAdTool.getList());
					}
				}
			};
			t.start();
		} catch (Exception e) {
			e.printStackTrace();
			ImageManager.release();
			BannerAdTool.prob = 0;
		}
	}

	private static void initGameAdShowDialog(Context ctx) {
		if (BannerAdTool.getGameadForShow() != null
				&& BannerAdTool.showAdDrawable != null) {

			FelDebug.e("init GameAdShowDialog");

			BannerAdTool.hasGameShowed = false;
			_gameShowDialog = new GameShowDialog(ctx);
			_gameShowDialog.initData(
					BannerAdTool.getGameadForShow().marketLink,
					BannerAdTool.getGameadForShow().httpLink,
					BannerAdTool.getGameadForShow().packageName,
					BannerAdTool.showAdDrawable);
		} else {
			_gameShowDialog = null;
		}
	}

	public static void initSlideAd(SlideAd slideAd) {
		_slideAd = slideAd;
		if (BannerAdTool.getList() != null && BannerAdTool.getList().size() > 0) {
			_slideAd.init(BannerAdTool.getList().get(0));
		}
	}

	public static void showGameShowDialog(Activity act) {
		if (_gameShowDialog == null) {
			initGameAdShowDialog(act);
		}
		if (_gameShowDialog != null && !BannerAdTool.hasGameShowed) {
			_gameShowDialog.show();

			BannerAdTool.hasGameShowed = true;
			BannerAdTool.getGameadForShow().showCount++;
			BannerAdTool.saveGameADToJson(BannerAdTool.LOCAL_GAMELIST);
		}
	}

	public static void showGameShowDialogInNeed(Activity act) {
		if (_gameShowDialog == null) {
			initGameAdShowDialog(act);
		}
		if (_gameShowDialog != null) {
			_gameShowDialog.show();
			BannerAdTool.hasGameShowed = true;
			BannerAdTool.getGameadForShow().showCount++;
			BannerAdTool.saveGameADToJson(BannerAdTool.LOCAL_GAMELIST);
		}
	}

	public static void showGameShowDialog(Activity act, CloseEvent event) {
		if (_gameShowDialog == null) {
			initGameAdShowDialog(act);
		}
		if (_gameShowDialog != null && !BannerAdTool.hasGameShowed) {
			_gameShowDialog.showWithAction(event);
			BannerAdTool.hasGameShowed = true;
			BannerAdTool.getGameadForShow().showCount++;
			BannerAdTool.saveGameADToJson(BannerAdTool.LOCAL_GAMELIST);
		} else {
			if (event != null && !BannerAdTool.hasGameShowed) {
				BannerAdTool.hasGameShowed = true;
				event.closeActionFinish();
			}
		}
	}

	// 在其他游戏Back按键的时候调用，询问确定要退出吗？

	public static void showExitAd(final QuitEvent event) {
		showExitAd(null, event);
	}

	public static void showSlideAd(int maginTop) {
		if (_slideAd != null) {
			_slideAd.show(maginTop);
		}
	}

	public static void setSlideAdIsIcon(boolean isIconAd) {
		if (_slideAd != null) {
			_slideAd.setIsIcon(isIconAd);
		}
	}

	public static void showSlideAd(int x, int y) {
		if (_slideAd != null) {
			_slideAd.show(x, y);
		}
	}

	public static void dissmissSlideAd() {
		if (_slideAd != null) {
			_slideAd.dismiss();
		}
	}

	public static void setSlideAdSize(int width, int height) {
		if (_slideAd != null) {
			_slideAd.setSize(width, height);
		}
	}

	public static void setSlideAdDipSize(int dipWidth, int dipHeight) {
		if (_slideAd != null) {
			_slideAd.setDipSize(dipWidth, dipHeight);
		}
	}

	public static void setSlideAdHeight(int height) {
		if (_slideAd != null) {
			_slideAd.setHeight(height);
		}
	}

	public static void setSlideAdWidth(int width) {
		if (_slideAd != null) {
			_slideAd.setWidth(width);
		}
	}

	public static void setDipSlideAdHeight(int dipHeight) {
		if (_slideAd != null) {
			_slideAd.setDipHeight(dipHeight);
		}
	}

	public static void setDipSlideAdWidth(int dipWidth) {
		if (_slideAd != null) {
			_slideAd.setDipWidth(dipWidth);
		}
	}

	public static void refreshSlideAd() {
		if (_slideAd != null) {
			_slideAd.refresh();
		}
	}

	public static void showExitAd(Activity activity, final QuitEvent event) {
		if (BannerAdTool.getList() != null && BannerAdTool.getList().size() > 0) {
			if (_exitDialog != null) {
				_exitDialog.show(event);
			} else {
				showExitDialogDefault(activity, event);
				return;
			}
		} else {
			showExitDialogDefault(activity, event);
			return;
		}
	}

	public static void releasDefaultDialog() {
		ImageManager.release();
	}

	public static void batchInit(Activity act) {
		BannerAdTool.IS_RUNNING = true;
		BannerAd.initExitAdDialog(act);
	}

	private static void showExitDialogDefault(final Activity activity,
			final QuitEvent event) {
		if (activity == null) {
			System.exit(0);
			return;
		}
		Dialog dialog = new AlertDialog.Builder(activity)
				.setTitle(R.string.banner_quit_question)
				.setPositiveButton(activity.getString(R.string.banner_yes),
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {
								dialog.dismiss();
								if (event != null) {
									event.quit();
								} else {
									System.exit(0);
								}
							}
						})
				.setNegativeButton(activity.getString(R.string.banner_no),
						new DialogInterface.OnClickListener() {

							public void onClick(DialogInterface dialog,
									int which) {
								dialog.dismiss();
							}
						}).create();
		dialog.show();
	}
}
