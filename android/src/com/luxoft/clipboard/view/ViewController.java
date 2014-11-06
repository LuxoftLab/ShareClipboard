package com.luxoft.clipboard.view;

import com.luxoft.clipboard.Controller;
import com.luxoft.clipboard.MessageManager;

import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;

public class ViewController implements OnClickListener {

	MessageManager connection;
	
	public ViewController(MessageManager connection) {
		this.connection = connection;
	}

	@Override
	public void onClick(View view) {
		Bundle data = new Bundle();
		data.putString("name", "testRoom");
		data.putString("pass", "password");
		connection.send(Controller.MSG_CREATE_ROOM, data);
	}
	
}
