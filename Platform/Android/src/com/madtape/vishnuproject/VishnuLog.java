package com.madtape.vishnuproject;

import android.app.Activity;
import android.widget.ScrollView;
import android.widget.TextView;

public class VishnuLog {

	private static String mStrLog = null;
	private static TextView mView = null;
	private static ScrollView mScroll = null;
	private static Activity mActivity = null;
	static void setView(Activity act, ScrollView scroll, TextView view) {
		mView = view;
		mScroll = scroll;
		mActivity = act;
	}
	static void addLog(String logmsg) {
		final StringBuilder msg = new StringBuilder();
		msg.append(mStrLog);
		msg.append(logmsg);
		if(mView != null) {
			mActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					mView.setText(msg);
					mScroll.fullScroll(ScrollView.FOCUS_DOWN);
				}
			});
		}
		mStrLog = msg.toString();
	}
}
