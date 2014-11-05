package com.luxoft.clipboard;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import android.content.Context;
import android.net.DhcpInfo;
import android.net.wifi.WifiManager;
import android.util.Log;

public class UDPService extends Thread {
	private final static String LOG = "udp";
	
	public static final int PORT = 12345;
	
	private Controller controller;
	private DatagramSocket socket;
	private boolean enabled = false;
	private InetAddress local, broadcast;
	private int lastId;
	
	public UDPService(Controller controller) throws IOException {
		this.controller = controller;
		local = getLocal();
		broadcast = getBroadcastAddress();
		socket = new DatagramSocket(PORT);
	}
	
	@Override
	public void run() {
		byte[] buffer = new byte[1024];
		DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
		enabled = true;
		while(enabled) {
			try {
				socket.setBroadcast(true);
				Log.d(LOG, "receive...");
				socket.receive(packet);
				Log.d(LOG, "received");
				if(!packet.getAddress().equals(local)) {
					onPacket(
							new UDPPacket(packet.getData(), packet.getOffset(), packet.getLength()),
							packet.getAddress()
							);
				}
			} catch (IOException e) {
				Log.w(LOG, "receive exception");
				e.printStackTrace();
			}
		}
	}
	
	public void close() {
		enabled = false;
		socket.close();
	}
	
	public void getRooms() {
		UDPPacket packet = new UDPPacket(UDPPacket.GET_ROOM);
		sendBroadcast(packet);
	}
	
	public void deleteRoom() {
		UDPPacket packet = new UDPPacket(UDPPacket.DELETE_ROOM);
		sendBroadcast(packet);
	}
	
	public void sendRoom(InetAddress addr, String name) {
		UDPPacket packet = new UDPPacket(UDPPacket.ROOM, name);
		sendPacket(packet, addr);
	}
	
	public void notifyAboutRoom(String name) {
		UDPPacket packet = new UDPPacket(UDPPacket.ROOM, name);
		sendBroadcast(packet);
	}
	
	public InetAddress getLocalAddress() {
		return local;
	}
	
	private void onPacket(UDPPacket packet, InetAddress addr) {
		Log.d(LOG, "on packet: "+packet.type + " id: " + packet.id);
		if(lastId == packet.id) {
			return;
		}
		Log.d(LOG, "on packet: "+packet.type);
		lastId = packet.id;
		switch(packet.type) {
		case UDPPacket.GET_ROOM:
			controller.getRoom(addr);
			break;
		case UDPPacket.ROOM:
			controller.addRoom(packet.content, addr);
			break;
		case UDPPacket.DELETE_ROOM:
			controller.deleteRoom(addr);
			break;
		default:
			Log.w(LOG, "worng package type");
			break;
		}
	}
	
	private void sendBroadcast(UDPPacket packet) {
		sendPacket(packet, broadcast, true);
	}
	
	private void sendPacket(UDPPacket packet, InetAddress addr) {
		sendPacket(packet, addr, false);
	}
	
	private void sendPacket(UDPPacket packet, InetAddress addr, boolean isBroadcast) {
		byte[] buf = packet.getBinary();
		DatagramPacket datagram = new DatagramPacket(buf,buf.length, addr, PORT);
		Log.d(LOG, "send packet: "+packet.id);
		new UDPSender(datagram, socket, isBroadcast).start();
	}
	
	private InetAddress getBroadcastAddress() throws IOException {
		WifiManager wifi = (WifiManager) controller.getSystemService(Context.WIFI_SERVICE);
		DhcpInfo dhcp = wifi.getDhcpInfo();
		byte[] quads = new byte[4];
		int broadcast = (dhcp.ipAddress & dhcp.netmask) | ~dhcp.netmask;
		for (int k = 0; k < 4; k++)
			quads[k] = (byte) ((broadcast >> k * 8) & 0xFF);
		return InetAddress.getByAddress(quads);
	}
	
	private InetAddress getLocal() throws IOException {
		WifiManager wifi = (WifiManager) controller.getSystemService(Context.WIFI_SERVICE);
		DhcpInfo dhcp = wifi.getDhcpInfo();
		byte[] quads = new byte[4];
		int ip = dhcp.ipAddress;
		for (int k = 0; k < 4; k++)
			quads[k] = (byte) ((ip >> k * 8) & 0xFF);
		return InetAddress.getByAddress(quads);
	}
}
