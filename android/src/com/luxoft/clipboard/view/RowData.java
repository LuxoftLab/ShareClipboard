package com.luxoft.clipboard.view;

public class RowData {
	String name, ip;
	
	public RowData(String name, String ip) {
		this.name = name;
		this.ip = ip;
	}
	
	@Override
	public boolean equals(Object o) {
		return ip.equals(((RowData)o).ip);
	}
}
