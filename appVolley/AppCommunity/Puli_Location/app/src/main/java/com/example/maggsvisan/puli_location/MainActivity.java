package com.example.maggsvisan.puli_location;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final EditText username= (EditText)findViewById(R.id.username);
        final EditText password= (EditText)findViewById(R.id.password);

        final Button bLogin= (Button)findViewById(R.id.bLogin);

        bLogin.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent registerIntend = new Intent(MainActivity.this,UserAreaActivity.class);
                MainActivity.this.startActivity(registerIntend);
            }
        });
    }
}
