package com.cafgame.encrypt;

import java.security.MessageDigest;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

public class CafGameHelper {
	private static CafGameHelper _instance = null;
	private String CafDebugSignature = "5b:15:80:e5:29:2b:3a:c5:d3:39:e9:9a:c1:41:d8:6f";
	private String CafTestSignature = "7c:c4:64:63:85:bf:37:5f:d8:0b:30:9d:4a:aa:77:2e";
	private String FTSignature = "ba:51:9c:4a:c2:68:ad:fd:24:f0:b8:c7:20:c6:20:26";
	
	public static CafGameHelper getInstance() {
		if (_instance == null) {
			_instance = new CafGameHelper();
		}
		return _instance;
	}
	
	private Activity m_context;
	
	public void init(Activity context) {
		m_context = context;
	}
	
	public boolean checkCertificate() {
		try {
			PackageManager pm = m_context.getPackageManager();
			Signature sig = pm.getPackageInfo(m_context.getPackageName(),
					PackageManager.GET_SIGNATURES).signatures[0];
			String md5Fingerprint = doFingerprint(sig.toByteArray(), "MD5");
//			System.out.println(md5Fingerprint + "");
			if (md5Fingerprint.equals(FTSignature)) {
				return true;
			}
		} catch (Exception e) {
		} finally {

		}
		return false;
	}

	protected static String doFingerprint(byte[] certificateBytes, String algorithm)
			throws Exception {
		MessageDigest md = MessageDigest.getInstance(algorithm);
		md.update(certificateBytes);
		byte[] digest = md.digest();
		
		String toRet = "";
		for (int i = 0; i < digest.length; i++) {
			if (i != 0)
				toRet += ":";
			int b = digest[i] & 0xff;
			String hex = Integer.toHexString(b);
			if (hex.length() == 1)
				toRet += "0";
			toRet += hex;
		}
		return toRet;
	}
}
