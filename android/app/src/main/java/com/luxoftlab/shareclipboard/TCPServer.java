package com.luxoftlab.shareclipboard;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import android.util.Log;

public class TCPServer extends Thread {
	private static final String LOG = "tcpserver";
	
	public static final int PORT = 1080;
	
	private ServerRoom room;
	private ServerSocket server;
	private boolean enabled = false;
	
	public TCPServer(ServerRoom room) throws IOException {
		this.room = room;
		server = new ServerSocket(PORT);
	}
	
	@Override
	public void run() {
		try {
			enabled = true;
			while(enabled) {
				Log.d(LOG, "wait connection...");
				Socket client = server.accept();
				Log.d(LOG, "got connection");
				room.addDevice(client);
			}
		} catch (IOException e) {
			Log.w(LOG, "tcp server exception");
			e.printStackTrace();
		}
	}
	
	public void close() {
		enabled = false;
		try {
			server.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
