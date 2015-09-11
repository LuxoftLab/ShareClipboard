package com.luxoftlab.shareclipboard;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.Log;

public class ClipboardService implements ClipboardManager.OnPrimaryClipChangedListener {
	private static final String LOG = "clipboard";
	
	private ClipboardManager clipboard;
	private ClientRoom room;
	
	private boolean locked = false;
	
	public ClipboardService(Controller controller, ClientRoom room) {
		clipboard = controller.getClipboardManager();
		this.room = room;
		clipboard.addPrimaryClipChangedListener(this);
	}
	
	@Override
	public void onPrimaryClipChanged() {
		if(locked) {
			locked = false;
			return;
		}
		ClipData.Item item = clipboard.getPrimaryClip().getItemAt(0);
		CharSequence result = item.getText();
		if(result == null) {
			Uri resultUri = item.getUri();
			if(resultUri == null) {
				Intent resultIntent = item.getIntent();
				Log.d(LOG, "clipboard intent");
			} else {
				Log.d(LOG, "clipboard uri");
			}
		} else {
			String data = result.toString();
			Log.d(LOG, "clipboard text: "+data);
			room.onTextCopied(data);
		}
	}
	
	public void putText(String text) {
		locked = true;
		Log.d(LOG, "put text: "+text);
		clipboard.setText(text);
	}
	
}
