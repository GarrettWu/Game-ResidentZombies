package com.wiyun.engine.skeleton;

import java.util.ArrayList;
import java.util.List;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.apsalar.sdk.Apsalar;
import com.cafgame.encrypt.CafGameHelper;
import com.cafgame.residentzombies.R;
import com.example.android.trivialdrivesample.util.IabHelper;
import com.example.android.trivialdrivesample.util.IabResult;
import com.example.android.trivialdrivesample.util.Inventory;
import com.example.android.trivialdrivesample.util.Purchase;
import com.feelingtouch.bannerad.BannerAd;
import com.feelingtouch.bannerad.QuitEvent;
import com.feelingtouch.bannerad.SlideAd;
import com.feelingtouch.bannerad.util.OfferWallUtil;
import com.feelingtouch.felad.FelAd;
import com.feelingtouch.felad.FelAdManager;
import com.feelingtouch.felad.FelDebug;
import com.feelingtouch.felad.GameShowListener;
import com.feelingtouch.offerwall.gl3d.FTOfferWallActivity;
import com.google.ads.AdView;
import com.google.analytics.tracking.android.EasyTracker;
import com.google.analytics.tracking.android.GoogleAnalytics;
import com.google.analytics.tracking.android.MapBuilder;
import com.google.analytics.tracking.android.Tracker;
import com.google.android.gms.games.Player;
import com.google.example.games.basegameutils.GameHelper;
import com.vungle.sdk.VunglePub;
import com.wiyun.engine.nodes.Director;
import com.wiyun.engine.nodes.Director.IDirectorLifecycleListener;
import com.wiyun.engine.opengl.WYGLSurfaceView;
import com.wiyun.engine.utils.PrefUtil;

public class Skeleton extends FTOfferWallActivity implements IDirectorLifecycleListener, GameHelper.GameHelperListener {
	static {
		System.loadLibrary("wiengine");
		System.loadLibrary("box2d");
		System.loadLibrary("wisound");
		System.loadLibrary("wiengineskeleton");
		
//		System.load("/data/data/com.cafgame.residentzombies/lib/libwiengine.so");
//		System.load("/data/data/com.cafgame.residentzombies/lib/libbox2d.so");
//		System.load("/data/data/com.cafgame.residentzombies/lib/libwisound.so");
//		System.load("/data/data/com.cafgame.residentzombies/lib/libwiengineskeleton.so");
	}
	
	protected WYGLSurfaceView mGLView;
	
	private AdView adView;
	private boolean mStarted;
	private static Handler handler;
	public static Skeleton instance;
	
	public native void nativeSetAssetManager(AssetManager ass);
	
	private static Tracker mTracker, mTrackerMine;
	
	private static boolean isGameDebug = true;
	
	private static void showLog(String msg) {
		if (!isGameDebug)
			return;
		
		System.out.println(msg);
	}
	
    @Override
	public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        instance = this;
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.main);

		CafGameHelper.getInstance().init(this);
		
		nativeSetAssetManager(this.getAssets());

		mGLView = (WYGLSurfaceView) this.findViewById(R.id.glview);
		
        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        
        DisplayMetrics dm = new DisplayMetrics();  
        dm = getResources().getDisplayMetrics();  
        int screenWidth  = dm.widthPixels;      // ÆÁÄ»¿í£¨ÏñËØ£¬Èç£º480px£©  
        int screenHeight = dm.heightPixels;     // ÆÁÄ»¸ß£¨ÏñËØ£¬Èç£º800px£©  
        Director.getInstance().setScaleMode(Director.SCALE_MODE_BASE_SIZE_FIT_XY);
        Director.getInstance().setBaseSize(screenWidth, screenHeight);
        
        Director.getInstance().addLifecycleListener(this);

        try {
        	showLog("VunglePub init start...");
        	VunglePub.init(this, "com.cafgame.residentzombies");//
        	showLog("VunglePub init end...");
        	
        	initFT();
        	initGA();
        	this.initGoogleLeaderBoard();
        	initGoogleBilling();
        } catch (Exception e) {
        	e.printStackTrace();
        }
		
		handler = new Handler() {
			public void handleMessage(Message msg) {
				try {
					switch (msg.what) {
					case BACK_KEY_DOWN:
						BannerAd.showExitAd(Skeleton.this, new QuitEvent() {
							public void quit() {// TODO
								FelAdManager.trackEnd(Skeleton.this);
								Skeleton.this.finish();
								android.os.Process
										.killProcess(android.os.Process.myPid());
							}
						});

						break;
					case SHOW_SLIDE_AD:
						break;
					case HIDE_SLIDE_AD:
						break;
					case SHOW_OFFER_WALL_TAPJOY:
						OfferWallUtil.showOfferWall(Skeleton.this,
								Constant.TAPJOY_ID);
						break;
					case SHOW_OFFER_WALL_SPONSOR:
						OfferWallUtil.showOfferWall(Skeleton.this,
								Constant.SPONSOR_ID);
						break;
					case REPORT_EVENT:
						Bundle b = msg.getData();
						String category = b.getString("category");
						String action = b.getString("action");
						String label = b.getString("label");
						Long value = b.getLong("value");
						showLog(value+"");
						
						mTrackerMine.send(MapBuilder.createEvent(category, // Event
								action, // Event action (required)
								label, // Event label
								value) // Event value
								.build());
						
						if (!category.equals("Extra")) {
							mTracker.send(MapBuilder.createEvent(category, // Event
								action, // Event action (required)
								label, // Event label
								value) // Event value
								.build());
						}
						break;
					case PURCHASE_ITEM_START:
						Bundle b1 = msg.getData();
						int itemId = b1.getInt("itemId");

						buyItem(itemId);
						break;
					case PURCHASE_ITEM_END:
						Bundle b2 = msg.getData();
						int itemId1 = b2.getInt("itemId");
						purchaseItem(itemId1);
						hideAd();
						break;
					case SHOW_ADMOB_AD:
						showAdmobAdImpl();
						break;
					case HIDE_ADMOB_AD:
						dismissAdmobAdImpl();
						break;
					case SHOW_WEB_URL:
						String url = (String) msg.obj;
						Uri uri = Uri.parse(url);
						startActivity(new Intent(Intent.ACTION_VIEW, uri));
						break;
					case SHOW_TOAST:
						String msgTxt = (String) msg.obj;
						Toast.makeText(getApplicationContext(), msgTxt,
								Toast.LENGTH_LONG).show();
						break;
					case SHOW_CHARTBOOST:
						// cb.showInterstitial();
						showPromotion();
						break;
					case Constant.MESSGE_GAMESHOW_DIALOG:
						BannerAd.showGameShowDialogInNeed(Skeleton.this);
						break;
					case Constant.TEST:
						Toast.makeText(
								getApplicationContext(),
								"Reward you "
										+ testTmp
										+ " gold coins for your finishing offer wall tasks",
								Toast.LENGTH_LONG).show();
						break;
					case SHOW_LEADERBOARD:
						if (gameHelper.isSignedIn()) {
							startActivityForResult(gameHelper.getGamesClient()
									.getAllLeaderboardsIntent(), 5000);
						} else {
							gameHelper.beginUserInitiatedSignIn();
						}
						break;
					case SUBMIT_LEADERBOARD:
						try {
							int score = (Integer) msg.obj;
							if (gameHelper.isSignedIn()) {
								showLog("submit score directly:" + score);
								gameHelper
										.getGamesClient()
										.submitScore(
												getString(R.string.leaderboard_endless_highscore),
												score);
							} else {
//								Toast.makeText(getApplicationContext(),
//										"google play service not loggedin...",
//										Toast.LENGTH_LONG).show();
							}
						} catch (Exception e) {

						}
						break;
					case SUBMIT_SCORE_AND_SHOWLB:
						int score1 = (Integer) msg.obj;
						if (gameHelper.isSignedIn()) {
							showLog("submit score:" + score1);
							gameHelper
									.getGamesClient()
									.submitScore(
											getString(R.string.leaderboard_endless_highscore),
											score1);
							showLeaderBoard();
						} else {
							gameHelper.beginUserInitiatedSignIn();
						}
						break;
					case RELOAD_TEX:
						onResumeGame();
						break;
					case RELOAD_TEX_END:
						isReloaingTex = false;
						Skeleton.this.closeProgress();
						break;
					case SHOW_PROGRESS:
						Skeleton.this.showProgress();
						break;
					case HIDE_PROGRESS:
						Skeleton.this.closeProgress();
						break;
					case SHOW_NOTLEGAL:
//						if (checkRet != 1) {
						final Dialog dialog = 
						new AlertDialog.Builder(instance)
						.setMessage("Sorry, this application is hacked. Please download legal versions.")
						.setCancelable(false)
						.setPositiveButton("Ok", 
								new DialogInterface.OnClickListener() {
									@Override
									public void onClick(DialogInterface dialog, int which) {
										instance.finish();
										android.os.Process
												.killProcess(android.os.Process.myPid());
									}
								}).create();
						dialog.show();
//						}
						break;
					case SHOW_RATEME_GOOGLE:
						Skeleton.this.startActivity(new Intent(
								Intent.ACTION_VIEW, Uri
										.parse("market://details?id="
												+ "com.cafgame.residentzombies")));
						break;
					case PLAY_VUNGLE_VEDIO:
						if (VunglePub.isVideoAvailable(true)) {
							VunglePub.displayAdvert();
							reportGaEvent("IAP", "VungleVedio", "play", 0);
							PrefUtil.setIntPref("PLAY_VUNGLE", 0);
							showLog("play vungle success...");
						}
						showLog("play vungle failed...");
						break;
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};

//		showLog(""+this.getApplicationContext().getPackageName());
//		AppRater.app_launched(this);
		
		Apsalar.startSession(this, "GarrettWu", "FC2b8vDU");
    }
	
    @Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {

			if (FelAdManager.cb != null && FelAdManager.cb.onBackPressed()) {
				return true;
			}

			return true;
		}

		return super.onKeyDown(keyCode, event);
	}
    
    public void initFT() {
    	FelDebug.DEBUG = false;
    	
    	BannerAd.batchInit(this);
		BannerAd.initSlideAd((SlideAd) findViewById(R.id.slide_ad1));
		BannerAd.setSlideAdDipSize(340, 70);
		initFelAd();
		
//		try {
//			Gift gift = AdsTransport.INSTANCE.getGift(
//					Skeleton.this.getPackageName(), "123456789", "654321");
//		} catch (RpcException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
    }
    
    private static FelAd _felAd;
    private void initFelAd() {

		FelAdManager.initFullScreenAds(this, Constant.CHART_BOOST_APP_ID,
				Constant.CHART_BOOST_SIGNATURE);
		FelAdManager.init(this, Constant.ADMOB_ID, getPackageName(), false);
		_felAd = new FelAd(this, R.id.ad_wrapper);
		FelAdManager.add(_felAd);
		FelAdManager.loadConfig();

		try {
			FelAdManager.trackInit(getApplicationContext(), Constant.TAPJOY_ID,
					Constant.TAPJOY_KEY, Constant.FLURRY);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
    
	public void showPromotion() {
		System.err.println("show promotion.");
		FelAdManager.showFullScreenAd(this, new GameShowListener() {
			@Override
			public void showGameShowDialog() {
				showGameshowDialog();
			}
		});
	}
	
	public void showGameshowDialog() {
		if (handler != null) {
			handler.sendEmptyMessage(Constant.MESSGE_GAMESHOW_DIALOG);
		}
	}
    
	public void initGA() {
//		GoogleAnalytics.getInstance(this).setDryRun(true);
		
		mTracker = GoogleAnalytics.getInstance(this).getTracker(Constant.GA_PROPERTY_ID);

		mTrackerMine = GoogleAnalytics.getInstance(this).getTracker(Constant.GA_PROPERTY_ID_MINE);
	}
	
	GameHelper gameHelper;
	public void initGoogleLeaderBoard() {
		gameHelper = new GameHelper(this);
		gameHelper.setup(this, GameHelper.CLIENT_GAMES | GameHelper.CLIENT_PLUS);
	}
	@Override
	public void onSignInFailed() {
		showLog("google service login failed...");
		Toast.makeText(getApplicationContext(), "google service login failed...",
			     Toast.LENGTH_LONG).show();
	}

	@Override
	public void onSignInSucceeded() {
		// Set the greeting appropriately on main menu
        Player p = gameHelper.getGamesClient().getCurrentPlayer();
        String displayName;
        if (p == null) {
        	showLog("mGamesClient.getCurrentPlayer() is NULL!");
            displayName = "???";
        } else {
            displayName = p.getDisplayName();
        }
        
        Toast.makeText(getApplicationContext(), "google service login succeed...",
			     Toast.LENGTH_LONG).show();
        showLeaderBoard();
        showLog("google service login successed, player is:" + displayName);
	}
	
	public static void showLeaderBoard() {
		Message msg = new Message();
    	msg.what = SHOW_LEADERBOARD;
    	handler.sendMessage(msg);
	}
	
	public static void submitScoreAndShowLB(int score) {
		Message msg = new Message();
    	msg.what = SUBMIT_SCORE_AND_SHOWLB;
    	msg.obj = score;
    	handler.sendMessage(msg);
	}
	
	public static void submitScoreToLeaderBoard(int score) {
		Message msg = new Message();
    	msg.what = SUBMIT_LEADERBOARD;
    	msg.obj = score;
    	handler.sendMessage(msg);
	}
	
	public static void reportGaEvent(String category, String action, String label, int value) {
    	Message msg = new Message();
    	msg.what = REPORT_EVENT;
    	
    	Bundle b = new Bundle();
    	b.putString("category", category);
    	b.putString("action", action);
    	b.putString("label", label);
    	b.putLong("value", value);
    	msg.setData(b);
    	
    	handler.sendMessage(msg);
    	
    	showLog("report event ga:"+category+"/"+action+"/"+label+"/"+value);
	}
	
	public void showAdmobAdImpl() {
		if (_felAd != null) {
			_felAd.show();
		}
	}
	
	public void dismissAdmobAdImpl() {
		if (_felAd != null) {
			_felAd.dismiss();
		}
	}
	
	public void refreshAdmobAdImpl() {
	}
	
    public static void showWebUrl(String url) {
    	Message msg = new Message();
    	msg.what = SHOW_WEB_URL;
    	msg.obj = url;
    	
    	handler.sendMessage(msg);
    }
    
	public static void showAd(int type) {
		handler.sendEmptyMessage(SHOW_ADMOB_AD);
	}
	
	public static void hideAd() {
		handler.sendEmptyMessage(HIDE_ADMOB_AD);
	}
	
	public static void showExit() {
		handler.sendEmptyMessage(BACK_KEY_DOWN);
	}
	
	public static void showOfferWall(int type) {
		handler.sendEmptyMessage(SHOW_OFFER_WALL_SPONSOR);
	}
	
	public static void showChartBoost() {
		handler.sendEmptyMessage(SHOW_CHARTBOOST);
	}
	
	public static void showRateMeForGooglePlay() {
		handler.sendEmptyMessage(SHOW_RATEME_GOOGLE);
	}
	
	public static void callGC() {
		System.gc();
	}
	
	public static void reportEvent(String event_id, String label) {
    	Message msg = new Message();
    	msg.what = REPORT_EVENT;
    	
    	Bundle b = new Bundle();
    	b.putString("eventId", event_id);
    	b.putString("eventContent", label);
    	msg.setData(b);
    	
    	handler.sendMessage(msg);
	}
	
	public static void startPurchaseGameItem(int itemId) {
		Message msg = new Message();
    	msg.what = PURCHASE_ITEM_START;
    	
    	Bundle b = new Bundle();
    	b.putInt("itemId", itemId);
    	msg.setData(b);
    	
    	handler.sendMessage(msg);
	}
	
	public static void endPurchaseGameItem(int itemId) {
    	Message msg = new Message();
    	msg.what = PURCHASE_ITEM_END;
    	
    	Bundle b = new Bundle();
    	b.putInt("itemId", itemId);
    	msg.setData(b);
    	
    	handler.sendMessage(msg);
	}
	
	public static int checkGamePackageName() {
		int ret = 0;
		try {
			if (instance.getApplicationContext().getPackageName().equals("com.cafgame.residentzombies")) {
				ret = 1;
			}
		} catch (Exception e) {
			
		}
		
		showLog("retttt:"+ret+"");
		if (ret != 1) {
			handler.sendEmptyMessage(SHOW_NOTLEGAL);
		}
		return ret;
	}
	
	public String getGamePackageName() {
		String pName = "";
		try {
			pName = instance.getPackageName();
		} catch (Exception e) {
			
		}
		return pName;
	}
	
	public static int checkSignature() {
		if (isGameDebug || CafGameHelper.getInstance().checkCertificate()) {
			return 1;
		}
		
		handler.sendEmptyMessage(SHOW_NOTLEGAL);
		return 0;
	}
	
	public static void playVungleVedio() {
		handler.sendEmptyMessage(PLAY_VUNGLE_VEDIO);
	}
	
    public String getUserId() {
    	TelephonyManager telephonyManager=(TelephonyManager) this.getSystemService(Context.TELEPHONY_SERVICE);
    	String imei=telephonyManager.getDeviceId();
    	return imei+";";
    }

	public static final int BACK_KEY_DOWN = 1;
	public static final int SHOW_SLIDE_AD = 2;
	public static final int HIDE_SLIDE_AD = 3;
	public static final int SHOW_OFFER_WALL_TAPJOY = 4;
	public static final int SHOW_OFFER_WALL_SPONSOR = 5;
	public static final int REPORT_EVENT = 6;
	public static final int PURCHASE_ITEM_START = 7;
	public static final int PURCHASE_ITEM_END = 8;
	public static final int SHOW_ADMOB_AD = 9;
	public static final int HIDE_ADMOB_AD = 10;
	public static final int SHOW_WEB_URL = 11;
	public static final int SHOW_TOAST = 12;
	public static final int SHOW_CHARTBOOST = 13;
	public static final int SHOW_LEADERBOARD = 14;
	public static final int SUBMIT_LEADERBOARD = 15;
	public static final int SUBMIT_SCORE_AND_SHOWLB = 16;
	public static final int RELOAD_TEX = 17;
	public static final int RELOAD_TEX_END = 18;
	public static final int SHOW_PROGRESS = 19;
	public static final int HIDE_PROGRESS = 20;
	public static final int SHOW_NOTLEGAL = 21;
	public static final int SHOW_RATEME_GOOGLE = 22;
	public static final int PLAY_VUNGLE_VEDIO = 23;
	
	private native void nativeStart();
	private native void addGem(int gemnum);
	private native void addGold(int goldnum);
	private native void purchaseItem(int itemId);
	private native void replayBGM();
	private native void onResumeGame();
	private native int isShopOpening();
	private native void setShopNeedReloadTex();
	private native int checkPackageNameJNI();
    
	public static void onResumeGameEnd() {
		handler.sendEmptyMessage(RELOAD_TEX_END);
	}
	
	boolean isGamePaused = false;
	@Override
    public void onPause() {
        super.onPause();
        showLog("on pause...");
        
        isGamePaused = true;
        
        
        Director.getInstance().pause();
        com.wiyun.engine.sound.AudioManager.pauseBackgroundMusic();
        
        VunglePub.onPause();
    }

	boolean isReloaingTex = false;
    @Override
    public void onResume() {
        super.onResume();
        showLog("on resume... =========================");
        Director.getInstance().resume();
        
        OfferWallUtil.getOfferWallPoints(Skeleton.this,
				Constant.SPONSOR_SECURITY_TOKEN, Constant.SPONSOR_ID);
        
        com.wiyun.engine.sound.AudioManager.resumeBackgroundMusic();
        
        if (isGamePaused) {
        	isGamePaused = false;
        	//in shop layer, no need to reload tex on resume.
        	showLog("isShopOpening:"+isShopOpening()+"");
        	if (isShopOpening() == 0) {
        		this.showProgress();
        		handler.sendEmptyMessageDelayed(RELOAD_TEX, 100);
        	} else {
        		setShopNeedReloadTex();
        	}
        }
        
        VunglePub.onResume();
    }
    
    public static void showUIProgress() {
    	handler.sendEmptyMessage(SHOW_PROGRESS);
    }
    
    public static void hideUIProgress() {
    	handler.sendEmptyMessage(HIDE_PROGRESS);
    }

    @Override
    public void onDestroy() {
    	Director.getInstance().end();
    	showLog("on destory...");
        super.onDestroy();
        
        try {
        	if (FelAdManager.cb != null) {
    			FelAdManager.cb.onDestroy(this);
    		}
        	
        	instance = null;
        } catch (Exception e) {
        	e.printStackTrace();
		}
    }
    
    @Override
    public void onStart() {
        super.onStart();
        showLog("on start...");
        try {
    		if (FelAdManager.cb != null) {
    			FelAdManager.cb.onStart(this);
    		}
            
            EasyTracker.getInstance(this).activityStart(this);
		} catch (Exception e) {
			e.printStackTrace();
		}
    }
    
    @Override
    public void onStop() {
        super.onStop();
        showLog("on stop...");
        try {
        	if (FelAdManager.cb != null) {
    			FelAdManager.cb.onStop(this);
    		}
        	
        	Apsalar.endSession();
        	EasyTracker.getInstance(this).activityStop(this);
		} catch (Exception e) {
			e.printStackTrace();
		}
    }

	public void onDirectorEnded() {
	}

	public void onDirectorPaused() {
	}

	public void onDirectorResumed() {
	}

	public void onDirectorScreenCaptured(String path) {
	}

	public void onSurfaceChanged(int w, int h) {
    	boolean landscape = getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
    	if(!mStarted && (landscape && w >= h || !landscape && h >= w)) {
    		mStarted = true;
    		nativeStart();
    	}
	}

	public void onSurfaceCreated() {
	}

	public void onSurfaceDestroyed() {
	}
	
    int testTmp = 0;
    
	@Override
	public void addVitualCash(int count) {
		testTmp = count;
		handler.sendEmptyMessage(Constant.TEST);
		
		showLog("addVitualCash:"+count);
		
		addGold(count);
//		reportGaEvent("IAP", "CompleteOfferwall", "["+testTmp+"]", 0);
		reportGaEvent("IAP", "CompleteOfferwall", "get", testTmp);
	}

	@Override
	public String getSuccessMesage(float count) {
		showLog("get coin:"+count+"");
		
		return "Reward you " + (int) count
				+ " gold coins for your finishing offer wall tasks";
	}
	
	@Override
	public String getSponsorpayId() {
		return Constant.SPONSOR_ID;
	}

	@Override
	public String getSponsorpaySecurityToken() {
		return Constant.SPONSOR_SECURITY_TOKEN;
	}
	
	protected static final int INITIALIZE_SUCCESS = 1;
	protected static final int SAVE_TO_SERVER = 2;
	
	static final int RC_REQUEST = 10001;
	static final String TAG = "aaa";
	
	static final int FT_RZ_COIN_1 = 0;
	static final int FT_RZ_COIN_2 = 1;
	static final int FT_RZ_COIN_3 = 2;
	static final int FT_RZ_CASH_1 = 3;
	static final int FT_RZ_CASH_2 = 4;
	static final int FT_RZ_CASH_3 = 5;
	static final int FT_RZ_GIFTPACK_1 = 6;
	static final int FT_RZ_DOUBLECOIN_1 = 7;
	
	static String[] PURCHASE_NAMES = { 
		"6000 coins",
		"18500 coins",
		"45000 coins",
		"45 cash",
		"150 cash",
		"450 cash",
		"gift pack: 3000 coins & 15 cash",
		"Double coins for ResidentZombies 4.99"
	};
	
	static String[] PURCHASE_IDS = { 
		"ft_rz_coin_1",
		"ft_rz_coin_2",
		"ft_rz_coin_3",
		"ft_rz_cash_1",
		"ft_rz_cash_2",
		"ft_rz_cash_3",
		"ft_rz_giftpack_1",
		"ft_rz_doublecoin_1",
	};
	
	static double[] PURCHASE_PRICE = { 
		1.99d,
		4.99d,
		9.99d,
		2.99d,
		7.99d,
		19.99d,
		0.99d,
		4.99d,
	};
	
	IabHelper mHelper;
	String currentSku;
	
	void complain(String message) {
		showLog("**** TrivialDrive Error: " + message);
        alert(message);
    }
	void alert(String message) {
        AlertDialog.Builder bld = new AlertDialog.Builder(this);
        bld.setMessage(message);
        bld.setNeutralButton("OK", null);
        showLog("Showing alert dialog: " + message);
        bld.create().show();
    }
	
	boolean isGoogleBillingInited = false;
	public void initGoogleBilling() {
		String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAz72ynGvwtUmdMcuGmShaE1WDuBErb3OZBPbmlXwkbhiEmzQ5KyVUk0ELxY6lEz7yU3Qxr6JYuqQycZKRf5q3H+BukxOF+d1mrpO841BUnOPKIC4FsSr7U8iKPobrwvavoUVDhu1yuoF7Rur46uTmWOmxzhHAc6PrBH8i4TyzfshTlU6Njf6yN+dkJm1HMIeUWPODAgr94zE6RlbtrfLWgHR9w/LMx61TpqWYf+3JECrK0Ov3LECxXXIM50ExelgY72zTExs7DMXmOm+d4xnXvgfx8St6qanb0sO9S24HBEuBW1+raavDnNl3BgxLCmRFm7sDofMxRpKC0jKIvotSGQIDAQAB";
		
		mHelper = new IabHelper(this, base64EncodedPublicKey);
        
        // enable debug logging (for a production application, you should set this to false).
        mHelper.enableDebugLogging(false);

        isGoogleBillingInited = false;
        // Start setup. This is asynchronous and the specified listener
        // will be called once setup completes.
        showLog("Starting setup.");
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
            	showLog("Setup finished.");

                if (!result.isSuccess()) {
                	isGoogleBillingInited = false;
                    // Oh noes, there was a problem.
                	showLog("Problem setting up in-app billing: " + result);
                    return;
                }
                
                isGoogleBillingInited = true;
                mHelper.queryInventoryAsync(mGotInventoryListener);
            }
        });
	}
	
	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
		public void onQueryInventoryFinished(IabResult result,
				Inventory inventory) {
			try {
				if (result.isFailure()) {
					complain("Failed to query inventory: " + result);
					return;
				}

				List<Purchase> purchases = new ArrayList<Purchase>();
				for (int i = FT_RZ_COIN_1; i <= FT_RZ_GIFTPACK_1; i++) {
					Purchase purchase = inventory.getPurchase(PURCHASE_IDS[i]);
					if (purchase != null && verifyDeveloperPayload(purchase)) {
						showLog(purchase.getSku() + " is in inv. consume it...");
						purchases.add(purchase);
					}
				}
				if (!purchases.isEmpty()) {
					mHelper.consumeAsync(purchases, mConsumeMultiFinishedListener);
				}
			} catch (Exception e) {

			}
		}
	};
	
	boolean _isFreedomUser = false;
	String pName = "cc.cz.madkite.freedom";
	String pName1 = "cc.madkite.freedom";
	
	private void checkFreedom() {
		PackageInfo info = null;
		_isFreedomUser = false;
		try {
			info = getApplicationContext().getPackageManager().getPackageInfo(
					pName, 0);
			if (info != null) {
				_isFreedomUser = true;
			}
		} catch (NameNotFoundException e) {
//			 e.printStackTrace();
		} catch (Exception e) {
//			 e.printStackTrace();
		}
		
		try {
			info = getApplicationContext().getPackageManager().getPackageInfo(
					pName1, 0);
			if (info != null) {
				_isFreedomUser = true;
			}
		} catch (NameNotFoundException e) {
//			 e.printStackTrace();
		} catch (Exception e) {
//			 e.printStackTrace();
		}
	}
	
	String myPayLoad = "fafewrwerfadfeeee33kklnzvadfsdg";
	boolean isBuyingItem = false;
	public void buyItem(int itemId) {
		try {
			if (isBuyingItem) {
				return;
			}
			
			checkFreedom();
			if (_isFreedomUser) {
				this.complain("Sorry, please don't use freedom iap cracker.");
				return;
			}
			
			if (!isGoogleBillingInited) {
				initGoogleBilling();
				this.complain("Sorry, the google app billing service is not available.");
				return;
			}
			isBuyingItem = true;
			showProgress();
			showLog("Launching purchase flow for "+PURCHASE_IDS[itemId]);

			/*
			 * TODO: for security, generate your payload here for verification.
			 * See the comments on verifyDeveloperPayload() for more info. Since
			 * this is a SAMPLE, we just use an empty string, but on a
			 * production app you should carefully generate this.
			 */
			String payload = myPayLoad;

			String goodId = PURCHASE_IDS[itemId];
//			goodId = "android.test.purchased";
			mHelper.launchPurchaseFlow(this, goodId, RC_REQUEST,
					mPurchaseFinishedListener, payload);
			currentSku = goodId;
			showLog("purchase start... " + currentSku);
			
//			doRealPurchase(currentSku, "123456");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
    
 // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
			try {
//				String sku;
				showLog("purchase end... " + currentSku);
				isBuyingItem = false;

				showLog("Purchase finished: " + result + ", purchase: "
						+ purchase);
				if (result.isFailure()) {
					showLog("result:" + result.getResponse());
					if (result.getResponse() != IabHelper.IABHELPER_USER_CANCELLED) {
						complain("Sorry, purchase failed. Please try again later.");
					}
					closeProgress();
					return;
				}
				if (!verifyDeveloperPayload(purchase)) {
					complain("Error purchasing. Authenticity verification failed.");
					closeProgress();
					return;
				}

				mHelper.consumeAsync(purchase, mConsumeFinishedListener);
//				closeProgress();
			} catch (Exception e) {
				e.printStackTrace();
			}
        }
    };
    
	// Called when consumption is complete
	IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
		public void onConsumeFinished(Purchase purchase, IabResult result) {
			try {
				showLog("Consumption finished. Purchase: " + purchase
						+ ", result: " + result);

				// if we were disposed of in the meantime, quit.
				if (mHelper == null)
					return;

				if (!result.isSuccess()) {
					complain("Error while consuming. You may restart game to try again.");
				} else {
					doRealPurchase(purchase);
					closeProgress();
				}
			} catch (Exception e) {

			}
		}
	};
    
    IabHelper.OnConsumeMultiFinishedListener mConsumeMultiFinishedListener = new IabHelper.OnConsumeMultiFinishedListener() {
		@Override
		public void onConsumeMultiFinished(List<Purchase> purchases,
				List<IabResult> results) {
	            try {
	            	if(results != null && results.size() > 0) {
                        for(int i = 0; i< results.size(); i++) {
                            if(results.get(i).isSuccess()) {
                                Purchase purchase = purchases.get(i);
                                doRealPurchase(purchase);
                            }
                        }
                    }
	            } catch (Exception ex) {
	            	ex.printStackTrace();
	            }
		}
	};
    
	private boolean checkOrderIdIsConsumed(String orderId) {
		boolean flag = false;
		SharedPreferences sharedPreferences = getSharedPreferences("ORDERS", Context.MODE_PRIVATE);
		int result = sharedPreferences.getInt(orderId, 0);
		if (result == 1) {
			flag = true;
			showLog("order " + orderId + " has been consumed, skip...");
		} else {
			Editor editor = sharedPreferences.edit();
			editor.putInt(orderId, 1);
			editor.commit();
			
			flag = false;
		}
		return flag;
	}
	
//	private void doRealPurchase(String sku, String orderId) {
////		if (checkOrderIdIsConsumed(orderId)) {
////			return;
////		}
//		
//		showLog("Consume successful." + sku);
//		
//		int itemId = -1;
//		if (sku.equals(PURCHASE_IDS[FT_RZ_COIN_1])) {
//			endPurchaseGameItem(FT_RZ_COIN_1);
//			itemId = FT_RZ_COIN_1;
//		} else if (sku.equals(PURCHASE_IDS[FT_RZ_COIN_2])) {
//			endPurchaseGameItem(FT_RZ_COIN_2);
//
//			itemId = FT_RZ_COIN_2;
//		} else if (sku.equals(PURCHASE_IDS[FT_RZ_COIN_3])) {
//			endPurchaseGameItem(FT_RZ_COIN_3);
//
//			itemId = FT_RZ_COIN_3;
//		} else if (sku.equals(PURCHASE_IDS[FT_RZ_CASH_1])) {
//			endPurchaseGameItem(FT_RZ_CASH_1);
//
//			itemId = FT_RZ_CASH_1;
//		} else if (sku.equals(PURCHASE_IDS[FT_RZ_CASH_2])) {
//			endPurchaseGameItem(FT_RZ_CASH_2);
//
//			itemId = FT_RZ_CASH_2;
//		} else if (sku.equals(PURCHASE_IDS[FT_RZ_CASH_3])) {
//			endPurchaseGameItem(FT_RZ_CASH_3);
//
//			itemId = FT_RZ_CASH_3;
//		} else if (sku.equals(PURCHASE_IDS[FT_RZ_GIFTPACK_1])) {
//			endPurchaseGameItem(FT_RZ_GIFTPACK_1);
//
//			itemId = FT_RZ_GIFTPACK_1;
//		} else if (sku.equals(PURCHASE_IDS[FT_RZ_DOUBLECOIN_1])) {
//			endPurchaseGameItem(FT_RZ_DOUBLECOIN_1);
//
//			itemId = FT_RZ_DOUBLECOIN_1;
//		}
//		Toast.makeText(getApplicationContext(),
//				"Congratulations! Purchase " + PURCHASE_NAMES[itemId] + " succeed.",
//				Toast.LENGTH_LONG).show();
//		if (itemId != -1) {
//			sendPurchaseCompleteEventToGA(orderId, sku,
//					itemId);
//		}
//	}
	
	private void doRealPurchase(Purchase purchase) {
		String sku = purchase.getSku();
		String orderId = purchase.getOrderId();
		
		if (checkOrderIdIsConsumed(orderId)) {
			return;
		}
		
		showLog("Consume successful." + sku);
		
		int itemId = -1;
		if (sku.equals(PURCHASE_IDS[FT_RZ_COIN_1])) {
			endPurchaseGameItem(FT_RZ_COIN_1);
			itemId = FT_RZ_COIN_1;
		} else if (sku.equals(PURCHASE_IDS[FT_RZ_COIN_2])) {
			endPurchaseGameItem(FT_RZ_COIN_2);

			itemId = FT_RZ_COIN_2;
		} else if (sku.equals(PURCHASE_IDS[FT_RZ_COIN_3])) {
			endPurchaseGameItem(FT_RZ_COIN_3);

			itemId = FT_RZ_COIN_3;
		} else if (sku.equals(PURCHASE_IDS[FT_RZ_CASH_1])) {
			endPurchaseGameItem(FT_RZ_CASH_1);

			itemId = FT_RZ_CASH_1;
		} else if (sku.equals(PURCHASE_IDS[FT_RZ_CASH_2])) {
			endPurchaseGameItem(FT_RZ_CASH_2);

			itemId = FT_RZ_CASH_2;
		} else if (sku.equals(PURCHASE_IDS[FT_RZ_CASH_3])) {
			endPurchaseGameItem(FT_RZ_CASH_3);

			itemId = FT_RZ_CASH_3;
		} else if (sku.equals(PURCHASE_IDS[FT_RZ_GIFTPACK_1])) {
			endPurchaseGameItem(FT_RZ_GIFTPACK_1);

			itemId = FT_RZ_GIFTPACK_1;
		} else if (sku.equals(PURCHASE_IDS[FT_RZ_DOUBLECOIN_1])) {
			endPurchaseGameItem(FT_RZ_DOUBLECOIN_1);

			itemId = FT_RZ_DOUBLECOIN_1;
		}
		Toast.makeText(getApplicationContext(),
				"Congratulations! Purchase " + PURCHASE_NAMES[itemId] + " succeed.",
				Toast.LENGTH_LONG).show();
		if (itemId != -1) {
			sendPurchaseCompleteEventToGA(orderId, sku,
					itemId);
		}
	}
	
    void sendPurchaseCompleteEventToGA(String orderId, String sku, int itemId) {
    	showLog("sending ecommerce event to ga:"+orderId+","+sku+","+itemId);
    	mTrackerMine.send(MapBuilder
    		      .createTransaction(orderId,       // (String) Transaction ID
    		                         "Google Play",   // (String) Affiliation
    		                         PURCHASE_PRICE[itemId],            // (Double) Order revenue
    		                         0.0d,            // (Double) Tax
    		                         0.0d,             // (Double) Shipping
    		                         "USD")            // (String) Currency code
    		      .build()
    		  );

      	mTrackerMine.send(MapBuilder
    		      .createItem(orderId,               // (String) Transaction ID
    		    		  PURCHASE_NAMES[itemId],      // (String) Product name
    		                  sku,                  // (String) Product SKU
    		                  "IAP",        // (String) Product category
    		                  PURCHASE_PRICE[itemId],                    // (Double) Product price
    		                  1L,                       // (Long) Product quantity
    		                  "USD")                    // (String) Currency code
    		      .build()
    		  );
      	
    	mTracker.send(MapBuilder
    		      .createTransaction(orderId,       // (String) Transaction ID
    		                         "Google Play",   // (String) Affiliation
    		                         PURCHASE_PRICE[itemId],            // (Double) Order revenue
    		                         0.0d,            // (Double) Tax
    		                         0.0d,             // (Double) Shipping
    		                         "USD")            // (String) Currency code
    		      .build()
    		  );

    	mTracker.send(MapBuilder
    		      .createItem(orderId,               // (String) Transaction ID
    		    		  PURCHASE_NAMES[itemId],      // (String) Product name
    		                  sku,                  // (String) Product SKU
    		                  "IAP",        // (String) Product category
    		                  PURCHASE_PRICE[itemId],                    // (Double) Product price
    		                  1L,                       // (Long) Product quantity
    		                  "USD")                    // (String) Currency code
    		      .build()
    		  );
    }
    
    /** Verifies the developer payload of a purchase. */
    boolean verifyDeveloperPayload(Purchase p) {
        String payload = p.getDeveloperPayload();
        
        /*
         * TODO: verify that the developer payload of the purchase is correct. It will be
         * the same one that you sent when initiating the purchase.
         * 
         * WARNING: Locally generating a random string when starting a purchase and 
         * verifying it here might seem like a good approach, but this will fail in the 
         * case where the user purchases an item on one device and then uses your app on 
         * a different device, because on the other device you will not have access to the
         * random string you originally generated.
         *
         * So a good developer payload has these characteristics:
         * 
         * 1. If two different users purchase an item, the payload is different between them,
         *    so that one user's purchase can't be replayed to another user.
         * 
         * 2. The payload must be such that you can verify it even when the app wasn't the
         *    one who initiated the purchase flow (so that items purchased by the user on 
         *    one device work on other devices owned by the user).
         * 
         * Using your own server to store and verify developer payloads across app
         * installations is recommended.
         */
        if (payload.equals(myPayLoad)) {
        	return true;
        }
        
        return false;
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	showLog("onActivityResult(" + requestCode + "," + resultCode + "," + data);
        gameHelper.onActivityResult(requestCode, resultCode, data);
        
        if (mHelper == null) return;

        // Pass on the activity result to the helper for handling
        if (!mHelper.handleActivityResult(requestCode, resultCode, data)) {
            // not handled, so handle it ourselves (here's where you'd
            // perform any handling of activity results not related to in-app
            // billing...
            super.onActivityResult(requestCode, resultCode, data);
        }
        else {
        	showLog("onActivityResult handled by IABUtil.");
        }
    }
    
    private ProgressDialog mProgress = null;
    public void showProgress() {
    	if (mProgress != null) {
    		showLog("is showing progress, no need to show again...");
    		return;
    	}
    	
    	showLog("show progress...");
    	mProgress = new ProgressDialog(Skeleton.this);
		mProgress.setMessage("Loading, please wait...");
		mProgress.setIndeterminate(false);
		mProgress.setCancelable(false);
		mProgress.show();
    }
    
    public void closeProgress() {
        try {
            if (mProgress != null) {
                mProgress.dismiss();
                mProgress = null;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}






