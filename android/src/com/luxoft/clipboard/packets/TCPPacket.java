package com.luxoft.clipboard.packets;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public abstract class TCPPacket {
	
	public static final byte AUTH = 0,
							 DEVICE = 1,
							 FAIL = 2,
							 DELETE_DEVICE = 3,
							 CLIPBOARD_TEXT = 4;
	
	public byte type;
	
	public TCPPacket(byte type) {
		this.type = type;
	}
	
	public String readString(DataInputStream in) throws IOException {
		//TODO read doc about readUTF()
		int length = in.readInt();
		byte[] temp = new byte[length];
		in.read(temp);
		return new String(temp);
	}
	
	public void writeString(DataOutputStream out, String str) throws IOException {
		//TODO read doc about readUTF()
		byte[] temp = str.getBytes();
		out.writeInt(temp.length);
		out.write(temp);
	}
	
	
	public abstract void read(DataInputStream in) throws IOException;
	
	public abstract void write(DataOutputStream out) throws IOException;
	
}
