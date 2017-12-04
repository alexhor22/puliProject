package com.example.maggsvisan.puli_location;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class RegisterActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        final EditText FName= (EditText)findViewById(R.id.FName);
        final EditText LName= (EditText)findViewById(R.id.LName);
        final EditText username= (EditText)findViewById(R.id.username);
        final EditText password= (EditText)findViewById(R.id.password);
        final EditText numHome= (EditText)findViewById(R.id.numHome);
        final EditText neighborhood= (EditText)findViewById(R.id.neighborhood);
        final EditText postalCode= (EditText)findViewById(R.id.postalCode);
        final EditText phone= (EditText)findViewById(R.id.phone);

        final Button bRegister= (Button)findViewById(R.id.bRegister);

        bRegister.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent registerIntend = new Intent(RegisterActivity.this,UserAreaActivity.class);
                RegisterActivity.this.startActivity(registerIntend);
            }
        });
    }
}
