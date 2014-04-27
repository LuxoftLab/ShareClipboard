package com.luxoft.clipboard;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;

import android.util.Log;

public class Packet {
	
	private final String LOG_NAME = "packet";
	private final String CHARSET = "UTF-16BE";
	
	public static final int INVALID = 0;
	public static final int LOOKUP = 1;
	public static final int HELLO = 2;
	public static final int CLIPBOARD = 4;
	public static final int ARE_YOU_HERE = 8;
	
	private int type;
	private String content;
	private byte[] data;
	
	public Packet(byte[] data, int offset, int len) {
		this.data = data;
		ByteBuffer wrapped = ByteBuffer.wrap(data, offset, len);
		type = wrapped.getInt();
		int length = wrapped.getInt();
		try {
			content = new String(data, 8, length, CHARSET);
		} catch (UnsupportedEncodingException e) {
			Log.d(LOG_NAME, "connot convert to UTF-16");
			e.printStackTrace();
		}
	}
	
	public Packet(int type, String msg) {
		try {
			byte[] temp = msg.getBytes(CHARSET);
			Log.d(LOG_NAME, "string length: "+temp.length);
			ByteBuffer buffer = ByteBuffer.allocate(temp.length + 8);
			buffer.putInt(type);
			buffer.putInt(temp.length);
			buffer.put(temp);
			data = new byte[temp.length + 8];
			Log.d(LOG_NAME, "remaining: "+buffer.remaining());
			Log.d(LOG_NAME, "position: "+buffer.position());
			buffer.position(0);
			Log.d(LOG_NAME, "position: "+buffer.position());
			buffer.get(data);
		} catch (UnsupportedEncodingException e) {
			Log.d(LOG_NAME, "connot convert to UTF-16");
			e.printStackTrace();
		}
	}
	
	public int getType() {
		return type;
	}

	public String getContent() {
		return content;
	}
	
	public byte[] getData() {
		return data;
	}
}
