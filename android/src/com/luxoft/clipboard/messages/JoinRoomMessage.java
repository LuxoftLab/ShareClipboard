package com.luxoft.clipboard.messages;

import android.os.Bundle;

public class JoinRoomMessage extends DeleteItemMessage {
	public static final String FIELD_PASS = "pass";
	
	public String password;
	
	public JoinRoomMessage(String ip, String password) {
		super(ip);
		this.password = password;
	}
	
	public JoinRoomMessage(Bundle bundle) {
		super(bundle);
		password = bundle.getString(FIELD_PASS);
	}
	
	@Override
	public Bundle pack() {
		Bundle data = super.pack();
		data.putString(FIELD_PASS, password);
		return data;
	}

}
