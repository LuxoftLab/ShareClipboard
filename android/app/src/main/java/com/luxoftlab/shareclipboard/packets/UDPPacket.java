package com.luxoftlab.shareclipboard.packets;

import android.util.Log;

import java.nio.ByteBuffer;

public class UDPPacket {
    public static final String LOG = "UDPPacket";

	public static final byte ROOM = 0,
							 GET_ROOM = 1,
							 DELETE_ROOM = 2;
	private static final int CONTENT_OFFSET = 1 + 4 + 4;
	
	public byte type;
	public int id;
	public String content;
	
	public UDPPacket(byte[] data, int offset, int length) {
		ByteBuffer wrapped = ByteBuffer.wrap(data, offset, length);
		type = wrapped.get();
		id = wrapped.getInt();
        int stringLength = wrapped.getInt();
        Log.d(LOG, stringLength+"|"+offset+"|"+length+"|"+CONTENT_OFFSET);
        content = new String(data, offset+CONTENT_OFFSET, stringLength);
    }
	
	public UDPPacket(byte type) {
		this.type = type;
		id = (int) (Math.random()*100000);
	}
	
	public UDPPacket(byte type, String content) {
		this(type);
		this.content = content;
	}
	
	public byte[] getBinary() {
		byte[] tmp = null;
		int length = CONTENT_OFFSET;
		if(content != null) {
			tmp = content.getBytes();
			length += tmp.length;
		}
		ByteBuffer buffer = ByteBuffer.allocate(length);
		buffer.put(type);
		buffer.putInt(id);
		if(tmp != null) {
            buffer.putInt(tmp.length);
            buffer.put(tmp);
        } else {
            buffer.putInt(0);
        }
		byte[] data = new byte[length];
		buffer.position(0);
		buffer.get(data);
		return data;
	}
}
