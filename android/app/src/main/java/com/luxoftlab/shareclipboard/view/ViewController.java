package com.luxoftlab.shareclipboard.view;

import com.luxoftlab.shareclipboard.Controller;
import com.luxoftlab.shareclipboard.Main;
import com.luxoftlab.shareclipboard.MessageManager;
import com.luxoftlab.shareclipboard.R;
import com.luxoftlab.shareclipboard.messages.CreateRoomMessage;
import com.luxoftlab.shareclipboard.messages.JoinRoomMessage;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.EditText;
import android.widget.TextView;

public class ViewController implements OnClickListener, DialogInterface.OnClickListener, OnItemClickListener {

	private MessageManager connection;
	private Context context;
	private LayoutInflater inflater;
	private AlertDialog createRoomDialog, joinRoomDialog,
						leaveRoomDialog;
	
	private String selectedRoom;
	
	public ViewController(Main activity) {
		connection = activity.getServiceConnection();
		context = activity;
		inflater = activity.getLayoutInflater();
		createRoomDialog = createDialog("Room creation", R.layout.dialog_create_room);
		joinRoomDialog = createDialog("Room creation", R.layout.dialog_join_room);
		leaveRoomDialog = createDialog("Are you sure to leave room?", 0);
	}

	@Override
	public void onClick(View view) {
		switch(view.getId()) {
		case R.id.addRoom:
			createRoomDialog.show();
			break;
		case R.id.leaveRoom:
			leaveRoomDialog.show();
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
		} else if(dialog == joinRoomDialog) {
			EditText password = (EditText)joinRoomDialog.findViewById(R.id.password);
			JoinRoomMessage msg = new JoinRoomMessage(
						selectedRoom,
						password.getText().toString()
					);
			connection.send(Controller.MSG_JOIN_ROOM, msg);
		} else if(dialog == leaveRoomDialog) {
			connection.send(Controller.MSG_LEAVE_ROOM, null);
		}
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
		TextView ip = (TextView)view.findViewById(R.id.ip);
		selectedRoom = ip.getText().toString();
		joinRoomDialog.show();
	}
	
	private AlertDialog createDialog(String title, int res) {
		AlertDialog.Builder builder = new AlertDialog.Builder(context);
		
		builder.setTitle(title);
		if(res != 0)
			builder.setView(inflater.inflate(res, null));
		builder.setPositiveButton("OK", this);
		builder.setNegativeButton("Cancel", null);
		
		return builder.create();
	}
}
