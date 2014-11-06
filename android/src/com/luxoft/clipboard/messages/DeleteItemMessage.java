package com.luxoft.clipboard.messages;

import android.os.Bundle;

public class DeleteItemMessage implements Packable {
	public static final String FIELD_IP = "ip";
	
	public String ip;
	
	public DeleteItemMessage(String ip) {
		this.ip = ip;
	}
	
	public DeleteItemMessage(Bundle bundle) {
		ip = bundle.getString(FIELD_IP);
	}
	
	@Override
	public Bundle pack() {
		Bundle data = new Bundle();
		data.putString(FIELD_IP, ip);
		return data;
	}
}
