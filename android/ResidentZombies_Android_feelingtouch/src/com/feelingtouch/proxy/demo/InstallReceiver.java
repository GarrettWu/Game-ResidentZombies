package com.feelingtouch.proxy.demo;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.apsalar.sdk.ApsalarReceiver;
import com.sponsorpay.sdk.android.advertiser.InstallReferrerReceiver;

public class InstallReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		// GA
		// com.google.analytics.tracking.android.CampaignTrackingReceiver
		// CampaignTrackingReceiver gReceiver = new CampaignTrackingReceiver();
		// gReceiver.onReceive(context, intent);
		try {
			// Sponsorpay
			InstallReferrerReceiver spInstallReferrerReceiver = new InstallReferrerReceiver();
			spInstallReferrerReceiver.onReceive(context, intent);

			// TODO add receivers you need

			ApsalarReceiver aReceiver = new ApsalarReceiver();
			aReceiver.onReceive(context, intent);
		} catch (Exception e) {

		}
	}

}
