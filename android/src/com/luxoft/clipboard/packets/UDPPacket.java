package com.luxoft.clipboard.packets;

import java.nio.ByteBuffer;

public class UDPPacket {
	public static final byte ROOM = 0,
							 GET_ROOM = 1,
							 DELETE_ROOM = 2;
	private static final int CONTENT_OFFSET = 5;
	
	public byte type;
	public int id;
	public String content;
	
	public UDPPacket(byte[] data, int offset, int length) {
		ByteBuffer wrapped = ByteBuffer.wrap(data, offset, length);
		type = wrapped.get();
		id = wrapped.getInt();
		content = new String(data, offset+CONTENT_OFFSET, length-CONTENT_OFFSET);
	}
	
	public UDPPacket(byte type) {
		this.type = type;
		id = (int) (Math.random()*(0xffffffffL));
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
		if(tmp != null)
			buffer.put(tmp);
		byte[] data = new byte[length];
		buffer.position(0);
		buffer.get(data);
		return data;
	}
}
