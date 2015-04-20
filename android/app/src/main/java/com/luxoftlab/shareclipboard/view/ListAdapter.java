package com.luxoftlab.shareclipboard.view;

import java.util.ArrayList;
import java.util.HashMap;

import com.luxoftlab.shareclipboard.R;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class ListAdapter extends ArrayAdapter<RowData> {
	private static final String LOG = "activity";
	HashMap<String, RowData> rows;
	LayoutInflater inflator;
	public ListAdapter(Context context, int resource) {
		super(context, resource, new ArrayList<RowData>());
		rows = new HashMap<String, RowData>();
		inflator = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
	
	public void put(RowData row) {
		Log.d(LOG, "add room "+row.ip);
		RowData last = rows.put(row.ip, row);
		if(last != null) {
			remove(last);
		}
		add(row);
		notifyDataSetChanged();
	}
	
	public void remove(String ip) {
		RowData last = rows.get(ip);
		if(last != null) {
			Log.d(LOG, "delete room "+last.name);
			remove(last);
			notifyDataSetChanged();
		}
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		RowData row = getItem(position);
		convertView = inflator.inflate(R.layout.list_line, null);
		TextView name = (TextView)convertView.findViewById(R.id.name);
		TextView ip = (TextView)convertView.findViewById(R.id.ip);
		name.setText(row.name);
		ip.setText(row.ip);
		return convertView;
	}

}

