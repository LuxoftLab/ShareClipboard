package com.luxoft.clipboard.packets;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class TextPacket extends TCPPacket {
	
	public String text;
	
	public TextPacket() {
		super(TCPPacket.CLIPBOARD_TEXT);
	}
	
	public TextPacket(String text) {
		this();
		this.text = text;
	}

	@Override
	public void read(DataInputStream in) throws IOException {
		text = readString(in);
	}

	@Override
	public void write(DataOutputStream out) throws IOException {
		writeString(out, text);
	}

}
