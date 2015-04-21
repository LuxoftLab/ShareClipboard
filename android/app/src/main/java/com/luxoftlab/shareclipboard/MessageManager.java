package com.luxoftlab.shareclipboard;

import com.luxoftlab.shareclipboard.messages.Packable;

import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.util.SparseArray;

public class MessageManager extends Handler {
	private static final String LOG = "handler";
	
	private SparseArray<Listener> listeners;
	private Messenger target, self;
	
	public MessageManager() {
		listeners = new SparseArray<Listener>();
		self = new Messenger(this);
	}
	
	@Override
	public void handleMessage(Message msg) {
        target = msg.replyTo;
		Listener listener = listeners.get(msg.what);
        if(listener != null) {
            listener.onMessage(msg.getData());
        } else {
            Log.e(LOG, "unexpected message");
        }
	}
	
	public void setTarget(Messenger target) {
		this.target = target;
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

    public void addListener(int msg, Listener listener) {
        if(listeners.get(msg) != null) {
            Log.e(LOG, "overwrite listener");
        }
        listeners.put(msg, listener);
    }
	
	interface Listener {
		public void onMessage(Bundle bundle);
	}
}
