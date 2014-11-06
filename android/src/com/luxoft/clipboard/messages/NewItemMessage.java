package com.luxoft.clipboard.messages;

import android.os.Bundle;

public class NewItemMessage extends DeleteItemMessage {
	public static final String FIELD_NAME = "name";
	
	public String name;
	
	public NewItemMessage(String name, String ip) {
		super(ip);
		this.name = name;
	}
	
	public NewItemMessage(Bundle bundle) {
		super(bundle);
		name = bundle.getString(FIELD_NAME);
	}
	
	@Override
	public Bundle pack() {
		Bundle data = super.pack();
		data.putString(FIELD_NAME, name);
		return data;
	}
}
