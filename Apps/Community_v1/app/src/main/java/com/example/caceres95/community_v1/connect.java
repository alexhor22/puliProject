package com.example.caceres95.community_v1;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;


public class connect extends AppCompatActivity {

    Button btnDis;
    TextView status;
    Boolean connected = false;
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;

    private DatabaseReference db;
    private FirebaseAuth mAuth;
    private static final String TAG = "MainActivity";

    Handler mHandler;
    final int RECIEVE_MESSAGE = 1;
    private StringBuilder sb = new StringBuilder();
    private ConnectedThread mConnectedThread;

    //SPP UUID. Look for it
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        db = FirebaseDatabase.getInstance().getReference();
        mAuth = FirebaseAuth.getInstance();

        Intent newint = getIntent();
        address = newint.getStringExtra(com.example.caceres95.community_v1.MainActivity.EXTRA_ADDRESS); //receive the address of the bluetooth device

        //view of the ledControl
        setContentView(R.layout.activity_connect);

        //call the widgtes
        btnDis = findViewById(R.id.btnDis);
        status = findViewById(R.id.alarmStatus);

        new ConnectBT().execute(); //Call the class to connect


        btnDis.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Disconnect(); //close connection
            }
        });

        mHandler = new Handler() {
            public void handleMessage(android.os.Message msg) {
                switch (msg.what) {
                    case RECIEVE_MESSAGE:                                                   // if receive massage
                        byte[] readBuf = (byte[]) msg.obj;
                        String strIncom = new String(readBuf, 0, msg.arg1);                 // create string from bytes array
                        sb.append(strIncom);                                                // append string
                        int endOfLineIndex = sb.indexOf("\r\n");                            // determine the end-of-line
                        if (endOfLineIndex > 0) {                                            // if end-of-line,
                            char data = sb.substring(0, endOfLineIndex).charAt(0);               // extract string
                            db.child("Board").child("Status").setValue(Integer.parseInt(sb.substring(0,1)));
                            sb.delete(0, sb.length());                                      // and clear
                            if(data == '0')
                            {
                                status.setText("No Alarm");            // update TextView
                            }
                            else if(data == '1')
                            {
                                status.setText("Crime");
                            }
                            else if(data == '2')
                            {
                                status.setText("Personal Emergency");
                            }
                            else if(data == '3')
                            {
                                status.setText("Suspicious Activity");
                            }
                            else
                            {
                                //status.setText("-" + sbprint + "-");
                                status.setText(data);
                            }

                        }
                        break;
                }
            };
        };


    }

    @Override
    public void onStart() {
        super.onStart();

        //FirebaseUser currentUser = mAuth.getCurrentUser();

        // Add value event listener to the post
        // [START post_value_event_listener]
        db.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Long actStatus = dataSnapshot.child("Board").child("Status").getValue(Long.class);
                String message;
                char info;
                if(actStatus != null)
                {
                    if(actStatus == 0)
                    {
                        message = "No Alarm";
                        info = 0;
                    }
                    else if(actStatus == 1)
                    {
                        message = "Crime";
                        info = 'R';
                    }
                    else if(actStatus == 2)
                    {
                        message = "Personal Emergency";
                        info = 'B';

                    }
                    else if(actStatus == 3)
                    {
                        message = "Suspicious Activity";
                        info = 'G';
                    }
                    else
                    {
                        message = "No Alarm";
                        info = '0';
                    }
                    status.setText(message);
                    if(connected)
                    {
                        sendBT(info);
                    }

                }
            }

            @Override
            public void onCancelled(DatabaseError error) {
                // Failed to read value
                Log.w(TAG, "Failed to read value.", error.toException());
            }
        });
    }

    private void sendBT(char info)
    {
        if (btSocket!=null) //If the btSocket is busy
        {
            try
            {
                btSocket.getOutputStream().write(info); //close connection
            }
            catch (IOException e)
            {
                //msg("Error");
            }
        }
        //finish(); //return to the first layout

    }

    private void Disconnect()
    {
        if (btSocket!=null) //If the btSocket is busy
        {
            try
            {
                btSocket.close(); //close connection
            }
            catch (IOException e)
            { msg("Error");}
        }
        finish(); //return to the first layout

    }


    // fast way to call Toast
    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(connect.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                    connected = true;
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            }
            else
            {
                msg("Connected.");
                isBtConnected = true;
                mConnectedThread = new ConnectedThread(btSocket);
                mConnectedThread.start();
            }
            progress.dismiss();
        }
    }

    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[256];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.read(buffer);
                    // Send the obtained bytes to the UI activity
                    mHandler.obtainMessage(RECIEVE_MESSAGE, bytes, -1, buffer)
                            .sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        public void write(byte[] bytes) {
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) { }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }
}
