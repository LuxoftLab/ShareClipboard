package com.luxoft.clipboard;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;

import android.util.Log;

public class UDPPackage {
	private static String LOG = "udppackage";
	private static final String CHARSET = "UTF-8";
	public static final byte NEW_ROOM = 0;
	public static final byte GET_ROOM = 1;
	
	private byte type;
	private int id;
	private String content;
	private byte[] data;
	
	public UDPPackage(byte[] data, int offset, int len) {
		ByteBuffer wrapped = ByteBuffer.wrap(data, offset, len);
		type = wrapped.get();
		id = wrapped.getInt();
		int length = len - 5;
		if(length == 0) {
			return;
		}
		try {
			content = new String(data, wrapped.position(), length, CHARSET);
		} catch (UnsupportedEncodingException e) {
			Log.d(LOG, "connot convert to "+CHARSET);
			e.printStackTrace();
		}
	}
	
	public UDPPackage(byte type) {
		this.type = type;
	}
	
	public UDPPackage(byte type, String content) {
		this.type = type;
		this.content = content;
	}
	
	public byte[] toBytes() {
		int len = 1+4; //type + id
		int length = 0;
		byte str[] = null;
		if(content != null) {
			try {
				str = content.getBytes(CHARSET);
				length = str.length;
			} catch (UnsupportedEncodingException e) {
				Log.d(LOG, "connot convert to "+CHARSET);
				e.printStackTrace();
			}
		}
		len += length;
		data = new byte[length];
		ByteBuffer wrapped = ByteBuffer.wrap(data, 0, len);
		wrapped.put(type);
		wrapped.putInt((int)(Math.random()*100000));
		if(length > 0) {
			wrapped.put(str);
		}
		return data;
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
		return ((UDPPackage)o).id == id;
	}
}
