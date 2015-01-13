package com.madtape.vishnuproject;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;


public class MainActivity extends Activity implements OnClickListener {

	private GLES2View mGLView = null;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // String path = getFilesDir().getAbsolutePath();
        String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Vishnu";
        extractResources(path);
        FrameLayout layout = new FrameLayout(this);
        //layout.setOrientation(LinearLayout.VERTICAL);
        GLES2View glesView = new GLES2View(this, path);
        ScrollView scroll = new ScrollView(this);
        TextView logView = new TextView(this);
        logView.setBackgroundColor(0x80000000);
        scroll.addView(logView);
        
        
        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.HORIZONTAL);
        panel.addView(scroll, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
      
/*
        Button btn = new Button(this);
        btn.setText("Restart");
        btn.setOnClickListener(this);
        panel.addView(btn);
*/        
        LinearLayout front = new LinearLayout(this);
        front.setOrientation(LinearLayout.VERTICAL);
        View dummy = new View(this);
        front.addView(dummy, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 0, 8));
        front.addView(panel, new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 0, 2));
        dummy.setBackgroundColor(0x00000000);        
        layout.addView(glesView);
        layout.addView(front);
        
        mGLView = glesView;
       
        VishnuLog.setView(this, scroll, logView);        
        setContentView(layout);
    }
    
    private static String archive = "vishnu.zip";
    
    private boolean extractResources(String basePath) {
    	boolean result = true;
    	// 展開先パスが既に存在していたら展開済みとして、何もせずにリターン。既存のものを利用する。
    	File pathDir = new File(basePath);
    	if(pathDir.exists()) return true;
    	pathDir.mkdirs();
 
    	try {
    		AssetManager assetManager = getAssets();
    		InputStream inputStream = assetManager.open(archive, AssetManager.ACCESS_STREAMING);
    		ZipInputStream zipInputStream = new ZipInputStream(inputStream);
    		ZipEntry zipEntry = zipInputStream.getNextEntry();
    		
    		while(zipEntry != null){
				String path = basePath + "/" + zipEntry.getName();
				File outputFile = new File(path);
    			if(zipEntry.isDirectory()) {
    				outputFile.mkdirs();
    			} else {
    				FileOutputStream fos = new FileOutputStream(outputFile.getAbsoluteFile());
    				byte [] buffer = new byte [4096];
    				int size = 0;
    				while((size = zipInputStream.read(buffer)) != -1) {
    					fos.write(buffer, 0, size);
    				}
    				fos.close();
    			}
    			zipInputStream.closeEntry();
    			zipEntry = zipInputStream.getNextEntry();	
    		}
    		zipInputStream.close();
    	} catch(Exception e) {
    		result = false;
    	}
    	return result;
    }
    
    @Override
    public void onPause() {
    	super.onPause();
    	mGLView.onPause();
    }
    
    @Override
    public void onResume() {
    	super.onResume();
    	mGLView.onResume();
    }

	@Override
	public void onClick(View v) {
		mGLView.Restart();
	}
}

