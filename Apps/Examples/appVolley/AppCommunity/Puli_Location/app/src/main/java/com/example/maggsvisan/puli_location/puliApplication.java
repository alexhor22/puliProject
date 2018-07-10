package com.example.maggsvisan.puli_location;


import android.app.Application;
import android.text.TextUtils;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;

/**
 * Created by maggsvisan on 12/1/17.
 */

public class puliApplication extends Application {

    private static puliApplication mInstance;
    public static RequestQueue mRequestQueue;
    private static String TAG= "DEFAULT";

    public void onCreate(){
        super.onCreate();

        mInstance= this;
    }

    public static synchronized puliApplication getInstance() {
        return mInstance;
    }

    public RequestQueue getmRequestQueue(){
        if (mRequestQueue == null){
            mRequestQueue= Volley.newRequestQueue(this.getApplicationContext());
        }
        return mRequestQueue;
    }

    public <T> void addToRequestQueue(Request<T> request,String tag){
        request.setTag(TextUtils.isEmpty(tag)? TAG :tag);
        getmRequestQueue().add(request);
    }

    public <T> void addToRequestQueue(Request<T> request){
        request.setTag(TAG);
        getmRequestQueue().add(request);
    }

}
