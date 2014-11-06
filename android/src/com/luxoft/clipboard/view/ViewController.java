package com.luxoft.clipboard.view;

import com.luxoft.clipboard.Controller;
import com.luxoft.clipboard.Main;
import com.luxoft.clipboard.MessageManager;
import com.luxoft.clipboard.R;
import com.luxoft.clipboard.messages.CreateRoomMessage;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;

public class ViewController implements OnClickListener, android.content.DialogInterface.OnClickListener {

	MessageManager connection;
	Context context;
	LayoutInflater inflater;
	AlertDialog createRoomDialog;
	
	public ViewController(Main activity) {
		connection = activity.getServiceConnection();
		context = activity;
		inflater = activity.getLayoutInflater();
		initCreateRoomDialog();
	}

	@Override
	public void onClick(View view) {
		switch(view.getId()) {
		case R.id.addRoom:
			createRoomDialog.show();
			break;
		}
	}

	@Override
	public void onClick(DialogInterface dialog, int which) {
		if(dialog == createRoomDialog) {
			EditText name = (EditText)createRoomDialog.findViewById(R.id.roomName);
			EditText password = (EditText)createRoomDialog.findViewById(R.id.password);
			CreateRoomMessage msg = new CreateRoomMessage(
						name.getText().toString(),
						password.getText().toString()
					);
			connection.send(Controller.MSG_CREATE_ROOM, msg);
		}
	}
	
	private void initCreateRoomDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(context);
		
		builder.setTitle("Room creation");
		builder.setView(inflater.inflate(R.layout.dialog_create_room, null));
		builder.setPositiveButton("OK", this);
		builder.setNegativeButton("Cancel", null);
		
		createRoomDialog = builder.create();
	}
	
}
