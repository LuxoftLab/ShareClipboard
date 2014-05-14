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
	
	private static final int DATA_OFFSET = 12;
	
	private int type;
	private int id;
	private String content;
	private byte[] data;
	
	public Packet(byte[] data, int offset, int len) {
		this.data = data;
		ByteBuffer wrapped = ByteBuffer.wrap(data, offset, len);
		type = wrapped.getInt();
		id = wrapped.getInt();
		int length = wrapped.getInt();
		try {
			content = new String(data, DATA_OFFSET, length, CHARSET);
		} catch (UnsupportedEncodingException e) {
			Log.d(LOG_NAME, "connot convert to UTF-16");
			e.printStackTrace();
		}
	}
	
	public Packet(int type, String msg) {
		Log.d(LOG_NAME, "msg: "+msg);
		this.type = type;
		this.content = msg;
		this.id = (int)Math.floor(Math.random()*0xffffff);
		try {
			byte[] temp = msg.getBytes(CHARSET);
			ByteBuffer buffer = ByteBuffer.allocate(temp.length + DATA_OFFSET);
			buffer.putInt(type);
			buffer.putInt(this.id);
			buffer.putInt(temp.length);
			buffer.put(temp);
			data = new byte[temp.length + DATA_OFFSET];
			buffer.position(0);
			buffer.get(data);
		} catch (UnsupportedEncodingException e) {
			Log.d(LOG_NAME, "connot convert to UTF-16");
			e.printStackTrace();
		}
	}
	
	public int getType() {
		return type;
	}
	
	public int getId() {
		return id;
	}

	public String getContent() {
		return content;
	}
	
	@Override
	public boolean equals(Object o) {
		if(o == null) {
			return false;
		}
		return ((Packet)o).id == id;
	}
	
	public byte[] getData() {
		return data;
	}
}
