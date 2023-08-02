package com.example.ad9854controller;

import androidx.activity.result.ActivityResult;
import androidx.activity.result.ActivityResultCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.app.ActivityCompat;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MyDebug";

    //从这里开始，就是
    ActivityResultLauncher<Intent> startBlueTooth = registerForActivityResult(new ActivityResultContracts.StartActivityForResult(), new ActivityResultCallback<ActivityResult>() {
        @Override
        public void onActivityResult(ActivityResult result)
        {
            if (result==null)
                Log.e(TAG,"打开失败");
            else if(result.getResultCode()==RESULT_CANCELED)
                Log.e(TAG,"用户取消");
        }
    });


    @SuppressLint("ClickableViewAccessibility")

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        int REQUEST_ENABLE_BIT = 1;


        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //验证设备是否支持蓝牙
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();     //一般都有蓝牙，这时候bluetoothAdapter就表示蓝牙的适配器。
        if (bluetoothAdapter == null)
            Log.e(TAG, "设备不支持蓝牙");
        if (!bluetoothAdapter.isEnabled()) {                //如果蓝牙没有打开，那么就弹窗提醒打开
            startBlueTooth.launch(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE));
        }



        /*
        if (!bluetoothAdapter.isEnabled()) {                //如果蓝牙没有打开，那么就弹窗提醒打开
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                Log.e(TAG,"Something wrong");
                return;
            }
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BIT);
        } */

    }
}