package com.luxoft.clipboard;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;

class MessageManager extends Handler {
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
	
	public void send(int type, Bundle data) {
		if(target == null) {
			Log.w(LOG, "cannot send message");
			return;
		}
		Message msg = Message.obtain(null, type);
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
