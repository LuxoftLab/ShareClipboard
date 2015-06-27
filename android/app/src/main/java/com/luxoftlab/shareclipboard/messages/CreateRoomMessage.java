package com.luxoftlab.shareclipboard.messages;

import android.os.Bundle;

public class CreateRoomMessage implements Packable {
	public static final String FIELD_NAME = "name",
							   FIELD_PASS = "pass";
	
	public final String name, password;

	public CreateRoomMessage(String name, String password) {
		this.name = name;
		this.password = password;
	}
	
	public CreateRoomMessage(Bundle bundle) {
		name = bundle.getString(FIELD_NAME);
		password = bundle.getString(FIELD_PASS);
	}

	@Override
	public Bundle pack() {
		Bundle data = new Bundle();
		data.putString(FIELD_NAME, name);
		data.putString(FIELD_PASS, password);
		return data;
	}
	
}
