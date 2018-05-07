package com.feelingtouch.offerwall.gl3d;

import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import com.feelingtouch.bannerad.util.AndroidUtil;
import com.sponsorpay.sdk.android.SponsorPay;
import com.sponsorpay.sdk.android.advertiser.SponsorPayAdvertiser;
import com.sponsorpay.sdk.android.publisher.currency.CurrencyServerAbstractResponse;
import com.sponsorpay.sdk.android.publisher.currency.CurrencyServerDeltaOfCoinsResponse;
import com.sponsorpay.sdk.android.publisher.currency.SPCurrencyServerListener;
import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyNotifier;
import com.tapjoy.TapjoySpendPointsNotifier;

abstract public class FTOfferWallActivity extends Activity implements
		TapjoyNotifier, TapjoySpendPointsNotifier, SPCurrencyServerListener {
	// 这里的message写成通用的
	private String _successMessage = "";
	private static int _tapjoyTempCount = 0;

	private Object lock = new Object();
	private static boolean _hasSponsorpayRegistered = false;

	@Override
	public void onResume() {
		super.onResume();
		try {
			SponsorPay.start(getSponsorpayId(),
					AndroidUtil.getAndroidUniqueID(this),
					getSponsorpaySecurityToken(), this);

			if (!_hasSponsorpayRegistered) {
				SponsorPayAdvertiser.register(this);
				_hasSponsorpayRegistered = !_hasSponsorpayRegistered;
			}

		} catch (RuntimeException e) {
			Log.e("OFFERWALL", e.getLocalizedMessage());
		}
	}

	private void notifyGetSuccess() {
		if (_offerWallHandler != null) {
			_offerWallHandler.sendEmptyMessage(0);
		}
	}

	protected Handler _offerWallHandler = new Handler() {
		@Override
		public void handleMessage(Message message) {
			super.handleMessage(message);
			Toast.makeText(FTOfferWallActivity.this, _successMessage,
					Toast.LENGTH_LONG).show();
		}
	};

	@Override
	public void getUpdatePoints(String currencyName, int pointTotal) {
		if (pointTotal > 0) {
			_tapjoyTempCount = pointTotal;
			TapjoyConnect.getTapjoyConnectInstance().spendTapPoints(pointTotal,
					this);
		}
	}

	@Override
	public void getUpdatePointsFailed(String error) {
		Log.e("OFFERWALL", "getUpdatePointsFailed");
	}

	@Override
	public void getSpendPointsResponse(String currencyName, int pointTotal) {
		synchronized (lock) {
			addVirtualCoins(_tapjoyTempCount);
			_successMessage = getSuccessMesage(_tapjoyTempCount);
			_tapjoyTempCount = 0;

			Log.e("OFFERWALL", "getSpendPointsResponse");

			if (_tapjoyTempCount > 0) {
				notifyGetSuccess();
			}
		}
	}

	@Override
	public void onSPCurrencyServerError(CurrencyServerAbstractResponse response) {
		Log.e("OFFERWALL", "onSPCurrencyServerError:" + response.getErrorCode());
	}

	@Override
	public void onSPCurrencyDeltaReceived(
			CurrencyServerDeltaOfCoinsResponse response) {
		synchronized (lock) {
			int coins = (int) response.getDeltaOfCoins();
			addVirtualCoins(coins);
			Log.e("OFFERWALL", "onSPCurrencyDeltaReceived");

			_successMessage = getSuccessMesage(coins);

			if (coins > 0) {
				notifyGetSuccess();
			}
		}
	}

	public void addVirtualCoins(int count) {
		if (count == 0)
			return;

		addVitualCash(count);
	}

	public abstract void addVitualCash(int count);

	public abstract String getSuccessMesage(float count);

	public abstract String getSponsorpayId();

	public abstract String getSponsorpaySecurityToken();

	@Override
	public void getSpendPointsResponseFailed(String error) {

	}
}
