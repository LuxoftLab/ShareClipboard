package com.luxoftlab.shareclipboard;

import com.luxoftlab.shareclipboard.messages.Packable;

import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

public class MessageManager extends Handler {
	private static final String LOG = "handler";
	
	private Listener listener;
	private Messenger target, self;
	
	public MessageManager(Listener listener) {
		this.listener = listener;
		self = new Messenger(this);
	}
	
	@Override
	public void handleMessage(Message msg) {
		if(!listener.onMessage(msg))
			super.handleMessage(msg);
	}
	
	public void setTarget(Messenger taget) {
		this.target = taget;
	}
	
	public IBinder getBinder() {
		return self.getBinder();
	}
	
	public void send(int type, Packable data) {
		if(target == null) return;
		Message msg = Message.obtain(null, type);
		if(data != null) msg.setData(data.pack());
		msg.replyTo = self;
		try {
			target.send(msg);
		} catch (RemoteException e) {
			Log.w(LOG, "cannot send message");
			e.printStackTrace();
		}
	}
	
	interface Listener {
		public boolean onMessage(Message msg);
	}
}
